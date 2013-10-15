//
//  UI3DProject.h
//
//  Created by Patricio Gonzalez Vivo on 10/15/13.
//
//

#pragma once

#include "UI2DProject.h"
#include "UILight.h"
#include "UIMaterial.h"

//  ADDONS
//
#include "ofxGenerative.h"
#include "ofxCameraSaveLoad.h"

class UI3DProject : public UI2DProject {
public:
    
	UI3DProject(){};
	~UI3DProject(){};
	
	//--------------------- VIRTUAL CLASSES TO EDIT
    //
    virtual string getSystemName(){ return "3D_ABSTRACT_PROJECT"; };
    
    virtual void selfDrawBackground(){};
    virtual void selfSceneTransformation(){};
    
	//---------------------
    
	// Interface functions
    //
	virtual void setup();
    virtual void play();
	virtual void stop();
    
	// these events are registered only when running the simulation
	virtual void update(ofEventArgs & args);
	virtual void draw(ofEventArgs & args);
	virtual void exit(ofEventArgs & args);
    
	virtual void keyPressed(ofKeyEventArgs & args);
	virtual void mousePressed(ofMouseEventArgs & args);
    
	virtual void keyReleased(ofKeyEventArgs & args){UI2DProject::keyReleased(args);};
	virtual void mouseDragged(ofMouseEventArgs & args){UI2DProject::mouseDragged(args);};
	virtual void mouseMoved(ofMouseEventArgs & args){UI2DProject::mouseMoved(args);};
	virtual void mouseReleased(ofMouseEventArgs & args){UI2DProject::mouseReleased(args);};
    
    // CORE
    virtual void setupCoreGuis();
    virtual void setupBackground();
    
    virtual void loadGUIS();
    virtual void loadPresetGUISFromPath(string presetPath);
    virtual void saveGUIS();
    virtual void savePresetGUIS(string presetName);
    
    // 3D SPECIFIC
    virtual void setupLightingGui();
    virtual void setupLightingParams();
    virtual void guiLightingEvent(ofxUIEventArgs &e);
    
    virtual void lightAdd( string _name, ofLightType _type );
    virtual void lightsBegin();
    virtual void lightsDraw();
    virtual void lightsEnd();
    
    virtual void setupCameraGui();
    virtual void setupCameraParams();
    virtual void guiCameraEvent(ofxUIEventArgs &e);
    
    virtual void materialAdd( string _name );
    
    virtual ofCamera&   getCameraRef();
	
protected:
    
    //  GUI
    UIReference     lgtGui, camGui;
	
    //  CAMERA
    //
    float           camDistance,camFOV;
    ofx1DExtruder   *xRot, *yRot, *zRot;
    vector<ofx1DExtruder *> extruders;
    ofEasyCam       cam;
	
    //  LIGHTS & MATERIALs
    //
    map<string, UILightReference >  lights;
    map<string, UIMaterialReference> materials;
    float   *globalAmbientColor;
    bool    bSmoothLighting;
    bool    bEnableLights;
};