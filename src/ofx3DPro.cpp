//
//  ofx3DPro.cpp
//
//  Copyright (c) 2013 Patricio Gonzalez Vivo <http://patriciogonzalezvivo.com>
//
//

#include "ofx3DPro.h"

void ofx3DPro::play(){
    if (!bPlaying){
        
        if(camera->bEnable){
            camera->enableMouseInput();
        }
        
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->play();
        }
        selectedLigth = "NULL";
        ofx2DPro::play();
    }
}

void ofx3DPro::stop(){
    if(bPlaying){
        camera->disableMouseInput();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->stop();
        }
        ofx2DPro::stop();
    }
}

void ofx3DPro::draw(ofEventArgs & args){
    if(bRenderSystem){
        
        for(int viewNum=0;viewNum<numViewports;viewNum++){
            currentViewPort = viewNum;
            ofPushStyle();
#ifdef TARGET_RASPBERRY_PI
            
#else
            getRenderTarget(viewNum).begin();
#endif
            //  Background
            //
            if ( background != NULL ){
                background->draw();
            }
            
            //  2D scene
            //
            ofPushStyle();
            ofPushMatrix();
            selfDrawBackground();
            ofPopMatrix();
            ofPopStyle();
            
            //  Start 3D scene
            //
            {
                if(cameraEnabled){
                    getCameraRef().begin();
                }
                fog.begin();
                
                //  Scene Setup
                //
                selfSceneTransformation();
                
                //  Cached Values
                //
                glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix.getPtr());
                glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix.getPtr());
                glGetDoublev(GL_PROJECTION_MATRIX, matP);
                glGetDoublev(GL_MODELVIEW_MATRIX, matM);
                glGetIntegerv(GL_VIEWPORT, viewport);
                
                //            glEnable(GL_DEPTH_TEST);
                ofEnableDepthTest();
                //            glDepthMask(false);
                
                if (bEdit){
                    lightsDraw();
                }
                
                //  Draw Debug
                //
                if( bDebug ){
                    ofPushStyle();
                    ofPushMatrix();
                    
                    selfDrawDebug();
                    
                    ofPopMatrix();
                    ofPopStyle();
                }
                
                //  Draw Scene
                //
                {
                    lightsBegin();
                    ofPushStyle();
                    ofPushMatrix();
                    
                    selfDraw();
                    
                    ofPopMatrix();
                    ofPopStyle();
                    lightsEnd();
                }
                
                //  Draw Log
                //
                {
                    ofPushStyle();
                    ofDisableLighting();
                    ofSetColor(background->getUIBrightness()*255.0);
                    logGui.draw();
                    ofPopStyle();
                }
                
                //            glDepthMask(true);
                ofDisableDepthTest();
                //            glDisable(GL_DEPTH_TEST);
                fog.end();
                
                //  Update Mouse
                //
                if (bUpdateCursor){
                    unprojectCursor(cursor, ofGetMouseX(), ofGetMouseY());
                    bUpdateCursor = false;
                }
                
                //            if (bEdit){
                //                ofEnableDepthTest();
                //                ofPushStyle();
                //                ofSetColor(255);
                //                ofSphere(cursor.world, 10);
                //                ofPopStyle();
                //                ofDisableDepthTest();
                //            }
                
                if(cameraEnabled){
                    getCameraRef().end();
                }
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
            getRenderTarget(viewNum).end();
#endif
            
        }
        
#ifdef TARGET_RASPBERRY_PI
#else
        //  Post-Draw ( shader time )
        //
        ofDisableLighting();
        selfPostDraw();
#endif
        logGui.drawStatus();
        
        ofPopStyle();
	}
    
}

void ofx3DPro::exit(ofEventArgs & args){
    if(logGui.isRecording()){
        logGui.record(false);
    }
    
    guiSave();
    
    lights.clear();
    materials.clear();
    
    guis.clear();
    selfExit();
}

