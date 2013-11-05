//
//  UICamera.h
//  example
//
//  Created by Patricio Gonzalez Vivo on 11/4/13.
//
//

#pragma once

#include "UIClass.h"

class UICamera : public UIClass{
public:
    
    UICamera();
    virtual ~UICamera(){};
    
    bool    save(string savePath);
    bool    load(string savePath);
    
    void    setupUI();
    
    void    reset();
    void    enableMouseInput();
    void    disableMouseInput();
    
    string  getClassName(){return "CAMERA";};
    ofCamera& getCameraRef(){return (*camera);};
    ofCamera* getCameraPtn(){return camera;};
    
    void    billboard();
    
private:
    void    guiEvent(ofxUIEventArgs &e);
    
    ofCamera *camera;
    string  type;
    
    float   FOV;
};


