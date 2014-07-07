//
//  ofx2DPro.h
//
//  Copyright (c) 2013 Patricio Gonzalez Vivo <http://patriciogonzalezvivo.com>
//
//

//#define PACKED_APP

#pragma once
#include "ofMain.h"

//  ADDONS
//
#include "ofxUI.h"

//  The basis
//
#include "UIClass.h"
#include "UIBackground.h"
#include "UIShader.h"
#include "UILog.h"

//  System
//
#ifdef TARGET_OSX
#include "SystemTextbox.h"
#endif
#include "HotKeys.h"

//  RaspberryPi Extras
//
#ifdef TARGET_RASPBERRY_PI
#include "ConsoleListener.h"
#endif

#ifdef TARGET_RASPBERRY_PI
class ofx2DPro : public SSHKeyListener {
#else 
class ofx2DPro {
#endif
public:
    
	ofx2DPro():background(NULL),bPlaying(false){};
	virtual ~ofx2DPro(){};
	
	//--------------------- VIRTUAL CLASSES TO EDIT
    //
    virtual string getSystemName(){ return "2D_ABSTRACT_PROJECT"; };
    
    virtual void selfSetup(){};
    virtual void selfSetupGuis(){};
    
    virtual void selfUpdate(){};
    virtual void selfDraw(){};
	virtual void selfDrawOverlay(){};
	virtual void selfPostDraw();
	virtual void selfPresetLoaded(string presetPath){};
	
    virtual void selfExit(){};
    virtual void selfBegin(){};
	virtual void selfEnd(){};
	
    virtual void selfWindowResized(ofResizeEventArgs & args){};
    
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
    virtual ofFbo& getRenderTarget(int _viewNumber=0);

	//  These events are registered to be call automatically
    //
	virtual void update(ofEventArgs & args);
	virtual void draw(ofEventArgs & args);
	virtual void exit(ofEventArgs & args);
    virtual void windowResized(ofResizeEventArgs & args);
    
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
    virtual void    guiAdd(UIClass &_UIClass);
    virtual void    guiEvent(ofxUIEventArgs &e);
    virtual void    guiAllEvents(ofxUIEventArgs &e);
    
	virtual bool    cursorIsOverGUI();
    ofxUISuperCanvas    *guiTemplate;
    vector<UIReference> guis;
    string          currentPresetName;
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

    virtual void    setupNumViewports(int num);
    int             currentViewPort;
    
    virtual void    setupRenderIsFlipped(bool flipped=true);
    bool            renderFlipped;
    vector<ofFbo>   renderTargets;
    
    //  BACKGROUND GUI
    //
    virtual void    backgroundSet(UIBackground *_bg);
    UIBackground    *background;
    
    //  LOG GUI
    //
    UILog           logGui;
};