//-------------------- Mouse events + camera interaction
//
ofPoint ofx3DPro::unproject(ofPoint _screen){
    float depth;
    //read z value from depth buffer at mouse coords
    getRenderTarget().getDepthTexture().bind();
    glReadPixels(_screen.x, _screen.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    getRenderTarget().getDepthTexture().unbind();
    
    if (depth == 1.0f) {
        return ofPoint(0,0,0);
    } else {
        GLdouble c[3];
        gluUnProject(_screen.x, _screen.y, depth, matM, matP, viewport, c, c+1, c+2);
        return ofPoint(c[0], c[1],c[2]);
    }
}

void ofx3DPro::unprojectCursor(MovingCursor &_cursor, float _x, float _y){
    _cursor.lastFrame = _cursor;
    
    //consider that these should be in viewport space
    _cursor.screen.x = _x;
    _cursor.screen.y = _y;
    
    cursor.world = unproject(_cursor.screen);
    
    if(cursor.world.z == 1.0f){
        _cursor.worldValid = false;
    } else {
        _cursor.worldValid = true;
    }
    
    ////
    //worldViewFrameDifference
    ofVec3f screenDiffNorm = _cursor.getScreenFrameDifference() / ofVec2f(viewport[2], -viewport[3]);
    _cursor.worldViewFrameDifference = screenDiffNorm * viewMatrix.getInverse().getRotate();
    float distance = (camera->getCameraPtn()->getPosition() - _cursor.world).length();
    _cursor.worldViewFrameDifference *= distance * tan( camera->getCameraPtn()->getFov() * DEG_TO_RAD / 2.0f) * 2.0f * 2.0f;
    _cursor.worldViewFrameDifference.x *= ofGetWidth() / ofGetHeight();
    
    _cursor.lastUpdate = ofGetFrameNum();
}

void ofx3DPro::mouseMoved(ofMouseEventArgs & args){
    if (bEdit){
        bUpdateCursor = true;
    }
    
    ofx2DPro::mouseMoved(args);
};

void ofx3DPro::mousePressed(ofMouseEventArgs & args){
	if( cursorIsOverGUI() ){
		camera->disableMouseInput();
	}
    else if (logGui.isTakingNotes()){
        camera->disableMouseInput();
        logGui.penDown(ofPoint(args.x,args.y));
    }
    else if(bEdit && cursorIsOverLight() != "NULL"){
        camera->disableMouseInput();
        selectedLigth = cursorIsOverLight();
        
        if(ofGetElapsedTimef()-lastClick<doublClickThreshold){
            if(!bGui){
                guiShow();
            }
            
            for (int i = 0; i<guis.size(); i++) {
                if(guis[i]->getName()==selectedLigth){
                    
                    guis[i]->setMinified(false);
                    
                    guis[i]->getRect()->setX(1);
                    guis[i]->setPosition(args.x+25,
                                         args.y-guis[i]->getRect()->getHalfHeight());
                }
            }
        }
    }
    else {
        if(ofGetElapsedTimef()-lastClick<doublClickThreshold)
            selfMouseDoublePressed(args);
        else
            selfMousePressed(args);
    }
}

void ofx3DPro::mouseDragged(ofMouseEventArgs & args){
    if (bEdit){
        bUpdateCursor = true;
    }
    
    if (cursorIsOverGUI()){
        
    }
    else if (logGui.isTakingNotes()){
        logGui.penDown(ofPoint(args.x,args.y));
    }
    else if(bEdit && selectedLigth != "NULL"){
        if(cursor.worldValid){
            ofPoint pmouse(ofGetPreviousMouseX(),-ofGetPreviousMouseY());
            ofPoint mouse(ofGetMouseX(),-ofGetMouseY());
            ofPoint diff = getCameraRef().cameraToWorld(mouse)-getCameraRef().cameraToWorld(pmouse);
            *lights[selectedLigth]+=diff*0.1;//diff.normalize()*cursor.getWorldFrameDifference().length();
        }
    }
    else {
        ofx2DPro::mouseDragged(args);
    }
};

void ofx3DPro::mouseReleased(ofMouseEventArgs & args){
    
    if(camera->bEnable){
        camera->enableMouseInput();
    }
    
    if(logGui.isTakingNotes()){
        logGui.penUp();
    }
    
    selfMouseReleased(args);
    selectedLigth = "NULL";
    lastClick = ofGetElapsedTimef();
}

//------------------------------------------------------------ CORE SETUP

void ofx3DPro::setupCoreGuis(){
    setupGui();
    
    cameraSet(new UIEasyCamera());
    cameraEnable();
    setupNumViewports(1);
    
    logGui.linkDataPath(getDataPath());
    guiAdd(logGui);
    
    setupSystemGui();
    setupRenderGui();
    
    backgroundSet(new UIBackground());
    
    guiAdd(fog);
    materialAdd( "MATERIAL" );
    
    setupLightingGui();
    lightAdd("POINT LIGHT 1", OF_LIGHT_POINT);
}

void ofx3DPro::cameraSet(UICamera *_cam){
    if(camera != NULL){
        
        for(int i = 0; i<guis.size(); i++){
            if (guis[i]->getName() == "CAMERA" || guis[i]->getName() == "EASYCAM" || guis[i]->getName() == "GAMECAM"){
                guis.erase(guis.begin()+i);
                break;
            }
        }
        
        delete camera;
        camera = NULL;
    }
    
    camera = _cam;
    guiAdd(*_cam);
    camera->loadLocations(getDataPath()+"cameras/");
    logGui.linkCamera(camera);
}

void ofx3DPro::cameraEnable(bool enable){
    cameraEnabled = enable;
}

//------------------------------------------------------------ 3D SPECIFIC SETUP

void ofx3DPro::backgroundSet(UIBackground *_bg){
    ofx2DPro::backgroundSet(_bg);
    fog.linkColor(background);
}

void ofx3DPro::setupLightingGui(){
    bSmoothLighting = true;
    bEnableLights = true;
    globalAmbientColor = new float[4];
    globalAmbientColor[0] = 0.5;
    globalAmbientColor[1] = 0.5;
    globalAmbientColor[2] = 0.5;
    globalAmbientColor[3] = 1.0;
    
    UIReference tmp( new ofxUISuperCanvas("LIGHT", guiTemplate) );
    lightsGui = tmp;
    lightsGui->copyCanvasStyle(guiTemplate);
    lightsGui->copyCanvasProperties(guiTemplate);
    lightsGui->setName("LightSettings");
    lightsGui->setPosition(guis[guis.size()-1]->getRect()->x+guis[guis.size()-1]->getRect()->getWidth()+1, 0);
    lightsGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    ofxUIToggle *toggle = lightsGui->addToggle("ENABLE", &bEnableLights);
    toggle->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
    lightsGui->resetPlacer();
    lightsGui->addWidgetDown(toggle, OFX_UI_ALIGN_RIGHT, true);
    lightsGui->addWidgetToHeader(toggle);
    
    lightsGui->addSpacer();
    lightsGui->addToggle("SMOOTH", &bSmoothLighting);
    lightsGui->addSpacer();
    float length = (lightsGui->getGlobalCanvasWidth()-lightsGui->getWidgetSpacing()*5)/3.;
    float dim = lightsGui->getGlobalSliderHeight();
    lightsGui->addLabel("GLOBAL AMBIENT COLOR", OFX_UI_FONT_SMALL);
    lightsGui->addMinimalSlider("R", 0.0, 1.0, &globalAmbientColor[0], length, dim)->setShowValue(false);
    lightsGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    lightsGui->addMinimalSlider("G", 0.0, 1.0, &globalAmbientColor[1], length, dim)->setShowValue(false);
    lightsGui->addMinimalSlider("B", 0.0, 1.0, &globalAmbientColor[2], length, dim)->setShowValue(false);
    lightsGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    lightsGui->autoSizeToFitWidgets();
    ofAddListener(lightsGui->newGUIEvent,this,&ofx3DPro::guiLightingEvent);
    guis.push_back(lightsGui);
}

void ofx3DPro::guiLightingEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "R"){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);
    } else if(name == "G"){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);
    } else if(name == "B"){
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);
    }
}

