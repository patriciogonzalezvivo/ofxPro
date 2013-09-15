
#include "UIProject.h"

void UIProject::setup(){
	
	cout << "SETTING UP SYSTEM " << getSystemName() << endl;
	background = NULL;
    
	ofAddListener(ofEvents().exit, this, &UIProject::exit);
	
    ofDirectory dir;
    string directoryName = getDataPath()+"Presets/";
    if(!dir.doesDirectoryExist(directoryName)){
        dir.createDirectory(directoryName);
    }
    
    string workingDirectoryName = directoryName+"Working/";
    if(!dir.doesDirectoryExist(workingDirectoryName)){
        dir.createDirectory(workingDirectoryName);
    }
    
    setupAppParams();
    setupDebugParams();
    setupCameraParams();
	setupLightingParams();
	setupMaterialParams();
    
    selfSetup();
    setupCoreGuis();
    selfSetupGuis();
    
	hideGUIS();
    
    ofRegisterMouseEvents(this);
    ofRegisterKeyEvents(this);
    ofAddListener(ofEvents().update, this, &UIProject::update);
    ofAddListener(ofEvents().draw, this, &UIProject::draw);
    
    loadGUIS();
    hideGUIS();
}

void UIProject::play(){
    cam.enableMouseInput();
    for(map<string, Light *>::iterator it = lights.begin(); it != lights.end(); ++it){
        it->second->light.setup();
    }
    
    selfBegin();
    
    bDebug = false;
}

void UIProject::stop(){
    
    selfEnd();
    
    hideGUIS();
    saveGUIS();
    cam.disableMouseInput();
    for(map<string, Light *>::iterator it = lights.begin(); it != lights.end(); ++it){
        it->second->light.destroy();
    }
    
    ofUnregisterMouseEvents(this);
    ofUnregisterKeyEvents(this);
    ofRemoveListener(ofEvents().update, this, &UIProject::update);
    ofRemoveListener(ofEvents().draw, this, &UIProject::draw);
}

void UIProject::update(ofEventArgs & args){

    if(bUpdateSystem){
        for(vector<ofx1DExtruder *>::iterator it = extruders.begin(); it != extruders.end(); ++it){
            (*it)->update();
        }
        
        selfUpdate();
    }
    
}

void UIProject::draw(ofEventArgs & args){
    ofPushStyle();
    
    if(bRenderSystem){
        
        {
            UIProject::getRenderTarget().begin();
            
            
            //  Background
            //
            if ( background != NULL ){
                background->draw();
            }
            
            {
                ofPushStyle();
                ofPushMatrix();
                selfDrawBackground();
                ofPopMatrix();
                ofPopStyle();
            }
            
            //  Start 3D scene
            //
            {
                getCameraRef().begin();
                
                //  Rotate Camera
                //
                ofRotateX(xRot->getPos());
                ofRotateY(yRot->getPos());
                ofRotateZ(zRot->getPos());
                
                //  Scene Setup
                //
                selfSceneTransformation();
                glEnable(GL_DEPTH_TEST);
            
                //  Draw Debug
                //
                if( bDebug ){
                    ofPushStyle();
                    ofPushMatrix();
                    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
                    
                    selfDrawDebug();
                    
                    ofPopMatrix();
                    ofPopStyle();
                }
                
                //  Draw Scene
                //
                {
                    lightsBegin();
                    ofPushStyle();
                    
                    selfDraw();
                    
                    ofPopStyle();
                    lightsEnd();
                }
                
                getCameraRef().end();
            }
            
            //  Draw Overlay
            //
            {
                ofPushStyle();
                ofPushMatrix();
                selfDrawOverlay();
                ofPopMatrix();
                ofPopStyle();
            }
            
            UIProject::getRenderTarget().end();
        }
        
        //  Post-Draw ( shader time )
        //
        selfPostDraw();
	}
    ofPopStyle();
}

void UIProject::exit(ofEventArgs & args){
//    delete bgColor;
//    delete bgColor2;
    
    saveGUIS();
    
    for(vector<ofx1DExtruder *>::iterator it = extruders.begin(); it != extruders.end(); ++it){
        ofx1DExtruder *e = (*it);
        delete e;
    }
    extruders.clear();
    
    for(map<string, Light *>::iterator it = lights.begin(); it != lights.end(); ++it){
        Light *l = it->second;
        delete l;
    }
    lights.clear();
    
    for(map<string, Material *>::iterator it = materials.begin(); it != materials.end(); ++it){
        Material *m = it->second;
        delete m;
    }
    materials.clear();
    materialGuis.clear();

    selfExit();
    
    deleteGUIS();
}

