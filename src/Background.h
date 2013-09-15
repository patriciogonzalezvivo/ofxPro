//
//  Background.h
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/14/13.
//
//

#pragma once

#include "UIClass.h"
#include "Particle.h"

class Background : public UIClass{
public:

    Background();
    ~Background();
    
    void    setupUI();
    void    setLinkedUIs( vector<ofxUISuperCanvas *> *_guis );
    string  getClassName(){return "BACKGROUND"; }
    
    
    void    draw();
    
protected:
    
    void    guiEvent(ofxUIEventArgs &e);
    
    vector<ofxUISuperCanvas *> *guis;
    
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
};