void ofx3DPro::lightAdd( string _name, ofLightType _type ){
    UILightReference newLight(new UILight(_name, _type));
    lights[_name] = newLight;
	guis.push_back(newLight->getUIReference(guiTemplate));
}

string ofx3DPro::cursorIsOverLight(){
    if(bEnableLights){
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            if ( it->second->distance(cursor.world) < 10 ){
                return it->first;
            }
        }
    }
    
    return "NULL";
}

void ofx3DPro::lightsBegin(){
    if(bEnableLights){
        
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);
        ofSetSmoothLighting(bSmoothLighting);
        ofEnableLighting();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->enable();
        }
    }
}

void ofx3DPro::lightsEnd(){
    if(!bEnableLights){
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->disable();
        }
        ofDisableLighting();
    }
}

void ofx3DPro::lightsDraw(){
    if(bEnableLights){
        ofPushStyle();
        ofDisableLighting();
        string overLight = cursorIsOverLight();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->draw();
            if( overLight == it->first){
                
                ofPushMatrix();
                ofPushStyle();
                ofNoFill();
                float pulse = abs(sin(ofGetElapsedTimef()));
                ofColor color = it->second->getColor();
                color.setBrightness(background->getUIBrightness()*255);
                ofSetColor( color, pulse*255);
                ofTranslate( it->second->getPosition() );
                float size = it->second->getPosition().distance(getCameraRef().getPosition())*0.1;
                camera->billboard();
                ofSetLineWidth(2);
                ofEllipse(0,0, size, size);
                ofPopStyle();
                ofPopMatrix();
            }
        }
        ofPopStyle();
    }
}

void ofx3DPro::materialAdd( string _name ){
    UIMaterialReference newMaterial( new UIMaterial() );
    
    if ( _name == "MATERIAL" ){
        _name = "MATERIAL " + ofToString( materials.size() + 1);
    }
    
    newMaterial->setName(_name);
    materials[ _name ] = newMaterial;
    guis.push_back( newMaterial->getUIReference(guiTemplate) );
}

//------------------------------------------------------ Save & Load + CAMERA

void ofx3DPro::guiLoad(){
    ofx2DPro::guiLoad();
    camera->load(getDataPath()+"Presets/Working/"+"current.cam");
}

void ofx3DPro::guiSave(){
    ofx2DPro::guiSave();
    camera->save(getDataPath()+"Presets/Working/"+"current.cam");
}

void ofx3DPro::guiLoadPresetFromPath(string presetPath){
    cout << "PRESET PATH: " << presetPath << endl;
    ofx2DPro::guiLoadPresetFromPath(presetPath);
    camera->load(presetPath+"/current.cam");
    ofx2DPro::guiLoadPresetFromPath(presetPath);
}

void ofx3DPro::guiSavePreset(string presetName){
    ofx2DPro::guiSavePreset(presetName);

    camera->save(getDataPath()+"Presets/"+presetName+"/current.cam");
    
    if(camera->bEnable){
        camera->enableMouseInput();
    }
}

ofCamera& ofx3DPro::getCameraRef(){
	return (*camera->getCameraPtn());
}