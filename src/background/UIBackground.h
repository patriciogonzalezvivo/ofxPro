//
//  Background.h
//
//  Created by Patricio Gonzalez Vivo on 9/14/13.
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
    virtual void    setupUI();
    virtual void    guiEvent(ofxUIEventArgs &e);
    
    vector<UIReference> *guis;
    
    ofxUISlider     *hueSlider;
    ofxUISlider     *satSlider;
    ofxUISlider     *briSlider;
};
