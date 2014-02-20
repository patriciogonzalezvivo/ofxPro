//
//  UISyphon.h
//
//  Created by Patricio Gonzalez Vivo on 2/18/14.
//
//

#pragma once

#include "UIClass.h"
#include "SyphonTextures.h"

class UISyphon: public UIClass{
public:
    
    UISyphon();
    virtual ~UISyphon(){};
    
    string  getClassName(){return "Syphon";}
    
    SyphonTexture   inPut;
    ofxSyphonServer outPut;
    
protected:
    virtual void    setupUI();
    virtual void    guiEvent(ofxUIEventArgs &e);
};
