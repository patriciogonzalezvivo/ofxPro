//
//  UIBackground.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once

#include "UIClass.h"
#include "aColor.h"

class UIBackground : public UIClass{
public:

    UIBackground();
    virtual ~UIBackground(){};
    
    void    linkUIs( vector<UIReference> *_guis );
    
    string  getClassName(){return "BACKGROUND"; }
    float   getUIBrightness();
    
    virtual void    draw();
    
    aColor  color;
    aColor  color2;
    bool    bGradient;
    
protected:
    virtual void setupUI();
    virtual void guiEvent(ofxUIEventArgs &e);
    
    vector<UIReference> *guis;
};
