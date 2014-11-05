//
//  UICamera.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once

#include "UIClass.h"
#include "CameraLocation.h"

class UICamera : public UIClass {
public:
    
    UICamera();
    virtual ~UICamera();
    
    bool    save(string _savePath);
    bool    load(string _savePath);
    
    void    addLocation(string _name);
    void    loadLocations(string _path);
    
    bool    saveLocation(string _savePath);
    bool    loadLocation(string _savePath);
    
    virtual void    setupUI();
    
    virtual void            setCameraLocation( const CameraLocation &_camPos );
    virtual CameraLocation  getCameraLocation();
    
    virtual string  getClassName(){return "CAMERA";};
    
    ofCamera*       getCameraPtn(){return camera;};
    ofMatrix4x4     getGlobalTransformMatrix(string _camLocName);
    
    virtual float   getPct();

    virtual void    enableMouseInput(){bMouse = true;};
    virtual void    disableMouseInput(){bMouse = false;};
    
    virtual void    billboard();
    
protected:
    virtual void    update(ofEventArgs& args);
    virtual void    guiEvent(ofxUIEventArgs &e);
    
    ofCamera        *camera;
    float           FOV;
    
    CameraLocation  targetLocation;
    
    ofxUITextInput  *textField;
    ofxUIRadio      *locations;
    string          pathToLocations;
    string          newLocationName;
    float           pct;
    float           lerpPct;
    bool            bNewLocation;
    
    bool            bMouse;
};

typedef shared_ptr<UICamera> UICameraReference;


