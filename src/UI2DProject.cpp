//
//  UI2DProject.cpp
//
//  Created by Patricio Gonzalez Vivo on 10/15/13.
//
//

#include "UI2DProject.h"

string UI2DProject::getDataPath(){
#ifdef TARGET_OSX
    string path = "../../../data/"+getSystemName()+"/";
#else
    string path = getSystemName()+"/";
#endif
    return path;
}

void UI2DProject::setup(){
    
	cout << "SETTING UP SYSTEM " << getSystemName() << endl;
	background = NULL;
    
	ofAddListener(ofEvents().exit, this, &UI2DProject::exit);
    
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
    bRenderSystem = true;
    bUpdateSystem = true;
    bDebug = false;
    
//#ifdef TARGET_RASPBERRY_PI
    consoleListener.setup(this);
//#endif    
    
    selfSetup();
    setupCoreGuis();
    selfSetupGuis();
    
	hideGUIS();
    
    ofRegisterMouseEvents(this);
    ofRegisterKeyEvents(this);
    ofAddListener(ofEvents().update, this, &UI2DProject::update);
    ofAddListener(ofEvents().draw, this, &UI2DProject::draw);
    
    loadGUIS();
    hideGUIS();
}

void UI2DProject::play(){
    selfBegin();
    bDebug = false;
}

void UI2DProject::stop(){
    
    hideGUIS();
    saveGUIS();
    
    ofUnregisterMouseEvents(this);
    ofUnregisterKeyEvents(this);
    ofRemoveListener(ofEvents().update, this, &UI2DProject::update);
    ofRemoveListener(ofEvents().draw, this, &UI2DProject::draw);
    
    selfEnd();
}

void UI2DProject::update(ofEventArgs & args){
    if(bUpdateSystem){
        selfUpdate();
    }
}

void UI2DProject::draw(ofEventArgs & args){
    ofPushStyle();
    if(bRenderSystem){
        
#ifdef TARGET_RASPBERRY_PI
        
#else
        UI2DProject::getRenderTarget().begin();
#endif
        {
            //  Background
            //
            if ( background != NULL ){
                background->draw();
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
#ifdef TARGET_RASPBERRY_PI
        
#else
        UI2DProject::getRenderTarget().end();
        
        //  Post-Draw ( shader time )
        //
        selfPostDraw();
#endif
	}
    ofPopStyle();
}

void UI2DProject::exit(ofEventArgs & args){
    saveGUIS();
    guis.clear();
    selfExit();
}

//------------------------------------------------------- KEYBOARD
void UI2DProject::keyPressed(ofKeyEventArgs & args){
    
    cout << "You just pressed " << char(args.key) << endl;
    
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
        if((*it)->hasKeyboardFocus()){
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

void UI2DProject::keyReleased(ofKeyEventArgs & args){
    switch (args.key){
        case 'p':{
            for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
                (*it)->setDrawWidgetPadding(false);
            }
        }
            break;
            
        default:
            selfKeyReleased(args);
            break;
    }
}

//-------------------------------------------------------- MOUSE

bool UI2DProject::cursorIsOverGUI(){
    for(int i = 0; i < guis.size(); i++){
		if(guis[i]->isHit(ofGetMouseX(), ofGetMouseY())){
			return true;
		}
	}
	return false;
}

void UI2DProject::mouseDragged(ofMouseEventArgs& data){
    selfMouseDragged(data);
}

void UI2DProject::mouseMoved(ofMouseEventArgs& data){
    selfMouseMoved(data);
}

void UI2DProject::mousePressed(ofMouseEventArgs & args){
    selfMousePressed(args);
}

void UI2DProject::mouseReleased(ofMouseEventArgs & args){
    selfMouseReleased(args);
}

//------------------------------------------------------------ SETUP

void UI2DProject::setupCoreGuis(){
    
    setupGui();
    setupSystemGui();
    setupRenderGui();
    setupPresetGui();
    setupBackground();
    
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
    ofAddListener(gui->newGUIEvent,this,&UI2DProject::guiEvent);
    
    guis.push_back(gui);
}

vector<string> UI2DProject::getPresets(){
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

void UI2DProject::guiEvent(ofxUIEventArgs &e){
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

void UI2DProject::setupSystemGui(){
    UIReference tmp( new ofxUISuperCanvas("SYSTEM", guiTemplate) );
    
    sysGui = tmp;
    sysGui->copyCanvasStyle( guiTemplate );
    sysGui->copyCanvasProperties( guiTemplate );
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
    
    rdrGui->addSpacer();
    selfSetupRenderGui();
    
    rdrGui->autoSizeToFitWidgets();
    ofAddListener(rdrGui->newGUIEvent,this,&UI2DProject::guiRenderEvent);
    guis.push_back(rdrGui);
}

void UI2DProject::setupPresetGui(){
    UIReference tmp( new ofxUISuperCanvas("PRESENTS", guiTemplate) );
	presetGui = tmp;
    presetGui->setTriggerWidgetsUponLoad(false);
	presetGui->setName("Presets");
	presetGui->copyCanvasStyle(guiTemplate);
    presetGui->copyCanvasProperties(guiTemplate);
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
    ofAddListener(presetGui->newGUIEvent,this,&UI2DProject::guiPresetEvent);
    guis.push_back(presetGui);
}

void UI2DProject::guiPresetEvent(ofxUIEventArgs &e){
    ofxUIToggle *t = (ofxUIToggle *) e.widget;
    if(t->getValue()){
        loadPresetGUISFromName(e.widget->getName());
    }
}

void UI2DProject::setupBackground(){
    background = new UIBackground();
    background->linkUIs( &guis );
    guiAdd( *background );
}

void UI2DProject::guiAdd(UIClass &_uiClass){
    UIReference uiClass = _uiClass.getUIReference(guiTemplate);
	guis.push_back(uiClass);
}

void UI2DProject::guiAllEvents(ofxUIEventArgs &e){
    
}

void UI2DProject::loadGUIS(){
    for(int i = 0; i < guis.size(); i++){
        guis[i]->loadSettings(getDataPath()+"Presets/Working/"+guis[i]->getName()+".xml");
    }
}

void UI2DProject::saveGUIS(){
    for(int i = 0; i < guis.size(); i++){
        guis[i]->saveSettings(getDataPath()+"Presets/Working/"+guis[i]->getName()+".xml");
    }
}

void UI2DProject::loadPresetGUISFromName(string presetName){
	loadPresetGUISFromPath(getDataPath()+"Presets/"+ presetName);
}

void UI2DProject::loadPresetGUISFromPath(string presetPath){
    
    for(int i = 0; i < guis.size(); i++){
        guis[i]->loadSettings(presetPath+"/"+guis[i]->getName()+".xml");
    }
    
	selfPresetLoaded(presetPath);
}

void UI2DProject::savePresetGUIS(string presetName){
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
}

void UI2DProject::showGUIS(){
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->enable();
    }
}

void UI2DProject::hideGUIS(){
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->disable();
    }
}

void UI2DProject::toggleGUIS(){
    for(vector<UIReference>::iterator it = guis.begin(); it != guis.end(); ++it){
        (*it)->toggleVisible();
    }
}

void UI2DProject::toggleGuiAndPosition(UIReference &g){
    if(g->isMinified()){
        g->setMinified(false);
        g->setPosition(ofGetMouseX(), ofGetMouseY());
    } else {
        g->setMinified(true);
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