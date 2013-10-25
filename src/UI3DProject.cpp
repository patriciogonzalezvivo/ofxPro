//
//  UI3DProject.cpp
//
//  Created by Patricio Gonzalez Vivo on 10/15/13.
//
//

#include "UI3DProject.h"

void UI3DProject::setup(){
    
	cout << "SETTING UP SYSTEM " << getSystemName() << endl;
	background = NULL;
    
    ofDirectory dir;
    string directoryName = getDataPath()+"Presets/";
    if(!dir.doesDirectoryExist(directoryName)){
        dir.createDirectory(directoryName);
    }
    
    string workingDirectoryName = directoryName+"Working/";
    if(!dir.doesDirectoryExist(workingDirectoryName)){
        dir.createDirectory(workingDirectoryName);
    }
    
    ofSetSphereResolution(30);
    
    setupCameraParams();
	setupLightingParams();
    
    selfSetup();
    setupCoreGuis();
    selfSetupGuis();
    
    bPlaying = false;
}

void UI3DProject::play(){
    if (!bPlaying){
        cam.enableMouseInput();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->play();
        }
        UI2DProject::play();
    }
}

void UI3DProject::stop(){
    if(bPlaying){
        cam.disableMouseInput();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->stop();
        }
        UI2DProject::stop();
    }
}

void UI3DProject::update(ofEventArgs & args){
    if(bUpdateSystem){
        for(vector<ExtruderRef>::iterator it = extruders.begin(); it != extruders.end(); ++it){
            (*it)->update();
        }
        selfUpdate();
    }
}

void UI3DProject::draw(ofEventArgs & args){
    ofPushStyle();
    
    if(bRenderSystem){
        
        {
#ifdef TARGET_RASPBERRY_PI
            
#else
            getRenderTarget().begin();
#endif
            
            //  Background
            //
            if ( background != NULL ){
                background->draw();
            }
            
            
            //  2D scene
            //
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
                glDisable(GL_DEPTH_TEST);
                
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
#ifdef TARGET_RASPBERRY_PI
            
#else
            getRenderTarget().end();
            
            //  Post-Draw ( shader time )
            //
            selfPostDraw();
#endif
        }
        
        
	}
    ofPopStyle();
}

void UI3DProject::exit(ofEventArgs & args){
    saveGUIS();
    
    extruders.clear();
    lights.clear();
    materials.clear();
    
    guis.clear();
    selfExit();
}

void UI3DProject::keyPressed(ofKeyEventArgs & args){
    
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
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
			UIReference last;
            for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
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
            UIReference last;
            for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
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
            UIReference last;
            for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
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
            for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it)
            {
                (*it)->getRect()->setX(x+sin(theta)*tempRadius - (*it)->getRect()->getHalfWidth());
                (*it)->getRect()->setY(y+cos(theta)*tempRadius - (*it)->getRect()->getHalfHeight());
                theta +=stepSize;
            }
        }
            break;
            
        case '=':{
            for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
                (*it)->toggleMinified();
            }
        }
            break;
            
        default:
            selfKeyPressed(args);
            break;
    }
}

void UI3DProject::mousePressed(ofMouseEventArgs & args){
	if(cursorIsOverGUI()){
		cam.disableMouseInput();
	}
    selfMousePressed(args);
}

void UI3DProject::mouseReleased(ofMouseEventArgs & args){
    cam.enableMouseInput();
    selfMouseReleased(args);
}

//------------------------------------------------------------ CORE SETUP

void UI3DProject::setupCoreGuis(){
    setupGui();
    setupSystemGui();
    setupRenderGui();
    setupBackground();
    
    //  This is the specific of the 3D project
    //
    setupLightingGui();
    setupCameraGui();
    setupPresetGui();
    //setupBackground();
    materialAdd( "MATERIAL" );
    lightAdd("POINT LIGHT 1", OF_LIGHT_POINT);
}

void UI3DProject::setupBackground(){
    background = new UIBackground();
    background->linkUIs( &guis );
    
    //  This is the specific of the 3D Project
    //
    background->linkCamera( &cam );
    
    guiAdd( *background );
}

//------------------------------------------------------------ 3D SPECIFIC SETUP

