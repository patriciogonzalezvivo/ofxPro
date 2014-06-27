//
//  UILUT.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once
#include "UIShader.h"

class UILUT : public UIShader {
public:
    
    UILUT();
    
    void    setupUI();
    
    bool    load(string _path);
    bool    loadCube(string _path);
    
    string  getClassName(){return "LUT";}
    ofTexture& getCube();
    
    void    begin();
    void    end();
    
protected:
    void    guiEvent(ofxUIEventArgs &e);
    
    ofxUIRadio  *cubes;
    ofTexture   cube;
    
    string  vertexShader, fragmentShader;
};