void UIProject::keyPressed(ofKeyEventArgs & args){
	
    for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it){
        if((*it)->hasKeyboardFocus())
        {
            return;
        }
    }
	
    switch (args.key){
        case '1':
            toggleGuiAndPosition(gui);
            break;
        case '2':
            toggleGuiAndPosition(sysGui);
            break;
        case '3':
            toggleGuiAndPosition(rdrGui);
            break;
        case '4':
            toggleGuiAndPosition(bgGui);
            break;
        case '5':
            toggleGuiAndPosition(lgtGui);
            break;
        case '0':
            toggleGuiAndPosition(camGui);
            break;
            
        case 'u':{
            bUpdateSystem = !bUpdateSystem;
        }
            break;

        case 's':{
            ofImage img;
            img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
			if( !ofDirectory(getDataPath()+"snapshots/").exists() ){
				ofDirectory(getDataPath()+"snapshots/").create();
			}
            img.saveImage(getDataPath()+"snapshots/" + getSystemName() + " " + ofGetTimestampString() + ".png");
        }
            break;
            
        case 'h':{
			toggleGUIS();
        }
            break;
            
        case 'f':{
            cout << "FULLSCREEN" << endl;
            ofToggleFullscreen();
        }
            break;
            
        case 'p':{
            for(int i = 0; i < guis.size(); i++){
                guis[i]->setDrawWidgetPadding(true);
            }
        }
            break;
            
        case 'e':{
			ofxUISuperCanvas *last = NULL;
            for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it){
                (*it)->setMinified(false);
                if(last != NULL){
                    (*it)->getRect()->setX(last->getRect()->getX());
                    (*it)->getRect()->setY(last->getRect()->getY()+last->getRect()->getHeight()+1);
                    
                    if((*it)->getRect()->getY()+(*it)->getRect()->getHeight() > ofGetHeight() ){
                        (*it)->getRect()->setX(last->getRect()->getX()+last->getRect()->getWidth()+1);
                        (*it)->getRect()->setY(1);
                    }
                } else {
                    (*it)->getRect()->setX(1);
                    (*it)->getRect()->setY(1);
                }
                last = (*it);
            }
        }
            break;
            
        case 'r':{
            float maxY = 0;
            ofxUISuperCanvas *last = NULL;
            for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it){
                if(last != NULL){
                    (*it)->getRect()->setX( last->getRect()->getX()+last->getRect()->getWidth()+1 );
                    (*it)->getRect()->setY( last->getRect()->getY() );
                    
                    if((*it)->getRect()->getX()+(*it)->getRect()->getWidth() > ofGetWidth() ){
                        (*it)->getRect()->setX(1);
                        (*it)->getRect()->setY(maxY+1);
                    }
                    
                } else {
                    (*it)->getRect()->setX(1);
                    (*it)->getRect()->setY(1);
                    
                }
                last = (*it);
                last->setMinified(false);
                
                float totalY = last->getRect()->getY()+last->getRect()->getHeight();
                if ( totalY > maxY ){
                    maxY = totalY;
                }
                
            }
        }
            break;
            
        case 't':{
            ofxUISuperCanvas *last = NULL;
            for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it){
                (*it)->setMinified(true);
                if(last != NULL){
                    (*it)->getRect()->setX(1);
                    (*it)->getRect()->setY(last->getRect()->getY()+last->getRect()->getHeight()+1);
                } else {
                    (*it)->getRect()->setX(1);
                    (*it)->getRect()->setY(1);
                }
                last = (*it);
            }
        }
            break;
            
        case 'y':{
            float x = ofGetWidth()*.5;
            float y = ofGetHeight()*.5;
            float tempRadius = gui->getGlobalCanvasWidth()*2.0;
            float stepSize = TWO_PI/(float)guis.size();
            float theta = 0;
            for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it)
            {
                (*it)->getRect()->setX(x+sin(theta)*tempRadius - (*it)->getRect()->getHalfWidth());
                (*it)->getRect()->setY(y+cos(theta)*tempRadius - (*it)->getRect()->getHalfHeight());
                theta +=stepSize;
            }
        }
            break;
            
        case '=':{
            for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it){
                (*it)->toggleMinified();
            }
        }
		break;

        default:
            selfKeyPressed(args);
            break;
    }
}

void UIProject::keyReleased(ofKeyEventArgs & args){
    switch (args.key){
        case 'p':{
            for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it){
                (*it)->setDrawWidgetPadding(false);
            }
        }
		break;
            
        default:
            selfKeyReleased(args);
            break;
    }
}

void UIProject::mouseDragged(ofMouseEventArgs& data){
    selfMouseDragged(data);
}

void UIProject::mouseMoved(ofMouseEventArgs& data){
    selfMouseMoved(data);
}

void UIProject::mousePressed(ofMouseEventArgs & args){
	if(cursorIsOverGUI()){
		cam.disableMouseInput();
	}
    selfMousePressed(args);
}

bool UIProject::cursorIsOverGUI(){
	
    for(int i = 0; i < guis.size(); i++){
		if(guis[i]->isHit(ofGetMouseX(), ofGetMouseY())){
			return true;
		}
	}
    
	return false;
}

void UIProject::mouseReleased(ofMouseEventArgs & args){
    cam.enableMouseInput();
    
    selfMouseReleased(args);
}

//------------------------------------------------------------ SETUP

