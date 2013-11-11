//
//  UI2DProject.h
//
//  Created by Patricio Gonzalez Vivo on 10/15/13.
//
//
#pragma once
#include "ofMain.h"

//  ADDONS
//
#include "ofxUI.h"

//  The basis
//
#include "UIClass.h"
#include "UIBackground.h"
#include "UISuperBackground.h"
#include "UILog.h"

//  RaspberryPi Extras
//
#ifdef TARGET_RASPBERRY_PI
#include "ConsoleListener.h"
#endif

#ifdef TARGET_RASPBERRY_PI
class UI2DProject : public SSHKeyListener {
#else 
class UI2DProject {
#endif
public:
    
	UI2DProject():background(NULL),bPlaying(false){};
	virtual ~UI2DProject(){};
	
	//--------------------- VIRTUAL CLASSES TO EDIT
    //
    virtual string getSystemName(){ return "2D_ABSTRACT_PROJECT"; };
    
    virtual void selfSetup(){};
    virtual void selfSetupGuis(){};
    
    virtual void selfUpdate(){};
    virtual void selfDraw(){};
    virtual void selfDrawDebug(){};
	virtual void selfDrawOverlay(){};
	virtual void selfPostDraw();
	virtual void selfPresetLoaded(string presetPath){};
	
    virtual void selfExit(){};
    virtual void selfBegin(){};
	virtual void selfEnd(){};
	
    virtual void selfKeyPressed(ofKeyEventArgs &args){};
    virtual void selfKeyReleased(ofKeyEventArgs &args){};
    
#ifdef TARGET_RASPBERRY_PI
    void onCharacterReceived(SSHKeyListenerEventData& e){
        ofKeyEventArgs arg;
        arg.key = (int)e.character;
        keyPressed(arg);
    }
    ConsoleListener consoleListener;
#endif
    
    virtual void selfMouseDragged(ofMouseEventArgs &data){};
    virtual void selfMouseMoved(ofMouseEventArgs &data){};
    virtual void selfMousePressed(ofMouseEventArgs &data){};
    virtual void selfMouseDoublePressed(ofMouseEventArgs &data){};
    virtual void selfMouseReleased(ofMouseEventArgs &data){};
    
    virtual void selfGuiEvent(ofxUIEventArgs &e){};
	
    virtual void selfSetupSystemGui(){};
    virtual void guiSystemEvent(ofxUIEventArgs &e){};
	
    virtual void selfSetupRenderGui(){};
    virtual void guiRenderEvent(ofxUIEventArgs &e){};
    //
    //----------------------
    
	// Interface functions
    //
	virtual void setup();
    virtual void play();
	virtual void stop();
    
    //  Data Folder Path
    //
    string  getDataPath();
    virtual ofFbo& getRenderTarget();

	//  These events are registered to be call automatically
    //
	virtual void update(ofEventArgs & args);
	virtual void draw(ofEventArgs & args);
	virtual void exit(ofEventArgs & args);
	virtual void keyPressed(ofKeyEventArgs & args);
	virtual void keyReleased(ofKeyEventArgs & args);
	
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);
    
protected:
    
    //  GENERAL UI stuff
    //
    virtual void    guiLoad();
    virtual void    guiLoadPresetFromName(string presetName);
    virtual void    guiLoadPresetFromPath(string presetPath);
    virtual void    guiSave();
    virtual void    guiSavePreset(string presetName);
    virtual void    guiShow();
    virtual void    guiHide();
    virtual void    guiToggle();
    virtual void    guiArrange( int _order_type );
    virtual void    guiAdd(UIClass &_uiClass);
    virtual void    guiEvent(ofxUIEventArgs &e);
    virtual void    guiAllEvents(ofxUIEventArgs &e);
    
	virtual bool    cursorIsOverGUI();
    ofxUISuperCanvas    *guiTemplate;
    vector<UIReference> guis;
    float           lastClick;
    float           doublClickThreshold;
    bool            bPlaying;
    bool            bGui;
    
    //  SCENE GUI
    //
    virtual void    setupGui();
    virtual void    setupCoreGuis();
    vector<string>  getPresets();
    ofxUIRadio      *presetRadio;
    UIReference     gui;
    bool            bDebug;
    bool            bEdit;

    //  UPDATE GUI
    //
    virtual void    setupSystemGui();
    UIReference     sysGui;
    bool            bUpdateSystem;
    
    //  RENDER GUI
    //
    virtual void    setupRenderGui();
    UIReference     rdrGui;
    bool            bRenderSystem;
    ofFbo           renderTarget;
    
    //  BACKGROUND GUI
    //
    virtual void    backgroundSet(UIBackground *_bg);
    UIBackground    *background;
    
    //  LOG GUI
    //
    UILog           logGui;
};