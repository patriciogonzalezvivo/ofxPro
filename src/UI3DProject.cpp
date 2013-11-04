//
//  UI3DProject.cpp
//
//  Created by Patricio Gonzalez Vivo on 10/15/13.
//
//

#include "UI3DProject.h"

void UI3DProject::play(){
    if (!bPlaying){
        camera.enableMouseInput();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->play();
        }
        selectedLigth = "NULL";
        UI2DProject::play();
    }
}

void UI3DProject::stop(){
    if(bPlaying){
        camera.disableMouseInput();
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
        
        UI2DProject::update(args);
    }
}

void UI3DProject::draw(ofEventArgs & args){
    
    
    if(bRenderSystem){
        ofPushStyle();
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
        ofDisableLighting();
        selfPostDraw();
#endif
        if(bRecording){
            ofPushStyle();
            ofFill();
            ofSetColor(255, 0, 0,abs(sin(ofGetElapsedTimef()))*255);
            ofCircle(ofGetWidth()-20, 20, 5);
            ofPopStyle();
        }
        ofPopStyle();
	}
    
}

void UI3DProject::exit(ofEventArgs & args){
    if(bRecording){
        recordingEnd();
    }
    
    guiSave();
    
    extruders.clear();
    lights.clear();
    materials.clear();
    
    guis.clear();
    selfExit();
}

//-------------------- Mouse events + camera interaction
//
void UI3DProject::mousePressed(ofMouseEventArgs & args){
	if(bGui && cursorIsOverGUI()){
		camera.disableMouseInput();
	} else if(bDebug && cursorIsOverLight() != "NULL"){
        camera.disableMouseInput();
        selectedLigth = cursorIsOverLight();
    } else {
        selfMousePressed(args);
    }
}

void UI3DProject::mouseDragged(ofMouseEventArgs & args){
    if(bDebug && selectedLigth != "NULL"){
        
        ofPoint pmouse(ofGetPreviousMouseX(),-ofGetPreviousMouseY());
        ofPoint mouse(ofGetMouseX(),-ofGetMouseY());
        
        ofPoint diff = camera.cameraToWorld(mouse)-camera.cameraToWorld(pmouse);
    
        *lights[selectedLigth]+=diff*0.1;
        
    } else {
        UI2DProject::mouseDragged(args);
    }
};

void UI3DProject::mouseReleased(ofMouseEventArgs & args){
    camera.enableMouseInput();
    selfMouseReleased(args);
    selectedLigth = "NULL";
}

//------------------------------------------------------------ CORE SETUP

void UI3DProject::setupCoreGuis(){
    UI2DProject::setupCoreGuis();
    
    //  This is the specific of the 3D project
    //
    setupCameraParams();
    setupCameraGui();
    
    setupLightingParams();
    setupLightingGui();
    
    lightAdd("POINT LIGHT 1", OF_LIGHT_POINT);
    materialAdd( "MATERIAL" );
}

void UI3DProject::setupBackground(){
    background = new UIBackground();
    background->linkUIs( &guis );
    
    //  This is the specific of the 3D Project
    //
    background->linkCamera( &camera );
    
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

void UI3DProject::lightAdd( string _name, ofLightType _type ){
    UILightReference newLight(new UILight(_name, _type));
    lights[_name] = newLight;
	guis.push_back(newLight->getUIReference(guiTemplate));
}

string UI3DProject::cursorIsOverLight(){
    if(bEnableLights){
        ofPoint mouse(ofGetMouseX(),ofGetMouseY());
        
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            ofPoint pos3D( it->second->x, it->second->y, it->second->z);
            ofPoint pos2D = camera.worldToScreen(pos3D);
            
            if ( pos2D.distance(mouse) < 50 ){
                return it->first;
            }
        }
    }
    
    return "NULL";
}

void UI3DProject::lightsBegin(){
    ofSetSmoothLighting(bSmoothLighting);
    if(bEnableLights){
        ofEnableLighting();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
//            ofEnableLighting();
            it->second->enable();
        }
    }
}

void UI3DProject::lightsEnd(){
    if(!bEnableLights){
//        ofDisableLighting();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->disable();
        }
        ofDisableLighting();
    }
}

