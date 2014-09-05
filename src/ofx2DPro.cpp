//
//  ofx2DPro.cpp
//
//  Copyright (c) 2013 Patricio Gonzalez Vivo <http://patriciogonzalezvivo.com>
//
//

#include "ofx2DPro.h"

ofx2DPro::ofx2DPro():
bPlaying(false),
doublClickThreshold(0.2),
currentPresetName("Working"),
bDebug(false),
bRenderSystem(true),
bUpdateSystem(true)
{
    guiTemplate = new ofxUISuperCanvas(ofToUpper(getSystemName()));
    guiTemplate->setName("TEMPLATE");
    guiTemplate->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    setupNumViewports(1);
};

ofx2DPro::~ofx2DPro(){
};

string ofx2DPro::getDataPath(){
#ifdef TARGET_OSX
    
#ifdef PACKED_APP
    string path = "data/"+getSystemName()+"/";
#else
//    string path = "data/"+getSystemName()+"/";
    string path = "../../../data/"+getSystemName()+"/";
#endif
    
#else
    string path = getSystemName()+"/";
#endif
    return path;
}

void ofx2DPro::setup(){
	cout << "SETTING UP SYSTEM " << getSystemName() << endl;
#ifdef PACKED_APP
    ofSetDataPathRoot("data/");
#endif
    
    //  Create directories if they are not there
    //
    ofDirectory dir;
    string directories[3] = {getDataPath(), "Presets/","Working/"};
    for (int i = 0; i < 3; i++) {
        string path = "";
        for(int j = 0; j <= i; j++ ){
            path += directories[j];
        }
        if(!dir.doesDirectoryExist(path)){
            dir.createDirectory(path);
        }
    }
    
    selfSetup();
    setupCoreGuis();
    selfSetupGuis();
}

void ofx2DPro::play(){
    
    if (!bPlaying){
        selfBegin();
        
#ifdef TARGET_RASPBERRY_PI
        consoleListener.setup(this);
#endif
        ofRegisterMouseEvents(this);
        ofRegisterKeyEvents(this);
        ofAddListener(ofEvents().update,this,&ofx2DPro::update);
        ofAddListener(ofEvents().draw,this,&ofx2DPro::draw);
        ofAddListener(ofEvents().exit,this,&ofx2DPro::exit);
        ofAddListener(ofEvents().windowResized, this, &ofx2DPro::windowResized);
        
        guiLoad();
        guiHide();
        
        bRenderSystem = true;
        bUpdateSystem = true;
        bPlaying = true;
    }
}

void ofx2DPro::stop(){
    if (bPlaying){
        if(logGui.isRecording()){
            logGui.record(false);
        }
        
        guiHide();
        guiSave();
        
        ofUnregisterMouseEvents(this);
        ofUnregisterKeyEvents(this);
        ofRemoveListener(ofEvents().update, this, &ofx2DPro::update);
        ofRemoveListener(ofEvents().draw, this, &ofx2DPro::draw);
        ofRemoveListener(ofEvents().exit, this, &ofx2DPro::exit);
        ofRemoveListener(ofEvents().windowResized, this, &ofx2DPro::windowResized);
        
        selfEnd();
        
        bRenderSystem = false;
        bUpdateSystem = false;
        bPlaying = false;
    }
}

void ofx2DPro::update(ofEventArgs & args){
    if(bUpdateSystem){
        selfUpdate();
        if(logGui.isRecording()){
            logGui.recordAddFrame();
        }
    }
}