void UIProject::setupAppParams(){
    ofSetSphereResolution(30);
    bRenderSystem = true;
    bUpdateSystem = true;
}

void UIProject::setupDebugParams(){
    //DEBUG
    bDebug = false;
}

void UIProject::setupCameraParams(){
    //CAMERA
    camFOV = 60;
    camDistance = 200;
    cam.setDistance(camDistance);
    cam.setFov(camFOV);

    xRot = new ofx1DExtruder(0);
    yRot = new ofx1DExtruder(0);
    zRot = new ofx1DExtruder(0);
    xRot->setPhysics(.9, 5.0, 25.0);
    yRot->setPhysics(.9, 5.0, 25.0);
    zRot->setPhysics(.9, 5.0, 25.0);
    
    extruders.push_back(xRot);
    extruders.push_back(yRot);
    extruders.push_back(zRot);
}

void UIProject::setupLightingParams(){
    //LIGHTING
    bSmoothLighting = true;
    bEnableLights = true;
    globalAmbientColor = new float[4];
    globalAmbientColor[0] = 0.5;
    globalAmbientColor[1] = 0.5;
    globalAmbientColor[2] = 0.5;
    globalAmbientColor[3] = 1.0;
}

void UIProject::setupMaterialParams(){
    mat = new Material();
}

void UIProject::setupCoreGuis(){
    
    setupGui();
    setupSystemGui();
    setupRenderGui();
    
    background = new UIBackground();
    background->setLinkedUIs( &guis );
    addGuiClass( *background );
    
    setupLightingGui();
    setupCameraGui();
    setupMaterial("MATERIAL 1", mat);
    setupPointLight("POINT LIGHT 1");
    setupPresetGui();
}

void UIProject::setupGui(){
    gui = new ofxUISuperCanvas(ofToUpper(getSystemName()));
    gui->setName("Settings");
    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    ofxUIFPS *fps = gui->addFPS();
    gui->resetPlacer();
    gui->addWidgetDown(fps, OFX_UI_ALIGN_RIGHT, true);
    gui->addWidgetToHeader(fps);
    
    gui->addSpacer();
    gui->addButton("SAVE", false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    ofxUIButton *loadbtn = gui->addButton("LOAD", false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    ofxUIButton *updatebtn = gui->addToggle("UPDATE", &bUpdateSystem);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    ofxUIButton *renderbtn = gui->addToggle("RENDER", &bRenderSystem);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addWidgetNorthOf(loadbtn, "RENDER", true);
    gui->setPlacer(updatebtn);
    gui->addSpacer();
    selfSetupGui();
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&UIProject::guiEvent);
    guis.push_back(gui);
    guimap[gui->getName()] = gui;
}

vector<string> UIProject::getPresets(){
	vector<string> presets;
	string presetPath = getDataPath()+"Presets/";
	ofDirectory presetsFolder = ofDirectory(presetPath);
	cout << "PRESET PATH AT " << presetPath << endl;
	
	if(presetsFolder.exists()){
		presetsFolder.listDir();
		cout << " found " << presetsFolder.size() << " files " << endl;
		for(int i = 0; i < presetsFolder.size(); i++){
			if(presetsFolder.getFile(i).isDirectory() &&
               ofFilePath::removeTrailingSlash(presetsFolder.getName(i)) != "Working" &&
			   presetsFolder.getName(i).at(0) != '_') //use leading _ to hide folders
            {
				presets.push_back(presetsFolder.getName(i));
			}
		}
	}
	return presets;
}

void UIProject::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE"){
        ofxUIButton *b = (ofxUIButton *) e.widget;
        if(b->getValue()){
            string presetName = ofSystemTextBoxDialog("Save Preset As");
            if(presetName.length()){
                savePresetGUIS(presetName);
            } else {
                saveGUIS();
            }
        }
    } else if(name == "LOAD"){
        ofxUIButton *b = (ofxUIButton *) e.widget;
        if(b->getValue()){
            ofFileDialogResult result = ofSystemLoadDialog("Load Visual System Preset Folder", true, getDataPath()+"Presets/");
            if(result.bSuccess && result.fileName.length()){
                loadPresetGUISFromPath(result.filePath);
            } else {
                loadGUIS();
            }
        }
    }
    

	
    selfGuiEvent(e);
}

void UIProject::setupSystemGui(){
    sysGui = new ofxUISuperCanvas("SYSTEM", gui);
    sysGui->copyCanvasStyle(gui);
    sysGui->copyCanvasProperties(gui);
    sysGui->setPosition(guis[guis.size()-1]->getRect()->x+guis[guis.size()-1]->getRect()->getWidth()+1, 0);
    sysGui->setName("SystemSettings");
    sysGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    ofxUIToggle *toggle = sysGui->addToggle("DEBUG", &bDebug);
    toggle->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
    sysGui->resetPlacer();
    sysGui->addWidgetDown(toggle, OFX_UI_ALIGN_RIGHT, true);
    sysGui->addWidgetToHeader(toggle);
    sysGui->addSpacer();
    
    selfSetupSystemGui();
    sysGui->autoSizeToFitWidgets();
    ofAddListener(sysGui->newGUIEvent,this,&UIProject::guiSystemEvent);
    guis.push_back(sysGui);
    guimap[sysGui->getName()] = sysGui;
}

