//
//  UIEasyCam.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once

#include "UICamera.h"


class UIEasyCamera : public UICamera {
public:
    
    UIEasyCamera();
    virtual ~UIEasyCamera();
    
    CameraLocation  getCameraLocation();
    
    string          getClassName(){return "EASYCAM";};
    
    virtual void    enableMouseInput();
    virtual void    disableMouseInput();
    
protected:
    void    update(ofEventArgs& args);
};