void ofx2DPro::draw(ofEventArgs & args){
    if(bRenderSystem){
        for(int i=0; i < renderTargets.size();i++){
            currentViewPort = i;
            ofPushStyle();
#ifndef TARGET_RASPBERRY_PI
            ofx2DPro::getRenderTarget(currentViewPort).begin();
#endif
            {
                //  Background
                //
                background->draw();
                
                //  Draw Scene
                //
                {
                    ofPushStyle();
                    ofPushMatrix();
                    selfDraw();
                    ofPopMatrix();
                    ofPopStyle();
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
                
            }
            
#ifndef TARGET_RASPBERRY_PI
            ofx2DPro::getRenderTarget(currentViewPort).end();
#endif
            ofPopStyle();
        }
        selfPostDraw();
        
        logGui.drawStatus();
	}
    
    
}

void ofx2DPro::setupNumViewports(int _num){    
    while(renderTargets.size() < _num){
        renderTargets.push_back(ofFbo());
    }
}

void ofx2DPro::setupRenderIsFlipped(bool _flipped){
    renderFlipped = _flipped;
}

void ofx2DPro::exit(ofEventArgs & args){
    if(logGui.isRecording()){
        logGui.record(false);
    }
    
    guiSave("Working");
    guis.clear();
    selfExit();
}

//------------------------------------------------------- KEYBOARD
void ofx2DPro::keyPressed(ofKeyEventArgs & args){
    
    if (bGui){
        for (auto &it : guis) {
            if(it->hasKeyboardFocus()){
                return;
            }
        }
        
        switch (args.key) {
            case '\\':
                guiArrange(0);
                return;
                break;
            case '1':
                guiArrange(1);
                return;
                break;
            case '2':
                guiArrange(2);
                return;
                break;
            case '3':
                guiArrange(3);
                return;
                break;
            case '4':
                guiArrange(4);
                return;
                break;
        }
    }
    
#ifdef TARGET_OSX
    if( getModifierSpecialPressed() ){
#else
    if( getModifierControlPressed() ){
#endif
        switch (args.key){
            case 's':
                guiSave(currentPresetName);
                logGui.screenShot(currentPresetName);
                break;
            case 'S':{
#ifdef TARGET_OSX
                string presetName = SystemTextBoxDialog("Save Preset As", currentPresetName);
#else
                string presetName = ofSystemTextBoxDialog("Save Preset As", currentPresetName);
#endif
                guiSave(presetName);
                currentPresetName = presetName;
            }
                break;
            case 'r':{
                logGui.record(!logGui.isRecording());
            }
                break;
            case 'f':
                ofToggleFullscreen();
                break;
            case 'e':
                bEdit = !bEdit;
                break;
            case 'd':
                bDebug = !bDebug;
                break;
        }
    } else {
        if(args.key == 'h'){
//#ifndef PACKED_APP
            guiToggle();
//#endif
        } else {
            selfKeyPressed(args);
        }
    }
}

void ofx2DPro::keyReleased(ofKeyEventArgs & args){
    switch (args.key){
        default:
            selfKeyReleased(args);
            break;
    }
}

//-------------------------------------------------------- MOUSE

bool ofx2DPro::cursorIsOverGUI(){
    if (bGui){
        for(auto &it: guis){
            if(it->isHit(ofGetMouseX(), ofGetMouseY())){
                return true;
            }
        }
    }
	return false;
}

void ofx2DPro::mouseMoved(ofMouseEventArgs& data){
    selfMouseMoved(data);
}

void ofx2DPro::mousePressed(ofMouseEventArgs & args){
    if(cursorIsOverGUI()){
        return;
    }
    
    if(ofGetElapsedTimef()-lastClick<doublClickThreshold)
        selfMouseDoublePressed(args);
    else
        selfMousePressed(args);
}

void ofx2DPro::mouseDragged(ofMouseEventArgs& args){
    if(cursorIsOverGUI())
        return;
    
    selfMouseDragged(args);
}

void ofx2DPro::mouseReleased(ofMouseEventArgs & args){
    if(cursorIsOverGUI())
        return;
    
    lastClick = ofGetElapsedTimef();
    selfMouseReleased(args);
}

//------------------------------------------------------------ SETUP

void ofx2DPro::setupCoreGuis(){
    setupGui();
    
    logGui.linkDataPath(getDataPath());
    logGui.linkRenderTarget(&getRenderTarget());
    
    guiAdd(logGui);
    
    setupSystemGui();
    setupRenderGui();
    
    backgroundSet(new UIBackground());
}

void ofx2DPro::setupGui(){
    UIReference tmp( new ofxUISuperCanvas(getSystemName(), guiTemplate) );
    gui = tmp;
    ofxUIFPS *fps = gui->addFPS();
    gui->resetPlacer();
    gui->addWidgetDown(fps, OFX_UI_ALIGN_RIGHT, true);
    gui->addWidgetToHeader(fps);
    gui->addSpacer();
    gui->addToggle("EDIT",&bEdit);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addToggle("DEBUG",&bDebug);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addButton("SAVE", false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addButton("LOAD", false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSpacer();
    
    gui->setTriggerWidgetsUponLoad(false);
    vector<string> empty;
    empty.clear();
	presetRadio = gui->addRadio("PRESETS", empty);
    
	gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    vector<string> presets = getPresets();
    for(vector<string>::iterator it = presets.begin(); it != presets.end(); ++it){
        ofxUIToggle *t = gui->addToggle((*it), false);
        presetRadio->addToggle(t);
    }
    
    gui->autoSizeToFitWidgets();
    
    ofAddListener(gui->newGUIEvent,this,&ofx2DPro::guiEvent);
    
    guis.push_back(gui);
}

void ofx2DPro::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();

    if(name == "SAVE"){
        ofxUIButton *b = (ofxUIButton *) e.widget;
        if(b->getValue()){
#ifdef TARGET_OSX
            string presetName = SystemTextBoxDialog("Save Preset As", currentPresetName);
#else
			string presetName = ofSystemTextBoxDialog("Save Preset As", currentPresetName);
#endif
            guiSave(presetName);
            currentPresetName = presetName;
        }
    } else if(name == "LOAD"){
        ofxUIButton *b = (ofxUIButton *) e.widget;
        if(b->getValue()){
            ofFileDialogResult result = ofSystemLoadDialog("Load Visual System Preset Folder", true, getDataPath()+"Presets/");
            if(result.bSuccess && result.fileName.length()){
                guiLoadFromPath(result.filePath);
            } else {
                guiLoad();
            }
        }
    } else if( name == "EDIT" ){
        
    } else if( name == "DEBUG" ){
        
    } else {
        ofxUIToggle *t = (ofxUIToggle *) e.widget;
        if(t != NULL){
            if(t->getValue()){
                guiLoad(name);
            }
        }
    }
    selfGuiEvent(e);
}

vector<string> ofx2DPro::getPresets(){
	vector<string> presets;
	string presetPath = getDataPath()+"Presets/";
	ofDirectory presetsFolder = ofDirectory(presetPath);
    
	if(presetsFolder.exists()){
		presetsFolder.listDir();
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

void ofx2DPro::setupSystemGui(){
    UIReference tmp( new ofxUISuperCanvas("SYSTEM", guiTemplate) );
    
    sysGui = tmp;
    sysGui->copyCanvasStyle( guiTemplate );
    sysGui->copyCanvasProperties( guiTemplate );
    sysGui->setPosition(guis[guis.size()-1]->getRect()->x+guis[guis.size()-1]->getRect()->getWidth()+1, 0);
    sysGui->setName("SystemSettings");
    sysGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    ofxUIToggle *toggle = sysGui->addToggle("ENABLE", &bUpdateSystem);
    toggle->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
    sysGui->resetPlacer();
    sysGui->addWidgetDown(toggle, OFX_UI_ALIGN_RIGHT, true);
    sysGui->addWidgetToHeader(toggle);
    sysGui->addSpacer();
    
    selfSetupSystemGui();
    sysGui->autoSizeToFitWidgets();
    ofAddListener(sysGui->newGUIEvent,this,&ofx2DPro::guiSystemEvent);
    guis.push_back(sysGui);
}

void ofx2DPro::setupRenderGui(){
    UIReference tmp( new ofxUISuperCanvas("RENDER", guiTemplate ) );
    rdrGui = tmp;
    rdrGui->copyCanvasStyle( guiTemplate );
    rdrGui->copyCanvasProperties( guiTemplate );
    rdrGui->setPosition(guis[guis.size()-1]->getRect()->x+guis[guis.size()-1]->getRect()->getWidth()+1, 0);
    rdrGui->setName("RenderSettings");
    rdrGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
    ofxUIToggle *toggle = rdrGui->addToggle("ENABLE", &bRenderSystem);
    toggle->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
    rdrGui->resetPlacer();
    rdrGui->addWidgetDown(toggle, OFX_UI_ALIGN_RIGHT, true);
    rdrGui->addWidgetToHeader(toggle);
    rdrGui->addSpacer();
    
    selfSetupRenderGui();
    
    rdrGui->autoSizeToFitWidgets();
    ofAddListener(rdrGui->newGUIEvent,this,&ofx2DPro::guiRenderEvent);
    guis.push_back(rdrGui);
}

void ofx2DPro::backgroundSet(UIBackground *_bg){
    if(background.get() != NULL){
        for(int i = 0; i< guis.size(); i++){
            if (guis[i]->getName() == "BACKGROUND"){
                guis.erase(guis.begin()+i);
                break;
            }
        }
    }
    
    background = UIBackgroundReference(_bg);
    background->linkUIs( &guis );
    guiAdd( *background );
}

void ofx2DPro::guiAdd(UIClass &_UIClass){
    UIReference UIClass = _UIClass.getUIReference(guiTemplate);
	guis.push_back(UIClass);
}

void ofx2DPro::guiAllEvents(ofxUIEventArgs &e){
    
}

// LOAD
//

void ofx2DPro::guiLoad(string presetName){
    guiLoadFromPath(getDataPath()+"Presets/"+presetName);
}
    
void ofx2DPro::guiLoadFromPath(string presetPath){
    for(int i = 0; i < guis.size(); i++){
        guis[i]->loadSettings(presetPath+"/"+guis[i]->getName()+".xml");
    }
    selfPresetLoaded(presetPath);
    currentPresetName = ofFilePath::getBaseName(presetPath);
}

//  SAVE
//

void ofx2DPro::guiSave(string presetName){
    
    cout << "Saving " << presetName << endl;    
    
    ofDirectory dir;
    string presetDirectory = getDataPath()+"Presets/"+presetName+"/";
    if(!dir.doesDirectoryExist(presetDirectory)){
        dir.createDirectory(presetDirectory);
        presetRadio->addToggle(gui->addToggle(presetName, true));
        gui->autoSizeToFitWidgets();
    }
    
    for(int i = 0; i < guis.size(); i++){
        guis[i]->saveSettings(presetDirectory+guis[i]->getName()+".xml");
    }
}

//  Others
//
void ofx2DPro::guiShow(){
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->enable();
    }
    bGui = true;
}

void ofx2DPro::guiHide(){
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->disable();
    }
    bGui = false;
}

void ofx2DPro::guiToggle(){
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->toggleVisible();
    }
    bGui = !bGui;
}

void ofx2DPro::guiArrange(int _type){
    if (_type == 0){
        for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
            (*it)->toggleMinified();
        }
    } else if ( _type == 1){
        UIReference last;
        for(int i = 0; i < guis.size(); i++){
            if( i > 0){
                guis[i]->setMinified(true);
            }
            if(last != NULL){
                guis[i]->getRect()->setX(1);
                guis[i]->getRect()->setY(last->getRect()->getY()+last->getRect()->getHeight()+1);
            } else {
                guis[i]->getRect()->setX(1);
                guis[i]->getRect()->setY(1);
            }
            last = guis[i];
        }
    } else if (_type == 2){
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
    } else if (_type == 3 ){
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
    } else if ( _type == 4){
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
}

ofFbo& ofx2DPro::getRenderTarget(int _viewNumber){
    if (_viewNumber>=renderTargets.size()) {
        setupNumViewports(_viewNumber+1);
    }
    
    ofFbo *renderTarget = &renderTargets[_viewNumber];
    int width = ofGetWidth();// / renderTargets.size();
    int height = ofGetHeight();
    if(!renderTarget->isAllocated() || renderTarget->getWidth() != width || renderTarget->getHeight() != height){
        ofFbo::Settings settings;
        settings.width = width;
        settings.height = height;
        settings.internalformat = GL_RGB;
        settings.numSamples = 0;
        settings.useDepth = true;
        settings.useStencil = true;
        settings.depthStencilAsTexture = true;
#ifdef TARGET_OPENGLES
        settings.textureTarget = GL_TEXTURE_2D;
#else
        settings.textureTarget = ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
        renderTarget->allocate(settings);
#endif
		renderTarget->begin();
        ofClear(0, 0,0,0);
        renderTarget->end();
    } 
    
    return *(renderTarget);
}
    
void ofx2DPro::selfPostDraw(){
    int offsetX = 0;
    for (auto &it : renderTargets) {
        if(!renderFlipped){
            it.draw(offsetX,0.0,
                    it.getWidth(),it.getHeight());
        } else {
            it.draw(offsetX,it.getHeight(),
                    it.getWidth(),-it.getHeight());
        }
        offsetX += it.getWidth();
    }
}

void ofx2DPro::windowResized(ofResizeEventArgs &args){
    selfWindowResized(args);
}