void UIProject::setupRenderGui(){
    rdrGui = new ofxUISuperCanvas("RENDER", gui);
    rdrGui->copyCanvasStyle(gui);
    rdrGui->copyCanvasProperties(gui);
    rdrGui->setPosition(guis[guis.size()-1]->getRect()->x+guis[guis.size()-1]->getRect()->getWidth()+1, 0);
    rdrGui->setName("RenderSettings");
    rdrGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    rdrGui->addSpacer();
    selfSetupRenderGui();
    
    rdrGui->autoSizeToFitWidgets();
    ofAddListener(rdrGui->newGUIEvent,this,&UIProject::guiRenderEvent);
    guis.push_back(rdrGui);
    guimap[rdrGui->getName()] = rdrGui;
}

void UIProject::setupLightingGui(){
    lgtGui = new ofxUISuperCanvas("LIGHT", gui);
    lgtGui->copyCanvasStyle(gui);
    lgtGui->copyCanvasProperties(gui);
    lgtGui->setName("LightSettings");
    lgtGui->setPosition(guis[guis.size()-1]->getRect()->x+guis[guis.size()-1]->getRect()->getWidth()+1, 0);
    lgtGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    ofxUIToggle *toggle = lgtGui->addToggle("ENABLE", &bEnableLights);
    toggle->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
    lgtGui->resetPlacer();
    lgtGui->addWidgetDown(toggle, OFX_UI_ALIGN_RIGHT, true);
    lgtGui->addWidgetToHeader(toggle);
    
    lgtGui->addSpacer();
    lgtGui->addToggle("SMOOTH", &bSmoothLighting);
    lgtGui->addSpacer();
    float length = (lgtGui->getGlobalCanvasWidth()-lgtGui->getWidgetSpacing()*5)/3.;
    float dim = lgtGui->getGlobalSliderHeight();
    lgtGui->addLabel("GLOBAL AMBIENT COLOR", OFX_UI_FONT_SMALL);
    lgtGui->addMinimalSlider("R", 0.0, 1.0, &globalAmbientColor[0], length, dim)->setShowValue(false);
    lgtGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    lgtGui->addMinimalSlider("G", 0.0, 1.0, &globalAmbientColor[1], length, dim)->setShowValue(false);
    lgtGui->addMinimalSlider("B", 0.0, 1.0, &globalAmbientColor[2], length, dim)->setShowValue(false);
    lgtGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    lgtGui->autoSizeToFitWidgets();
    ofAddListener(lgtGui->newGUIEvent,this,&UIProject::guiLightingEvent);
    guis.push_back(lgtGui);
    guimap[lgtGui->getName()] = lgtGui;
}

void UIProject::guiLightingEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "R"){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);
    } else if(name == "G"){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);
    } else if(name == "B"){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);
    }
}


void UIProject::setupCameraGui(){
    camGui = new ofxUISuperCanvas("CAMERA", gui);
    camGui->copyCanvasStyle(gui);
    camGui->copyCanvasProperties(gui);
    camGui->setName("CamSettings");
    camGui->setPosition(guis[guis.size()-1]->getRect()->x+guis[guis.size()-1]->getRect()->getWidth()+1, 0);
    camGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    ofxUIButton *button = camGui->addButton("RESET", false);
    button->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
    camGui->resetPlacer();
    camGui->addWidgetDown(button, OFX_UI_ALIGN_RIGHT, true);
    camGui->addWidgetToHeader(button);
    camGui->addSpacer();
    camGui->addSlider("DIST", 0, 1000, &camDistance);
    camGui->addSlider("FOV", 0, 180, &camFOV);
    camGui->addSlider("ROT-X", 0, 360.0, xRot->getPosPtr())->setIncrement(1.0);
    camGui->addSlider("ROT-Y", 0, 360.0, yRot->getPosPtr())->setIncrement(1.0);
    camGui->addSlider("ROT-Z", 0, 360.0, zRot->getPosPtr())->setIncrement(1.0);
    
    camGui->addSpacer();
    vector<string> views;
    views.push_back("TOP");
    views.push_back("BOTTOM");
    views.push_back("FRONT");
    views.push_back("BACK");
    views.push_back("RIGHT");
    views.push_back("LEFT");
    views.push_back("3D");
    views.push_back("DISABLE");
    
    ofxUIDropDownList *ddl = camGui->addDropDownList("VIEW", views);
    ddl->setAutoClose(false);
    ddl->setShowCurrentSelected(true);
    ddl->activateToggle("DISABLE"); 
    
    camGui->autoSizeToFitWidgets();
    ofAddListener(camGui->newGUIEvent,this,&UIProject::guiCameraEvent);
    guis.push_back(camGui);
    guimap[camGui->getName()] = camGui;
}

