//
//  ofx3DPro.h
//
//  Copyright (c) 2013 Patricio Gonzalez Vivo <http://patriciogonzalezvivo.com>.
//
//

#pragma once

#include "ofx2DPro.h"

#include "UIFog.h"
#include "UILight.h"
#include "UIMaterial.h"

#include "UICamera.h"
#include "UIEasyCam.h"
#include "UIGameCam.h"

#include "Cursor.h"

class ofx3DPro : public ofx2DPro {
public:
    
	ofx3DPro(){};
	virtual ~ofx3DPro(){};
	
	//--------------------- VIRTUAL CLASSES TO EDIT
    //
    virtual string getSystemName(){ return "3D_ABSTRACT_PROJECT"; };
    
    virtual void selfSceneTransformation(int _viewNum){};   //  3D Previus sceen transformations
    //
	//---------------------
    
	// Interface functions
    //
    virtual void play();
	virtual void stop();
    
    virtual ofCamera& getCameraRef();
    
	//  These events are registered to be call automatically
    //
	virtual void draw(ofEventArgs & args);
	virtual void exit(ofEventArgs & args);

    virtual void mouseMoved(ofMouseEventArgs & args);
    virtual void mousePressed(ofMouseEventArgs & args);
	virtual void mouseDragged(ofMouseEventArgs & args);
	virtual void mouseReleased(ofMouseEventArgs & args);
	
protected:
    
    // CORE
    virtual void setupCoreGuis();
    
    virtual void guiLoad();
    virtual void guiLoadPresetFromPath(string presetPath);
    virtual void guiSave();
    virtual void guiSavePreset(string presetName);
    
    virtual void    backgroundSet(UIBackground *_bg);
    virtual string  cursorIsOverLight();
    
    // CAMERA
    //
    //  BACKGROUND GUI
    //
    virtual void    cameraSet(UICamera *_cam);
    virtual void    cameraEnable(bool enable=true);
    UICamera        *camera;
    bool            cameraEnabled;
    
    UIFog           fog;
    
    ofPoint         unproject(ofPoint _screen);
    void            unprojectCursor(MovingCursor &_cursor, float _x, float _y);
    MovingCursor    cursor;
    bool            bUpdateCursor;
    
    GLint           viewport[4];
	GLdouble        matM[16], matP[16];
    ofMatrix4x4     viewMatrix,projectionMatrix;
    
    //  LIGHTS
    //
    virtual void    setupLightingGui();
    virtual void    guiLightingEvent(ofxUIEventArgs &e);
    
    virtual void    lightAdd( string _name, ofLightType _type );
    virtual void    lightsBegin();
    virtual void    lightsDraw();
    virtual void    lightsEnd();
    
    map<string,UILightReference>    lights;
    UIReference     lightsGui;
    string          selectedLigth;
    float           *globalAmbientColor;
    bool            bSmoothLighting;
    bool            bEnableLights;
    
    //  MATERIALs
    //
    virtual void    materialAdd( string _name );
    map<string,UIMaterialReference> materials;
    
    bool            bBackCull,bFrontCull;
};