//
//  UIMatCap.h
//
//  Copyright (C) 2014 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once

#include "UIShader.h"

class UIMatCap : public UIShader {
public:
    UIMatCap();
    
    void    setupUI();
    
    void    loadNormal(string _path);
    void    loadMaterial(string _path);
    
    string  getClassName();
    
    void    begin();
    void    end();
    
    ofTexture   matCap,normal;
    bool        bNormals;
    
protected:
    void    guiEvent(ofxUIEventArgs &e);
};