void UI3DProject::lightsDraw(){
    if(bEnableLights){
        ofDisableLighting();
        string overLight = cursorIsOverLight();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->draw();
            if( overLight == it->first){
                
                ofPushMatrix();
                ofPopStyle();
                ofNoFill();
                float pulse = abs(sin(ofGetElapsedTimef()));
                ofColor color = it->second->getColor();
                color.setBrightness(255-background->getColor2().getBrightness());
                ofSetColor( color, pulse*255);
                ofTranslate( it->second->getPosition() );
                float size = it->second->getPosition().distance(camera.getPosition())*0.1;
                cameraBillboard();
                ofEllipse(0,0, size, size);
                ofPopStyle();
                ofPopMatrix();
            }
        }
    }
}

void UI3DProject::materialAdd( string _name ){
    UIMaterialReference newMaterial( new UIMaterial() );
    
    if ( newMaterial->getClassName()  == "MATERIAL" ){
        newMaterial->setName("MATERIAL " + ofToString( materials.size() + 1));
    }
    
    materials[ newMaterial->getClassName() ] = newMaterial;
    guis.push_back( newMaterial->getUIReference(guiTemplate) );
}

//------------------------------------------------------ Save & Load + Camera

void UI3DProject::guiLoad(){
    UI2DProject::guiLoad();
    
    camera.reset();
    cameraLoad(getDataPath()+"Presets/Working/"+"ofEasyCamSettings");
}

void UI3DProject::guiSave(){
    UI2DProject::guiSave();
    cameraSave(getDataPath()+"Presets/Working/"+"ofEasyCamSettings");
}

void UI3DProject::guiLoadPresetFromPath(string presetPath){
    camera.reset();
    cameraLoad(presetPath+"/ofEasyCamSettings");

    UI2DProject::guiLoadPresetFromPath(presetPath);
}

void UI3DProject::guiSavePreset(string presetName){
    UI2DProject::guiSavePreset(presetName);
    
    cameraSave(getDataPath()+"Presets/"+presetName+"/ofEasyCamSettings");
    camera.enableMouseInput();
}

