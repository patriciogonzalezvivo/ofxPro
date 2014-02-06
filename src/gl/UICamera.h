//
//  UICamera.h
//  example
//
//  Created by Patricio Gonzalez Vivo on 11/4/13.
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
    
    void    setupUI();
    
    void            setCameraLocation( const CameraLocation &_camPos );
    CameraLocation  getCameraLocation();
    
    string          getClassName(){return "CAMERA";};
    
    ofCamera*       getCameraPtn(){return camera;};
    ofMatrix4x4     getGlobalTransformMatrix(string _camLocName);
    
    void    enableMouseInput();
    void    disableMouseInput();

    void    billboard();
    
private:
    void    update(ofEventArgs& args);
    void    guiEvent(ofxUIEventArgs &e);
    
    ofCamera *camera;
    string  type;
    float   FOV;
    
    CameraLocation  targetLocation;
    ofxUITextInput  *textField;
    ofxUIRadio      *locations;
    string          pathToLocations;
    string          newLocationName;
    float           pct;
    float           speed;
    bool            bNewLocation;
};


