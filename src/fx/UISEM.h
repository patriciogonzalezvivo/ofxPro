//
//  UISEM.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once

#include "UIClass.h"

class UISEM : public UIClass {
public:
    UISEM();
    
    void    setupUI();
    void    loadTexture(string _path);
    
    string  getClassName(){return "SEM";}
    ofTexture& getTexture();
    
    void    begin();
    void    end();
    
protected:
    void    guiEvent(ofxUIEventArgs &e);
    
    ofxUIRadio  *textures;
    ofTexture   texture;
    ofShader    shader;
};