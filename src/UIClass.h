//
//  GuiClass.h
//  LeapMotion
//
//  Created by Patricio Gonzalez Vivo on 9/9/13.
//
//

#pragma once
#include "ofMain.h"

#include "ofxUI.h"

class UIClass {
public:
    
    UIClass();
    ~UIClass();
    
    virtual void        setupUI() = 0;
    virtual string      getClassName(){ return "GUICLASS_DEFAULT"; };
    ofxUISuperCanvas*   getUI();
    
    bool                bEnable;
    
protected:
    
    virtual void        guiEvent(ofxUIEventArgs &e) = 0;
    ofxUISuperCanvas*   gui;
};