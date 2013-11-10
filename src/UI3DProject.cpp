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
        ofPushStyle();
        ofPushMatrix();
        selfDrawBackground();
        ofPopMatrix();
        ofPopStyle();
        
        //  Start 3D scene
        //
        {
            getCameraRef().begin();
            fog.begin();
            
            //  Scene Setup
            //
            selfSceneTransformation();
            glEnable(GL_DEPTH_TEST);
            
            if (bEdit){
                lightsDraw();
            }
            
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
                ofPushMatrix();
                
                selfDraw();
                
                ofPopMatrix();
                ofPopStyle();
                
                lightsEnd();
            }
            
            //  Draw Log
            //
            {
                ofDisableLighting();
                ofPushStyle();
                ofSetColor(background->getUIBrightness()*255.0);
                logGui.draw();
                ofPopStyle();
            }
            
            glDisable(GL_DEPTH_TEST);
            fog.end();
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
        if(logGui.isRecording()){
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
void UI3DProject::mousePressed(ofMouseEventArgs & args){
	if( cursorIsOverGUI() ){
		camera.disableMouseInput();
	}
    else if (logGui.isTakingNotes()){
        camera.disableMouseInput();
        logGui.penDown(ofPoint(args.x,args.y));
    }
    else if(bEdit && cursorIsOverLight() != "NULL"){
        camera.disableMouseInput();
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

void UI3DProject::mouseDragged(ofMouseEventArgs & args){
    if (cursorIsOverGUI()){
        
    }
    else if (logGui.isTakingNotes()){
        logGui.penDown(ofPoint(args.x,args.y));
    }
    else if(bEdit && selectedLigth != "NULL"){
        ofPoint pmouse(ofGetPreviousMouseX(),-ofGetPreviousMouseY());
        ofPoint mouse(ofGetMouseX(),-ofGetMouseY());
        ofPoint diff = getCameraRef().cameraToWorld(mouse)-getCameraRef().cameraToWorld(pmouse);
        *lights[selectedLigth]+=diff*0.1;
    }
    else {
        UI2DProject::mouseDragged(args);
    }
};

void UI3DProject::mouseReleased(ofMouseEventArgs & args){
    camera.enableMouseInput();
    
    if(logGui.isTakingNotes()){
        logGui.penUp();
    }
    
    selfMouseReleased(args);
    selectedLigth = "NULL";
    lastClick = ofGetElapsedTimef();
}

//------------------------------------------------------------ CORE SETUP

void UI3DProject::setupCoreGuis(){
    
    setupGui();
    
    logGui.linkDataPath(getDataPath());
    logGui.linkCamera(camera);
    guiAdd(logGui);
    
    setupSystemGui();
    setupRenderGui();
    
    backgroundSet(new UIBackground());
    
    guiAdd(camera);
    camera.loadLocations(getDataPath()+"cameras/");

    
    guiAdd(fog);
    materialAdd( "MATERIAL" );
    
    setupLightingGui();
    lightAdd("POINT LIGHT 1", OF_LIGHT_POINT);
}

//------------------------------------------------------------ 3D SPECIFIC SETUP

void UI3DProject::backgroundSet(UIBackground *_bg){
    UI2DProject::backgroundSet(_bg);
    fog.linkColor(background);
}

void UI3DProject::setupLightingGui(){
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
    ofAddListener(lightsGui->newGUIEvent,this,&UI3DProject::guiLightingEvent);
    guis.push_back(lightsGui);
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
            ofPoint pos2D = getCameraRef().worldToScreen(pos3D);
            
            if ( pos2D.distance(mouse) < 50 ){
                return it->first;
            }
        }
    }
    
    return "NULL";
}

void UI3DProject::lightsBegin(){
    if(bEnableLights){
        ofSetSmoothLighting(bSmoothLighting);
        ofEnableLighting();
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->enable();
        }
    }
}

void UI3DProject::lightsEnd(){
    if(!bEnableLights){
        for(map<string, UILightReference>::iterator it = lights.begin(); it != lights.end(); ++it){
            it->second->disable();
        }
        ofDisableLighting();
    }
}

void UI3DProject::lightsDraw(){
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
                camera.billboard();
                ofSetLineWidth(2);
                ofEllipse(0,0, size, size);
                ofPopStyle();
                ofPopMatrix();
            }
        }
        ofPopStyle();
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

//------------------------------------------------------ Save & Load + CAMERA

void UI3DProject::guiLoad(){
    UI2DProject::guiLoad();
    camera.load(getDataPath()+"Presets/Working/"+"current.cam");
}

void UI3DProject::guiSave(){
    UI2DProject::guiSave();
    camera.save(getDataPath()+"Presets/Working/"+"current.cam");
//    camera.save(getDataPath()+"cameras/default.cam");
}

void UI3DProject::guiLoadPresetFromPath(string presetPath){
    cout << "PRESET PATH: " << presetPath << endl;
    UI2DProject::guiLoadPresetFromPath(presetPath);
    camera.load(presetPath+"/current.cam");
    UI2DProject::guiLoadPresetFromPath(presetPath);
}

void UI3DProject::guiSavePreset(string presetName){
    UI2DProject::guiSavePreset(presetName);

    camera.save(getDataPath()+"Presets/"+presetName+"/current.cam");
//    camera.save(getDataPath()+"cameras/default.cam");
    camera.enableMouseInput();
}

ofCamera& UI3DProject::getCameraRef(){
	return (*camera.getCameraPtn());
}