void UIProject::guiCameraEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "DIST") {
        cam.setDistance(camDistance);
    } else if(name == "FOV"){
        cam.setFov(camFOV);
    } else if(name == "ROT-X"){
        xRot->setPosAndHome(xRot->getPos());
    } else if(name == "ROT-Y") {
        yRot->setPosAndHome(yRot->getPos());
    } else if(name == "ROT-Z"){
        zRot->setPosAndHome(zRot->getPos());
    } else if(name == "RESET"){
        ofxUIButton *b = (ofxUIButton *) e.widget;
        if(b->getValue()){
            cam.reset();
        }
    }else if(name == "TOP"){
        ofxUIToggle *t = (ofxUIToggle *) e.widget;
        if(t->getValue()){
            xRot->setHome(0);
            yRot->setHome(0);
            zRot->setHome(0);
        }
    } else if(name == "BOTTOM"){
        ofxUIToggle *t = (ofxUIToggle *) e.widget;
        if(t->getValue()){
            xRot->setHome(-180);
            yRot->setHome(0);
            zRot->setHome(0);
        }
    } else if(name == "BACK"){
        ofxUIToggle *t = (ofxUIToggle *) e.widget;
        if(t->getValue())
        {
            xRot->setHome(-90);
            yRot->setHome(0);
            zRot->setHome(180);
        }
    } else if(name == "FRONT"){
        ofxUIToggle *t = (ofxUIToggle *) e.widget;
        if(t->getValue()){
            xRot->setHome(-90);
            yRot->setHome(0);
            zRot->setHome(0);
        }
    } else if(name == "RIGHT"){
        ofxUIToggle *t = (ofxUIToggle *) e.widget;
        if(t->getValue())
        {
            xRot->setHome(-90);
            yRot->setHome(0);
            zRot->setHome(90);
        }
    } else if(name == "LEFT") {
        ofxUIToggle *t = (ofxUIToggle *) e.widget;
        if(t->getValue()){
            xRot->setHome(-90);
            yRot->setHome(0);
            zRot->setHome(-90);
            
        }
    } else if(name == "3D") {
        ofxUIToggle *t = (ofxUIToggle *) e.widget;
        if(t->getValue())
        {
            xRot->setHome(-70);
            yRot->setHome(0);
            zRot->setHome(45);
        }
    }
}

void UIProject::setupPresetGui(){
	presetGui = new ofxUISuperCanvas("PRESETS");
    presetGui->setTriggerWidgetsUponLoad(false);
	presetGui->setName("Presets");
	presetGui->copyCanvasStyle(gui);
    presetGui->copyCanvasProperties(gui);
    presetGui->addSpacer();
    
    vector<string> empty; empty.clear();
	presetRadio = presetGui->addRadio("PRESETS", empty);
	
	presetGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    vector<string> presets = getPresets();
    for(vector<string>::iterator it = presets.begin(); it != presets.end(); ++it){
        ofxUIToggle *t = presetGui->addToggle((*it), false);
        presetRadio->addToggle(t);
    }
	
	presetGui->autoSizeToFitWidgets();
    ofAddListener(presetGui->newGUIEvent,this,&UIProject::guiPresetEvent);
    guis.push_back(presetGui);
    guimap[presetGui->getName()] = presetGui;
}

void UIProject::guiPresetEvent(ofxUIEventArgs &e){
    ofxUIToggle *t = (ofxUIToggle *) e.widget;
    if(t->getValue()){
        loadPresetGUISFromName(e.widget->getName());
    }
}

