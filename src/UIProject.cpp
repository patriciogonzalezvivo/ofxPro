
#include "UIProject.h"

string UIProject::getSystemName(){
    return "Abstract";
}

string UIProject::getDataPath(){
    #ifdef TARGET_OSX
    string path = "../../../data/"+getSystemName()+"/";
    #else
    string path = getSystemName()+"/";
    #endif
    return path;
}

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
    for(map<string, UILight *>::iterator it = lights.begin(); it != lights.end(); ++it){
        it->second->play();
    }

    selfBegin();

    bDebug = false;
}

void UIProject::stop(){

    hideGUIS();
    saveGUIS();
    cam.disableMouseInput();
    for(map<string, UILight *>::iterator it = lights.begin(); it != lights.end(); ++it){
        it->second->stop();
    }

    ofUnregisterMouseEvents(this);
    ofUnregisterKeyEvents(this);
    ofRemoveListener(ofEvents().update, this, &UIProject::update);
    ofRemoveListener(ofEvents().draw, this, &UIProject::draw);

    selfEnd();
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

    saveGUIS();
//    deleteGUIS();

    for(vector<ofx1DExtruder *>::iterator it = extruders.begin(); it != extruders.end(); ++it){
        ofx1DExtruder *e = (*it);
        delete e;
    }
    extruders.clear();

    for(map<string, UILight *>::iterator it = lights.begin(); it != lights.end(); ++it){
        UILight *l = it->second;
        delete l;
    }
    lights.clear();

    for(map<string, UIMaterial *>::iterator it = materials.begin(); it != materials.end(); ++it){
        UIMaterial *m = it->second;
        delete m;
    }
    materials.clear();
    materialGuis.clear();

    selfExit();


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

void UIProject::setupCoreGuis(){

    setupGui();
    setupSystemGui();

    setupRenderGui();

    setupLightingGui();

    setupCameraGui();

    setupPresetGui();

    setupBackground();

    mat = new UIMaterial();
    materialAdd( mat );

    lightAdd("POINT LIGHT 1", OF_LIGHT_POINT);
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

void UIProject::setupBackground(){
    background = new UIBackground();
    background->linkUIs( &guis );
    background->linkCamera( &cam );
    guiAdd( *background );
}

void UIProject::guiAdd(UIClass &_uiClass){
    ofxUISuperCanvas* uiClass = _uiClass.getUI(gui);
	guis.push_back(uiClass);
	guimap[uiClass->getName()] = uiClass;
}

void UIProject::lightAdd( string _name, ofLightType _type ){
    UILight *newLight = new UILight(_name, _type);

    lights[_name] = newLight;

    guiAdd( *newLight );
}

void UIProject::materialAdd( UIMaterial *_newMaterial ){

    if ( _newMaterial->getClassName() == "MATERIAL" ){
        _newMaterial->setName("MATERIAL " + ofToString( materials.size() + 1));
    }

    materials[ _newMaterial->getClassName() ] = _newMaterial;
    guiAdd( *_newMaterial );
}

void UIProject::guiAllEvents(ofxUIEventArgs &e){

}

void UIProject::lightsBegin(){
    ofSetSmoothLighting(bSmoothLighting);
    if(bEnableLights){
        for(map<string, UILight *>::iterator it = lights.begin(); it != lights.end(); ++it){
            ofEnableLighting();
            it->second->enable();
        }
    }
}

void UIProject::lightsEnd(){
    if(!bEnableLights){
        ofDisableLighting();
        for(map<string, UILight *>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->disable();
        }
    }
}

void UIProject::lightsDraw(){
    if(bEnableLights){
        ofDisableLighting();
        for(map<string, UILight *>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->draw();
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
        if (g != NULL ){
            delete g;
            *it = NULL;
        }
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

        ofFbo::Settings settings;
        settings.width = ofGetWidth();
        settings.height = ofGetHeight();
        settings.internalformat = GL_RGB;
        settings.numSamples = 0;
        settings.useDepth = true;
        settings.useStencil = true;
        settings.depthStencilAsTexture = true;
        settings.textureTarget = ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;

        renderTarget.allocate(settings);
//        renderTarget.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

		renderTarget.begin();
		ofClear(0,0,0,0);
		renderTarget.end();

    }

    return renderTarget;
}

void UIProject::selfPostDraw(){
	UIProject::getRenderTarget().draw(0, 0,UIProject::getRenderTarget().getWidth(), UIProject::getRenderTarget().getHeight());
}
