//
//  UISEM.h
//
//  Created by Patricio Gonzalez Vivo on 5/7/14.
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