//
//  UI2DProject.cpp
//
//  Copyright (c) 2013 Patricio Gonzalez Vivo <http://patriciogonzalezvivo.com>
//
//

#include "UI2DProject.h"

string UI2DProject::getDataPath(){
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

void UI2DProject::setup(){
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
    
    ofSetSphereResolution(30);
    
    doublClickThreshold = 0.2;
    
    selfSetup();
    setupCoreGuis();
    selfSetupGuis();
}

void UI2DProject::play(){
    
    if (!bPlaying){
        selfBegin();
        
        bDebug = false;
        bRenderSystem = true;
        bUpdateSystem = true;
        
#ifdef TARGET_RASPBERRY_PI
        consoleListener.setup(this);
#endif
        ofRegisterMouseEvents(this);
        ofRegisterKeyEvents(this);
        ofAddListener(ofEvents().update,this,&UI2DProject::update);
        ofAddListener(ofEvents().draw,this,&UI2DProject::draw);
        ofAddListener(ofEvents().exit,this,&UI2DProject::exit);
        
        ofAddListener(ofEvents().windowResized, this, &UI2DProject::windowResized);
        
        guiLoad();
        guiHide();
        
        bPlaying = true;
    }
}

void UI2DProject::stop(){
    if (bPlaying){
        if(logGui.isRecording()){
            logGui.record(false);
        }
        
        guiHide();
        guiSave();
        
        ofUnregisterMouseEvents(this);
        ofUnregisterKeyEvents(this);
        ofRemoveListener(ofEvents().update, this, &UI2DProject::update);
        ofRemoveListener(ofEvents().draw, this, &UI2DProject::draw);
        ofRemoveListener(ofEvents().exit, this, &UI2DProject::exit);
        
        selfEnd();
        
        bRenderSystem = false;
        bUpdateSystem = false;
        bPlaying = false;
    }
}

void UI2DProject::update(ofEventArgs & args){
    if(bUpdateSystem){
        selfUpdate();
        if(logGui.isRecording()){
            logGui.recordAddFrame(getRenderTarget());
        }
    }
}

void UI2DProject::draw(ofEventArgs & args){
    ofPushStyle();
    if(bRenderSystem){
        
#ifdef TARGET_RASPBERRY_PI
        //  a full screen FBO is to much for RPI
        //
#else
        UI2DProject::getRenderTarget().begin();
#endif
        {
            //  Background
            //
            if ( background != NULL ){
                background->draw();
            }
            
            //  Draw Scene
            //
            {
                ofPushStyle();
                ofPushMatrix();
                selfDraw();
                ofPopMatrix();
                ofPopStyle();
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
            
            //  Draw Overlay
            //
            {
                ofPushStyle();
                ofPushMatrix();
                selfDrawOverlay();
                ofPopMatrix();
                ofPopStyle();
            }
            
            //  Draw Log
            //
            {
                ofPushStyle();
                ofPushMatrix();
                ofSetColor(background->getUIBrightness()*255);
                logGui.draw();
                ofPopMatrix();
                ofPopStyle();
            }
        }
#ifdef TARGET_RASPBERRY_PI
        //  a full screen FBO is to much for RPI
        //
#else
        UI2DProject::getRenderTarget().end();
        selfPostDraw();
#endif
        
        logGui.drawStatus();
	}
    
    ofPopStyle();
}

void UI2DProject::exit(ofEventArgs & args){
    if(logGui.isRecording()){
        logGui.record(false);
    }
    
    guiSave();
    guis.clear();
    selfExit();
}

//------------------------------------------------------- KEYBOARD
void UI2DProject::keyPressed(ofKeyEventArgs & args){
    
    if (bGui){
        for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
            if((*it)->hasKeyboardFocus()){
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
    if( getModifierSpecialPressed()){
#else
    if( getModifierControlPressed() ){
#endif
        switch (args.key){
            case 's':
                guiSavePreset(currentPresetName);
                logGui.screenShot(currentPresetName);
                break;
            case 'S':{
#ifdef TARGET_OSX
                string presetName = SystemTextBoxDialog("Save Preset As", currentPresetName);
#else
                string presetName = ofSystemTextBoxDialog("Save Preset As", currentPresetName);
#endif
                if(presetName != ""){
                    guiSavePreset(presetName);
                    currentPresetName = presetName;
                }
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

void UI2DProject::keyReleased(ofKeyEventArgs & args){
    switch (args.key){
        default:
            selfKeyReleased(args);
            break;
    }
}

//-------------------------------------------------------- MOUSE

bool UI2DProject::cursorIsOverGUI(){
    if (bGui){
        for(int i = 0; i < guis.size(); i++){
            if(guis[i]->isHit(ofGetMouseX(), ofGetMouseY())){
                return true;
            }
        }
    }
	return false;
}

void UI2DProject::mouseMoved(ofMouseEventArgs& data){
    selfMouseMoved(data);
}

void UI2DProject::mousePressed(ofMouseEventArgs & args){
    if(cursorIsOverGUI()){
        return;
    }
    
    if (logGui.isTakingNotes()){
        logGui.penDown(ofPoint(args.x,args.y));
        return;
    }
   
    if(ofGetElapsedTimef()-lastClick<doublClickThreshold)
        selfMouseDoublePressed(args);
    else
        selfMousePressed(args);
}

void UI2DProject::mouseDragged(ofMouseEventArgs& args){
    if(cursorIsOverGUI())
        return;
    
    if (logGui.isTakingNotes()){
        logGui.penDown(ofPoint(args.x,args.y));
        return;
    }
    
    selfMouseDragged(args);
}

void UI2DProject::mouseReleased(ofMouseEventArgs & args){
    if(cursorIsOverGUI())
        return;
    
    if (logGui.isTakingNotes()){
        logGui.penUp();
        return;
    }
    
    lastClick = ofGetElapsedTimef();
    selfMouseReleased(args);
}

//------------------------------------------------------------ SETUP

void UI2DProject::setupCoreGuis(){
    setupGui();
    
    logGui.linkDataPath(getDataPath());
    guiAdd(logGui);
    
    setupSystemGui();
    setupRenderGui();
    
    backgroundSet(new UIBackground());
}

void UI2DProject::setupGui(){
    
    guiTemplate = new ofxUISuperCanvas(ofToUpper(getSystemName()));
    guiTemplate->setName("TEMPLATE");
    guiTemplate->setWidgetFontSize(OFX_UI_FONT_SMALL);
    
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
    
    ofAddListener(gui->newGUIEvent,this,&UI2DProject::guiEvent);
    
    guis.push_back(gui);
}

void UI2DProject::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();

    if(name == "SAVE"){
        ofxUIButton *b = (ofxUIButton *) e.widget;
        if(b->getValue()){
#ifdef TARGET_OSX
            string presetName = SystemTextBoxDialog("Save Preset As", currentPresetName);
#else
			string presetName = ofSystemTextBoxDialog("Save Preset As", currentPresetName);
#endif
            if(presetName != ""){
                guiSavePreset(presetName);
				currentPresetName = presetName;
            }
        }
    } else if(name == "LOAD"){
        ofxUIButton *b = (ofxUIButton *) e.widget;
        if(b->getValue()){
            ofFileDialogResult result = ofSystemLoadDialog("Load Visual System Preset Folder", true, getDataPath()+"Presets/");
            if(result.bSuccess && result.fileName.length()){
                guiLoadPresetFromPath(result.filePath);
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
                guiLoadPresetFromName(name);
            }
        }
    }
    selfGuiEvent(e);
}

vector<string> UI2DProject::getPresets(){
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

void UI2DProject::setupSystemGui(){
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
    ofAddListener(sysGui->newGUIEvent,this,&UI2DProject::guiSystemEvent);
    guis.push_back(sysGui);
}

void UI2DProject::setupRenderGui(){
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
    ofAddListener(rdrGui->newGUIEvent,this,&UI2DProject::guiRenderEvent);
    guis.push_back(rdrGui);
}

void UI2DProject::backgroundSet(UIBackground *_bg){
    if(background != NULL){
        
        for(int i = 0; i<guis.size(); i++){
            if (guis[i]->getName() == "BACKGROUND"){
                guis.erase(guis.begin()+i);
                break;
            }
        }
        
        delete background;
        background = NULL;
    }
    
    background = _bg;
    background->linkUIs( &guis );
    guiAdd( *background );
}

void UI2DProject::guiAdd(UIClass &_uiClass){
    UIReference uiClass = _uiClass.getUIReference(guiTemplate);
	guis.push_back(uiClass);
}

void UI2DProject::guiAllEvents(ofxUIEventArgs &e){
    
}

// LOAD
//
void UI2DProject::guiLoad(){
    for(int i = 0; i < guis.size(); i++){
        guis[i]->loadSettings(getDataPath()+"Presets/Working/"+guis[i]->getName()+".xml");
    }
}

void UI2DProject::guiLoadPresetFromName(string presetName){
    guiLoadPresetFromPath(getDataPath()+"Presets/"+ presetName);
}
    
void UI2DProject::guiLoadPresetFromPath(string presetPath){
    for(int i = 0; i < guis.size(); i++){
        guis[i]->loadSettings(presetPath+"/"+guis[i]->getName()+".xml");
    }
    selfPresetLoaded(presetPath);
    currentPresetName = ofFilePath::getBaseName(presetPath);
}

//  SAVE
//
void UI2DProject::guiSave(){
    for(int i = 0; i < guis.size(); i++){
        guis[i]->saveSettings(getDataPath()+"Presets/Working/"+guis[i]->getName()+".xml");
    }
}


void UI2DProject::guiSavePreset(string presetName){
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
void UI2DProject::guiShow(){
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->enable();
    }
    bGui = true;
}

void UI2DProject::guiHide(){
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->disable();
    }
    bGui = false;
}

void UI2DProject::guiToggle(){
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->toggleVisible();
    }
    bGui = !bGui;
}

void UI2DProject::guiArrange(int _type){
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

ofFbo& UI2DProject::getRenderTarget(){
    if(!renderTarget.isAllocated() || renderTarget.getWidth() != ofGetWidth() || renderTarget.getHeight() != ofGetHeight()){
        ofFbo::Settings settings;
        settings.width = ofGetWidth();
        settings.height = ofGetHeight();
        settings.internalformat = GL_RGB;
        settings.numSamples = 0;
        settings.useDepth = true;
        settings.useStencil = true;
        settings.depthStencilAsTexture = true;
#ifdef TARGET_OPENGLES
        settings.textureTarget = GL_TEXTURE_2D;
#else
        settings.textureTarget = ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
        renderTarget.allocate(settings);
#endif
		renderTarget.begin();
		ofClear(0,0,0,0);
		renderTarget.end();
    }
    
    return renderTarget;
}

void UI2DProject::selfPostDraw(){
	UI2DProject::getRenderTarget().draw(0, 0,UI2DProject::getRenderTarget().getWidth(), UI2DProject::getRenderTarget().getHeight());
}

void UI2DProject::windowResized(ofResizeEventArgs &args){
    selfWindowResized(args);
}