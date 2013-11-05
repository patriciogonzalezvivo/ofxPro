//
//  Background.h
//
//  Created by Patricio Gonzalez Vivo on 9/14/13.
//
//

#pragma once

#include "UIClass.h"
#include "Particle.h"

class UIBackground : public UIClass{
public:

    UIBackground();
    virtual ~UIBackground();
    
    virtual void    setupUI();
    
    void    setColor( ofFloatColor _color );
    void    setColor2( ofFloatColor _color );
    
    void    linkUIs( vector<UIReference> *_guis );
    
    string  getClassName(){return "BACKGROUND"; }
    
    ofFloatColor&    getColor();
    ofFloatColor&    getColor2();
    
    virtual void    draw();
    
protected:
    virtual void    guiEvent(ofxUIEventArgs &e);
    
    vector<UIReference> *guis;
    
    int             gradientMode;
	   
    ofFloatColor    color;
    Particle        HSB;
    ofPoint         HSBTarget;
    
    ofFloatColor    color2;
    Particle        HSB2;
    ofPoint         HSBTarget2;
    
    ofxUISlider     *hueSlider;
    ofxUISlider     *satSlider;
    ofxUISlider     *briSlider;
    
    float           speed;
    bool            bChange;
};
