#pragma once

#include "ofMain.h"

#include "UILight.h"
#include "UIMaterial.h"

#include "UIClass.h"
#include "UIBackground.h"

//  ADDONS
//
#include "ofxUI.h"
#include "ofxGenerative.h"
#include "ofxCameraSaveLoad.h"

class UIProject {
  public:
	   
	UIProject(){};
	~UIProject(){};
	
	//SUB CLASSES USE THESE METHODS:
    virtual string getSystemName();
    
    virtual void selfSetup(){};
    virtual void selfSetupGuis(){};
    
    virtual void selfUpdate(){};
    virtual void selfDrawBackground(){};
    virtual void selfDrawDebug(){};
    virtual void selfSceneTransformation(){};
    virtual void selfDraw(){};
	virtual void selfDrawOverlay(){};
	virtual void selfPostDraw();
	virtual void selfPresetLoaded(string presetPath){};
	
    virtual void selfExit(){};
    virtual void selfBegin(){};
	virtual void selfEnd(){};
	
    virtual void selfKeyPressed(ofKeyEventArgs & args){};
    virtual void selfKeyReleased(ofKeyEventArgs & args){};
    
    virtual void selfMouseDragged(ofMouseEventArgs& data){};
    virtual void selfMouseMoved(ofMouseEventArgs& data){};
    virtual void selfMousePressed(ofMouseEventArgs& data){};
    virtual void selfMouseReleased(ofMouseEventArgs& data){};
    
    virtual void selfSetupGui(){};
    virtual void selfGuiEvent(ofxUIEventArgs &e){};
	
    virtual void selfSetupSystemGui(){};
    virtual void guiSystemEvent(ofxUIEventArgs &e){};
	
    virtual void selfSetupRenderGui(){};
    virtual void guiRenderEvent(ofxUIEventArgs &e){};
    
	//END SUBCLASS METHODS

	//Data Folder Path
    string  getDataPath();
	
	//APP CYCLE EVENTS
	//pre allocate any assets that will cause freezes
	virtual void setup();
    
	//these events are registered only when running the simulation
	virtual void update(ofEventArgs & args);
	virtual void draw(ofEventArgs & args);
	
	//application exit, clean up and don't crash
	virtual void exit(ofEventArgs & args);

	//INTERACTION EVENTS -- registered only
	virtual void keyPressed(ofKeyEventArgs & args);
	virtual void keyReleased(ofKeyEventArgs & args);
	
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseMoved(ofMouseEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);

	//these two methods are called by the controller class,
	//they register events and result in a calls to begin/end on the subclass
	virtual void play();
	virtual void stop();

    //Core Param Setup
    void setupAppParams();
    void setupDebugParams();
    void setupCameraParams();
    void setupLightingParams();
    void setupTimeLineParams();
	
    //Core UI[S]
    void setupCoreGuis();
    
    void setupGui();
    void guiEvent(ofxUIEventArgs &e);
    
    void setupSystemGui();
    void setupRenderGui();
    
    void setupLightingGui();
    void guiLightingEvent(ofxUIEventArgs &e);
    
    virtual void setupCameraGui();
    virtual void guiCameraEvent(ofxUIEventArgs &e);
    
	void setupPresetGui();
	void guiPresetEvent(ofxUIEventArgs &e);
    
    virtual void setupBackground();
    
    void guiAdd( UIClass &_uiClass );
    virtual void guiAllEvents(ofxUIEventArgs &e);
    
    void materialAdd( UIMaterial *m);
    void lightAdd( string _name, ofLightType _type );
    void lightsBegin();
    void lightsDraw();
    void lightsEnd();

    //UI Helpers
	vector<string> getPresets();
	
    virtual void loadGUIS();
	virtual void loadPresetGUISFromName(string presetName);
    virtual void loadPresetGUISFromPath(string presetPath);
    virtual void saveGUIS();
    virtual void savePresetGUIS(string presetName);
    
    void showGUIS();
    void hideGUIS();
    virtual void toggleGUIS();
    void toggleGuiAndPosition(ofxUISuperCanvas *g);
    void deleteGUIS();
    
    virtual ofCamera&   getCameraRef();
    virtual ofFbo&      getRenderTarget();
	virtual bool        cursorIsOverGUI();
	
protected:
	//  UI
    //
    ofxUISuperCanvas *gui;
    ofxUISuperCanvas *sysGui;
    ofxUISuperCanvas *rdrGui;
    ofxUISuperCanvas *bgGui;
    ofxUISuperCanvas *lgtGui;
    ofxUISuperCanvas *camGui;
    ofxUISuperCanvas *presetGui;
    ofxUISuperCanvas *tlGui;
    vector<ofxUISuperCanvas *> guis;
    map<string, ofxUICanvas *> guimap;
	
    //  BACKGROUND
    //
    UIBackground      *background;
    
	ofxUIRadio      *presetRadio;
	
    //  ANIMATORS
    //
    vector<ofx1DExtruder *>         extruders;
	
    //  MATERIAL
    //
    UIMaterial                      *mat;
    map<string, UIMaterial *>       materials;
    map<string, ofxUISuperCanvas *> materialGuis;
	
    //  LIGHTING
    //
    float   *globalAmbientColor;
    bool    bSmoothLighting;
    bool    bEnableLights;
	
    //  LIGHTS
    //
    map<string, UILight* >          lights;
    map<string, ofxUISuperCanvas* > lightGuis;
	
    //  APP
    //
    bool    bRenderSystem;
    bool    bUpdateSystem;
    bool    bDebug;
    bool    bCameraMouse;
	
    //  CAM
    //
    float           camDistance;
    float           camFOV;
    ofEasyCam       cam;
    ofx1DExtruder   *xRot;
    ofx1DExtruder   *yRot;
    ofx1DExtruder   *zRot;

    //  RENDER
    //
    ofFbo           renderTarget;
};
