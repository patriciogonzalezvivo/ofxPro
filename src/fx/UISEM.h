//
//  UISEM.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once

#include "UIShader.h"

class UISEM : public UIShader {
public:
    UISEM();
    
    void    setupUI();
    
    bool    load(string _path);
    void    loadTexture(string _path);
    
    string  getClassName(){return "SEM";}
    ofTexture& getTexture();
    
    void    begin();
    void    end();
    
protected:
    void    guiEvent(ofxUIEventArgs &e);
    
    ofxUIRadio  *textures;
    ofTexture   texture;
    
    string  vertexShader, fragmentShader;
};