void UI3DProject::setupLightingParams(){
    //LIGHTING
    bSmoothLighting = true;
    bEnableLights = true;
    globalAmbientColor = new float[4];
    globalAmbientColor[0] = 0.5;
    globalAmbientColor[1] = 0.5;
    globalAmbientColor[2] = 0.5;
    globalAmbientColor[3] = 1.0;
}
void UI3DProject::setupLightingGui(){
    UIReference tmp( new ofxUISuperCanvas("LIGHT", guiTemplate) );
    lgtGui = tmp;
    lgtGui->copyCanvasStyle(guiTemplate);
    lgtGui->copyCanvasProperties(guiTemplate);
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
    ofAddListener(lgtGui->newGUIEvent,this,&UI3DProject::guiLightingEvent);
    guis.push_back(lgtGui);
}

void UI3DProject::guiLightingEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "R"){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);
    } else if(name == "G"){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);
    } else if(name == "B"){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);
    }
}

void UI3DProject::setupCameraParams(){
    //CAMERA
    camFOV = 60;
    camDistance = 200;
    cam.setDistance(camDistance);
    cam.setFov(camFOV);
    
    ExtruderRef newXRot( new Extruder(0) );
    xRot = newXRot;
    ExtruderRef newYRot( new Extruder(0) );
    yRot = newYRot;
    ExtruderRef newZRot( new Extruder(0) );
    zRot = newZRot;
    
    xRot->setPhysics(.9, 5.0, 25.0);
    yRot->setPhysics(.9, 5.0, 25.0);
    zRot->setPhysics(.9, 5.0, 25.0);
    
    extruders.push_back(xRot);
    extruders.push_back(yRot);
    extruders.push_back(zRot);
}

void UI3DProject::setupCameraGui(){
    UIReference tmp( new ofxUISuperCanvas("CAMERA", guiTemplate) );
    
    camGui = tmp;
    camGui->copyCanvasStyle(guiTemplate);
    camGui->copyCanvasProperties(guiTemplate);
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
    ofAddListener(camGui->newGUIEvent,this,&UI3DProject::guiCameraEvent);
    guis.push_back(camGui);
}

void UI3DProject::guiCameraEvent(ofxUIEventArgs &e){
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

void UI3DProject::lightAdd( string _name, ofLightType _type ){
    UILightReference newLight(new UILight(_name, _type));
    lights[_name] = newLight;
	guis.push_back(newLight->getUIReference(guiTemplate));
}

void UI3DProject::materialAdd( string _name ){
    UIMaterialReference newMaterial( new UIMaterial() );
    
    if ( newMaterial->getClassName()  == "MATERIAL" ){
        newMaterial->setName("MATERIAL " + ofToString( materials.size() + 1));
    }
    
    materials[ newMaterial->getClassName() ] = newMaterial;
    guis.push_back( newMaterial->getUIReference(guiTemplate) );
}

void UI3DProject::lightsBegin(){
    ofSetSmoothLighting(bSmoothLighting);
    if(bEnableLights){
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            ofEnableLighting();
            it->second->enable();
        }
    }
}

void UI3DProject::lightsEnd(){
    if(!bEnableLights){
        ofDisableLighting();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->disable();
        }
    }
}

void UI3DProject::lightsDraw(){
    if(bEnableLights){
        ofDisableLighting();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->draw();
        }
    }
}

void UI3DProject::loadGUIS(){
    for(int i = 0; i < guis.size(); i++){
        guis[i]->loadSettings(getDataPath()+"Presets/Working/"+guis[i]->getName()+".xml");
    }
    cam.reset();
    ofxLoadCamera(cam, getDataPath()+"Presets/Working/"+"ofEasyCamSettings");
}

void UI3DProject::saveGUIS(){
    for(int i = 0; i < guis.size(); i++){
        guis[i]->saveSettings(getDataPath()+"Presets/Working/"+guis[i]->getName()+".xml");
    }
    ofxSaveCamera(cam, getDataPath()+"Presets/Working/"+"ofEasyCamSettings");
}

void UI3DProject::loadPresetGUISFromPath(string presetPath){
    
    for(int i = 0; i < guis.size(); i++){
        guis[i]->loadSettings(presetPath+"/"+guis[i]->getName()+".xml");
    }
    cam.reset();
    ofxLoadCamera(cam, presetPath+"/ofEasyCamSettings");
    
	selfPresetLoaded(presetPath);
}

void UI3DProject::savePresetGUIS(string presetName){
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

ofCamera& UI3DProject::getCameraRef(){
	return cam;
}