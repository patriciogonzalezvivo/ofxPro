//
//  UILUT.h
//
//  Created by Patricio Gonzalez Vivo on 2/9/14.
//
//

#pragma once
#include "UIClass.h"

class UILUT : public UIClass {
public:
    
    UILUT();
    
    void    setupUI();
    void    loadCube(string _path);
    
    string  getClassName(){return "LUT";}
    ofTexture& getCube();
    
    void    begin();
    void    end();
    
protected:
    void    guiEvent(ofxUIEventArgs &e);
    
    ofxUIRadio  *cubes;
    ofTexture   cube;
    ofShader    shader;
};