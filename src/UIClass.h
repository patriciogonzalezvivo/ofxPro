//
//  GuiClass.h
//
//  Created by Patricio Gonzalez Vivo on 9/9/13.
//
//

#pragma once
#include "ofMain.h"

#include "ofxUI.h"

typedef shared_ptr<ofxUISuperCanvas> UIReference;

class UIClass {
public:

    UIClass(){};
    virtual ~UIClass();

    virtual string      getClassName(){return "GUICLASS_DEFAULT";};
    UIReference         getUIReference(ofxUICanvas *_parent);
    bool                bEnable;

protected:
    virtual void        setupUI( ) = 0;
    virtual void        guiEvent(ofxUIEventArgs &e) = 0;
    UIReference         gui;
    
};

