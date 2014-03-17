//
//  UIEasyCam.h
//
//  Created by Patricio Gonzalez Vivo on 3/16/14.
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