//
//  UIFog.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once

#include "UIClass.h"
#include "UIBackground.h"

class UIFog : public UIClass{
public:
    
    UIFog();
    virtual ~UIFog(){};
    
    void    setupUI();
    void    setColor( ofColor _color );
    void    linkColor( UIBackground *_background );
    
    string  getClassName(){return "FOG"; }
    
    void    begin();
    void    end();
    
    float   density_exponent;
    float   density;
    
protected:
    void    guiEvent(ofxUIEventArgs &e);
    
    ofxUISlider *hue;
    ofxUISlider *sat;
    ofxUISlider *bri;

    aColor color;
    aColor *bgColor;

    bool        bMachBackground;
};