//----------------------------------------  CAMERA
void UI3DProject::setupCameraParams(){
    //CAMERA
    cameraFOV = 60;
    cameraDistance = 200;
    camera.setDistance(cameraDistance);
    camera.setFov(cameraFOV);
    
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
    camGui->addSlider("DIST", 0, 1000, &cameraDistance);
    camGui->addSlider("FOV", 0, 180, &cameraFOV);
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
        camera.setDistance(cameraDistance);
    } else if(name == "FOV"){
        camera.setFov(cameraFOV);
    } else if(name == "ROT-X"){
        xRot->setPosAndHome(xRot->getPos());
    } else if(name == "ROT-Y") {
        yRot->setPosAndHome(yRot->getPos());
    } else if(name == "ROT-Z"){
        zRot->setPosAndHome(zRot->getPos());
    } else if(name == "RESET"){
        ofxUIButton *b = (ofxUIButton *) e.widget;
        if(b->getValue()){
            camera.reset();
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

ofCamera& UI3DProject::getCameraRef(){
	return camera;
}

static bool saveOfCam(ofCamera &cam, string savePath){
    ofBuffer buffer;
    buffer.append("--------------ofCamera parameters--------------\n");
    buffer.append("transformMatrix\n" + ofToString(cam.getGlobalTransformMatrix()) + "\n" );
    buffer.append("fov\n" + ofToString(cam.getFov())+"\n");
    buffer.append("near\n" + ofToString(cam.getNearClip())+"\n");
    buffer.append("far\n" + ofToString(cam.getFarClip())+"\n");
    buffer.append("lensOffset\n" + ofToString(cam.getLensOffset())+"\n");
#ifndef USE_DEVELOP_BRANCH
    buffer.append("forceAspectRatio\n" + ofToString(cam.getForceAspectRatio())+"\n");
    buffer.append("aspectRatio\n" + ofToString(cam.getAspectRatio()) + "\n");
#endif
    buffer.append("isOrtho\n" + ofToString(cam.getOrtho()) + "\n");
    
    if(ofBufferToFile(savePath, buffer)){
        ofLogNotice("ofCamera saved successfully!");
        return true;
    }else{
        ofLogWarning("failed to save ofCamera!");
        return false;
    }
    
}

static bool loadOfCam(ofCamera &cam, string loadPath){
    ofFile file(loadPath);
	
	if(!file.exists()){
		ofLogError("The file " + loadPath + " is missing");
        return false;
	}
	float aRatio;
    bool bForceAspect =false;
	ofBuffer buffer(file);
	while (!buffer.isLastLine()) {
		string line = buffer.getNextLine();
        if (line == "transformMatrix") {
            string str =buffer.getNextLine() + "\n";
            str += buffer.getNextLine() + "\n";
            str += buffer.getNextLine() + "\n";
            str += buffer.getNextLine();
            
            ofMatrix4x4 m;
            istringstream iss;
            iss.str(str);
            iss >> m;
            cam.setTransformMatrix(m);
        }else if(line == "fov"){
            cam.setFov(ofToFloat(buffer.getNextLine()));
        }else if(line == "near"){
            cam.setNearClip(ofToFloat(buffer.getNextLine()));
        }else if(line == "far"){
            cam.setFarClip(ofToFloat(buffer.getNextLine()));
        }else if(line == "lensOffset"){
            vector<string> vals = ofSplitString(buffer.getNextLine(), ", ");
            if (vals.size()==2) {
                cam.setLensOffset(ofVec2f(ofToFloat(vals[0]), ofToFloat(vals[1])));
            }
#ifndef USE_DEVELOP_BRANCH
        }else if(line == "forceAspectRatio"){
            bForceAspect = ofToBool(buffer.getNextLine());
            cam.setForceAspectRatio(bForceAspect);
        }else if(line == "aspectRatio"){
#endif
            aRatio = ofToFloat(buffer.getNextLine());
        }else if(line == "isOrtho"){
            if(ofToBool(buffer.getNextLine())){
                cam.enableOrtho();
            }else{
                cam.disableOrtho();
            }
        }
	}
    if (bForceAspect){
        cam.setAspectRatio(aRatio);
    }
    
    return true;
    
}

bool UI3DProject::cameraSave(string savePath){
    
    if(saveOfCam((ofCamera &)camera, savePath)){
        ofBuffer buffer = ofBufferFromFile(savePath);
        
        buffer.append("--------------ofEasyCam parameters--------------\n");
        buffer.append("target\n" + ofToString(camera.getTarget().getPosition()) + "\n" );
        buffer.append("bEnableMouseMiddleButton\n" + ofToString(camera.getMouseMiddleButtonEnabled())+"\n");
        buffer.append("bMouseInputEnabled\n" + ofToString(camera.getMouseInputEnabled())+"\n");
        buffer.append("drag\n" + ofToString(camera.getDrag())+"\n");
        buffer.append("doTranslationKey\n" + ofToString(camera.getTranslationKey())+"\n");
        if(ofBufferToFile(savePath, buffer)){
            ofLogNotice("ofEasyCam saved successfully!");
            return true;
        }else{
            ofLogWarning("failed to save ofEasyCam!");
            return false;
        }
    }else{
        return false;
    }
}

bool UI3DProject::cameraLoad(string loadPath){
    if(loadOfCam((ofCamera &)camera, loadPath)){
        ofBuffer buffer = ofBufferFromFile(loadPath);
        while (!buffer.isLastLine()) {
            string line = buffer.getNextLine();
            
            if (line == "target") {
                vector<string> vals = ofSplitString(buffer.getNextLine(), ", ");
                if (vals.size()==3) {
                    camera.getTarget().setPosition(ofVec3f(ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2])));
                }
            }
            else if(line == "drag"){
                camera.setDrag(ofToFloat(buffer.getNextLine()));
            }else if(line == "bEnableMouseMiddleButton"){
                if(ofToBool(buffer.getNextLine())){
                    camera.enableMouseMiddleButton();
                }else{
                    camera.disableMouseMiddleButton();
                }
            }else if(line == "bMouseInputEnabled"){
                if(ofToBool(buffer.getNextLine())){
                    camera.enableMouseInput();
                }else{
                    camera.disableMouseInput();
                }
            }else if(line == "doTranslationKey"){
                camera.setTranslationKey(ofToChar(buffer.getNextLine()));
            }
        }
        return true;
    }else {
        return false;
    }
}

void UI3DProject::cameraBillboard(){
    ofVec3f objectLookAt = ofVec3f(0.0,0.0,1.0);
    ofVec3f objToCam = camera.getGlobalPosition();
    objToCam.normalize();
    float theta = objectLookAt.angle(objToCam);
    
    ofVec3f axisOfRotation = objToCam.crossed(objectLookAt);
    axisOfRotation.normalize();
    
    glRotatef(-zRot->getPos(), 0.0, 0.0, 1.0);
    glRotatef(-yRot->getPos(), 0.0, 1.0, 0.0);
    glRotatef(-xRot->getPos(), 1.0, 0.0, 0.0);
    glRotatef(-theta, axisOfRotation.x, axisOfRotation.y, axisOfRotation.z);
}