void UIProject::setupMaterial(string name, Material *m){
    materials[name] = m;
    ofxUISuperCanvas* g = new ofxUISuperCanvas(name, gui);
    materialGuis[name] = g;
    g->copyCanvasStyle(gui);
    g->copyCanvasProperties(gui);
    g->setName(name+"Settings");
    g->addSpacer();
    g->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    float length = (g->getGlobalCanvasWidth()-g->getWidgetSpacing()*5)/3.;
    float dim = g->getGlobalSliderHeight();
    
    g->addLabel("AMBIENT", OFX_UI_FONT_SMALL);
    g->addMinimalSlider("AR", 0.0, 1.0, &m->getAmbientColor().r, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    g->addMinimalSlider("AG", 0.0, 1.0, &m->getAmbientColor().g, length, dim)->setShowValue(false);
    g->addMinimalSlider("AB", 0.0, 1.0, &m->getAmbientColor().b, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    g->addSpacer();
    
    g->addLabel("DIFFUSE", OFX_UI_FONT_SMALL);
    g->addMinimalSlider("AR", 0.0, 1.0, &m->getDiffuseColor().r, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    g->addMinimalSlider("AG", 0.0, 1.0, &m->getDiffuseColor().g, length, dim)->setShowValue(false);
    g->addMinimalSlider("AB", 0.0, 1.0, &m->getDiffuseColor().b, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    g->addSpacer();
    
    g->addLabel("EMISSIVE", OFX_UI_FONT_SMALL);
    g->addMinimalSlider("ER", 0.0, 1.0, &m->getEmissiveColor().r, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    g->addMinimalSlider("EG", 0.0, 1.0, &m->getEmissiveColor().g, length, dim)->setShowValue(false);
    g->addMinimalSlider("EB", 0.0, 1.0, &m->getEmissiveColor().b, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    g->addSpacer();
    
    g->addLabel("SPECULAR", OFX_UI_FONT_SMALL);
    g->addMinimalSlider("SR", 0.0, 1.0, &m->getSpecularColor().r, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    g->addMinimalSlider("SG", 0.0, 1.0, &m->getSpecularColor().g, length, dim)->setShowValue(false);
    g->addMinimalSlider("SB", 0.0, 1.0, &m->getSpecularColor().b, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    g->addSpacer();
    
    g->addMinimalSlider("SHINY", 0.0, 128.0, &(m->getShininess()))->setShowValue(false);
    
    g->autoSizeToFitWidgets();
    g->setPosition(ofGetWidth()*.5-g->getRect()->getHalfWidth(), ofGetHeight()*.5 - g->getRect()->getHalfHeight());
    
    ofAddListener(g->newGUIEvent,this,&UIProject::guiMaterialEvent);
    guis.push_back(g);
    guimap[g->getName()] = g;
}

void UIProject::guiMaterialEvent(ofxUIEventArgs &e){
    
}

void UIProject::setupPointLight(string name){
    Light *l = new Light();
    l->light.setPointLight();
	//removes light until we are active
	l->light.destroy();
	
    lights[name] = l;
    
    ofxUISuperCanvas* g = new ofxUISuperCanvas(name, gui);
    lightGuis[name] = g;
    g->copyCanvasStyle(gui);
    g->copyCanvasProperties(gui);
    g->setName(name+"Settings");
    g->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    ofxUIToggle *toggle = g->addToggle("ENABLE", &l->bEnabled);
    toggle->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
    g->resetPlacer();
    g->addWidgetDown(toggle, OFX_UI_ALIGN_RIGHT, true);
    g->addWidgetToHeader(toggle);
    g->addSpacer();
    
    setupGenericLightProperties(g, l);
    
    g->autoSizeToFitWidgets();
    g->setPosition(ofGetWidth()*.5-g->getRect()->getHalfWidth(), ofGetHeight()*.5 - g->getRect()->getHalfHeight());
    
    ofAddListener(g->newGUIEvent,this,&UIProject::guiLightEvent);
    guis.push_back(g);
    guimap[g->getName()] = g;
}

void UIProject::setupSpotLight(string name){
    Light *l = new Light();
    l->light.setSpotlight();
	l->light.destroy();
	
    lights[name] = l;
    
    ofxUISuperCanvas* g = new ofxUISuperCanvas(name, gui);
    lightGuis[name] = g;
    g->copyCanvasStyle(gui);
    g->copyCanvasProperties(gui);
    g->setName(name+"Settings");
    g->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    ofxUIToggle *toggle = g->addToggle("ENABLE", &l->bEnabled);
    toggle->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
    g->resetPlacer();
    g->addWidgetDown(toggle, OFX_UI_ALIGN_RIGHT, true);
    g->addWidgetToHeader(toggle);
    g->addSpacer();
    
    g->addSlider("CUT OFF", 0, 90, &l->lightSpotCutOff);
    g->addSlider("EXPONENT", 0.0, 128.0, &l->lightExponent);
    g->addSpacer();
    
    setupGenericLightProperties(g, l);
    
    g->autoSizeToFitWidgets();
    g->setPosition(ofGetWidth()*.5-g->getRect()->getHalfWidth(), ofGetHeight()*.5 - g->getRect()->getHalfHeight());
    
    ofAddListener(g->newGUIEvent,this,&UIProject::guiLightEvent);
    guis.push_back(g);
    guimap[g->getName()] = g;
}

void UIProject::setupBeamLight(string name){
    Light *l = new Light();
    l->light.setDirectional();
	l->light.destroy();
	
    lights[name] = l;
    
    ofxUISuperCanvas* g = new ofxUISuperCanvas(name, gui);
    lightGuis[name] = g;
    g->copyCanvasStyle(gui);
    g->copyCanvasProperties(gui);
    g->setName(name+"Settings");
    g->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    ofxUIToggle *toggle = g->addToggle("ENABLE", &l->bEnabled);
    toggle->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
    g->resetPlacer();
    g->addWidgetDown(toggle, OFX_UI_ALIGN_RIGHT, true);
    g->addWidgetToHeader(toggle);
    g->addSpacer();
    
    setupGenericLightProperties(g, l);
    
    g->autoSizeToFitWidgets();
    g->setPosition(ofGetWidth()*.5-g->getRect()->getHalfWidth(), ofGetHeight()*.5 - g->getRect()->getHalfHeight());
    
    ofAddListener(g->newGUIEvent,this,&UIProject::guiLightEvent);
    guis.push_back(g);
    guimap[g->getName()] = g;
}

void UIProject::setupGenericLightProperties(ofxUISuperCanvas *g, Light *l){
    float length = (g->getGlobalCanvasWidth()-g->getWidgetSpacing()*5)/3.;
    float dim = g->getGlobalSliderHeight();
    
    switch(l->light.getType()){
        case OF_LIGHT_POINT:{
            g->addLabel("POSITION", OFX_UI_FONT_SMALL);
            g->addMinimalSlider("X", -1000.0, 1000.0, &l->lightPos.x, length, dim)->setShowValue(false);
            g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            g->addMinimalSlider("Y", -1000.0, 1000.0, &l->lightPos.y, length, dim)->setShowValue(false);
            g->addMinimalSlider("Z", -1000.0, 1000.0, &l->lightPos.z, length, dim)->setShowValue(false);
            g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            g->addSpacer();
        }
            break;
            
        case OF_LIGHT_SPOT:{
            g->addLabel("POSITION", OFX_UI_FONT_SMALL);
            g->addMinimalSlider("X", -1000.0, 1000.0, &l->lightPos.x, length, dim)->setShowValue(false);
            g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            g->addMinimalSlider("Y", -1000.0, 1000.0, &l->lightPos.y, length, dim)->setShowValue(false);
            g->addMinimalSlider("Z", -1000.0, 1000.0, &l->lightPos.z, length, dim)->setShowValue(false);
            g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            g->addSpacer();
            
            g->addLabel("ORIENTATION", OFX_UI_FONT_SMALL);
            g->addMinimalSlider("OX", -90.0, 90.0, &l->lightOrientation.x, length, dim)->setShowValue(false);
            g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            g->addMinimalSlider("OY", -90.0, 90.0, &l->lightOrientation.y, length, dim)->setShowValue(false);
            g->addMinimalSlider("OZ", -90.0, 90.0, &l->lightOrientation.z, length, dim)->setShowValue(false);
            g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            g->addSpacer();
        }
            break;
            
        case OF_LIGHT_DIRECTIONAL:{
            g->addLabel("ORIENTATION", OFX_UI_FONT_SMALL);
            g->addMinimalSlider("OX", -90.0, 90.0, &l->lightOrientation.x, length, dim)->setShowValue(false);
            g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            g->addMinimalSlider("OY", -90.0, 90.0, &l->lightOrientation.y, length, dim)->setShowValue(false);
            g->addMinimalSlider("OZ", -90.0, 90.0, &l->lightOrientation.z, length, dim)->setShowValue(false);
            g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            g->addSpacer();
        }
            break;
    }
    
    g->addLabel("AMBIENT", OFX_UI_FONT_SMALL);
    g->addMinimalSlider("AR", 0.0, 1.0, &l->lightAmbient.r, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    g->addMinimalSlider("AG", 0.0, 1.0, &l->lightAmbient.g, length, dim)->setShowValue(false);
    g->addMinimalSlider("AB", 0.0, 1.0, &l->lightAmbient.b, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    g->addSpacer();
    g->addLabel("DIFFUSE", OFX_UI_FONT_SMALL);
    g->addMinimalSlider("DR", 0.0, 1.0, &l->lightDiffuse.r, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    g->addMinimalSlider("DG", 0.0, 1.0, &l->lightDiffuse.g, length, dim)->setShowValue(false);
    g->addMinimalSlider("DB", 0.0, 1.0, &l->lightDiffuse.b, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    g->addSpacer();
    g->addLabel("SPECULAR", OFX_UI_FONT_SMALL);
    g->addMinimalSlider("SR", 0.0, 1.0, &l->lightSpecular.r, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    g->addMinimalSlider("SG", 0.0, 1.0, &l->lightSpecular.g, length, dim)->setShowValue(false);
    g->addMinimalSlider("SB", 0.0, 1.0, &l->lightSpecular.b, length, dim)->setShowValue(false);
    g->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    g->addSpacer();
}

void UIProject::guiLightEvent(ofxUIEventArgs &e){
    
}

void UIProject::guiAllEvents(ofxUIEventArgs &e){

}

void UIProject::lightsBegin(){
    ofSetSmoothLighting(bSmoothLighting);
    if(bEnableLights){
        for(map<string, Light *>::iterator it = lights.begin(); it != lights.end(); ++it){
            ofEnableLighting();
            it->second->enable();
        }
    }
}

void UIProject::lightsEnd(){
    if(!bEnableLights){
        ofDisableLighting();
        for(map<string, Light *>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->disable();
        }
    }
}

void UIProject::loadGUIS(){
    for(int i = 0; i < guis.size(); i++){
        guis[i]->loadSettings(getDataPath()+"Presets/Working/"+guis[i]->getName()+".xml");
    }
    cam.reset();
    ofxLoadCamera(cam, getDataPath()+"Presets/Working/"+"ofEasyCamSettings");
}

void UIProject::saveGUIS(){
    for(int i = 0; i < guis.size(); i++){
        guis[i]->saveSettings(getDataPath()+"Presets/Working/"+guis[i]->getName()+".xml");
    }
    ofxSaveCamera(cam, getDataPath()+"Presets/Working/"+"ofEasyCamSettings");
}

void UIProject::loadPresetGUISFromName(string presetName){
	loadPresetGUISFromPath(getDataPath()+"Presets/"+ presetName);
}

void UIProject::loadPresetGUISFromPath(string presetPath){
	
    for(int i = 0; i < guis.size(); i++){
        guis[i]->loadSettings(presetPath+"/"+guis[i]->getName()+".xml");
    }
    cam.reset();
    ofxLoadCamera(cam, presetPath+"/ofEasyCamSettings");
	
	selfPresetLoaded(presetPath);
}

void UIProject::savePresetGUIS(string presetName){
    ofDirectory dir;
    string presetDirectory = getDataPath()+"Presets/"+presetName+"/";
    if(!dir.doesDirectoryExist(presetDirectory)){
        dir.createDirectory(presetDirectory);
        presetRadio->addToggle(presetGui->addToggle(presetName, true));
        presetGui->autoSizeToFitWidgets();
    }
    
    for(int i = 0; i < guis.size(); i++){
        guis[i]->saveSettings(presetDirectory+guis[i]->getName()+".xml");
    }
    ofxSaveCamera(cam, getDataPath()+"Presets/"+presetName+"/ofEasyCamSettings");

    cam.enableMouseInput();
}

void UIProject::deleteGUIS(){
    for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it){
        ofxUICanvas *g = (*it);
        delete g;
        *it = NULL;
    }
    guis.clear();
}

void UIProject::showGUIS(){
    for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->enable();
    }
}

void UIProject::hideGUIS(){
    for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->disable();
    }
}

void UIProject::toggleGUIS(){
    for(vector<ofxUISuperCanvas *>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->toggleVisible();
    }
}

void UIProject::toggleGuiAndPosition(ofxUISuperCanvas *g){
    if(g->isMinified()){
        g->setMinified(false);
        g->setPosition(ofGetMouseX(), ofGetMouseY());
    } else {
        g->setMinified(true);
    }
}

ofCamera& UIProject::getCameraRef(){
	return cam;
}

ofFbo& UIProject::getRenderTarget(){
    
    if(!renderTarget.isAllocated() || renderTarget.getWidth() != ofGetWidth() || renderTarget.getHeight() != ofGetHeight()){
        
        renderTarget.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
		renderTarget.begin();
		ofClear(0,0,0,0);
		renderTarget.end();
        
    }
    
    return renderTarget;
}

void UIProject::billBoard(ofVec3f globalCamPosition, ofVec3f globelObjectPosition){
    ofVec3f objectLookAt = ofVec3f(0,0,1);
    ofVec3f objToCam = globalCamPosition - globelObjectPosition;
    objToCam.normalize();
    float theta = objectLookAt.angle(objToCam);
    ofVec3f axisOfRotation = objToCam.crossed(objectLookAt);
    axisOfRotation.normalize();
    glRotatef(-theta, axisOfRotation.x, axisOfRotation.y, axisOfRotation.z);
}

void UIProject::drawTexturedQuad(){
    glBegin (GL_QUADS);
    glTexCoord2f (0.0, 0.0);
    glVertex3f (0.0, 0.0, 0.0);
    glTexCoord2f (ofGetWidth(), 0.0);
    glVertex3f (ofGetWidth(), 0.0, 0.0);
    glTexCoord2f (ofGetWidth(), ofGetHeight());
    glVertex3f (ofGetWidth(), ofGetHeight(), 0.0);
    glTexCoord2f (0.0, ofGetHeight());
    glVertex3f (0.0, ofGetHeight(), 0.0);
    glEnd ();
}

void UIProject::drawNormalizedTexturedQuad(){
    glBegin (GL_QUADS);
    glTexCoord2f (0.0, 0.0);
    glVertex3f (0.0, 0.0, 0.0);
    glTexCoord2f (1.0, 0.0);
    glVertex3f (ofGetWidth(), 0.0, 0.0);
    glTexCoord2f (1.0, 1.0);
    glVertex3f (ofGetWidth(), ofGetHeight(), 0.0);
    glTexCoord2f (0.0, 1.0);
    glVertex3f (0.0, ofGetHeight(), 0.0);
    glEnd ();
}

void UIProject::selfPostDraw(){
	UIProject::getRenderTarget().draw(0, 0,UIProject::getRenderTarget().getWidth(), UIProject::getRenderTarget().getHeight());
}

string UIProject::getDataPath(){
    string path = "../../../data/"+getSystemName()+"/";
    return path;
}

string UIProject::getSystemName(){
    return "Abstract";
}

void UIProject::addGuiClass(UIClass &_uiClass){
    ofxUISuperCanvas* uiClass = _uiClass.getUI();
	guis.push_back(uiClass);
	guimap[uiClass->getName()] = uiClass;
}