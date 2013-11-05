//
//  UI3DProject.h
//
//  Created by Patricio Gonzalez Vivo on 10/15/13.
//
//

#pragma once

#include "UI2DProject.h"

#include "UILight.h"
#include "UICamera.h"
#include "UIMaterial.h"


class UI3DProject : public UI2DProject {
public:
    
	UI3DProject(){};
	virtual ~UI3DProject(){};
	
	//--------------------- VIRTUAL CLASSES TO EDIT
    //
    virtual string getSystemName(){ return "3D_ABSTRACT_PROJECT"; };
    
    virtual void selfDrawBackground(){};        //  2D Scene here
    virtual void selfSceneTransformation(){};   //  3D Previus sceen transformations
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
    
    virtual void keyPressed(ofKeyEventArgs & args){UI2DProject::keyPressed(args);};
	virtual void keyReleased(ofKeyEventArgs & args){UI2DProject::keyReleased(args);};
    
    virtual void mouseMoved(ofMouseEventArgs & args){UI2DProject::mouseMoved(args);};
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
    
    virtual string  cursorIsOverLight();
    
    // CAMERA
    //
    UICamera        camera;
    
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
};