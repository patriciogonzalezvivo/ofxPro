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
    ~UIBackground();
    
    virtual void    setupUI();
    
    void    linkUIs( vector<ofxUISuperCanvas *> *_guis );
    void    linkCamera( ofEasyCam *_cam );
    
    string  getClassName(){return "BACKGROUND"; }
    
    virtual void    draw();
    
    ofFloatColor    color;
    ofPoint         HSBTarget;
    
    ofFloatColor    color2;
    ofPoint         HSBTarget2;
    
    
    bool            bChange;
    
protected:
    virtual void    guiEvent(ofxUIEventArgs &e);
    
    vector<ofxUISuperCanvas *> *guis;
    ofEasyCam       *cam;
    
    int             gradientMode;
	    
    Particle        HSB;
    Particle        HSB2;
    
    ofxUISlider     *hueSlider;
    ofxUISlider     *satSlider;
    ofxUISlider     *briSlider;
    
    float           speed;
};
