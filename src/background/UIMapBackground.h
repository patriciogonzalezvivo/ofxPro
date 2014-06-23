//
//  UIMapBackground.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once

#include "UIBackground.h"

class UIMapBackground : public UIBackground {
public:
    
    UIMapBackground();
    ~UIMapBackground();
    
    void    getBackgroundMesh(ofMesh& _mesh, ofTexture& _image, float _width, float _height);
    
    void    draw();
    float   exp;
    
protected:
    virtual void setupUI();
	virtual void update(ofEventArgs & args);
    virtual void windowResized(ofResizeEventArgs & args);
    virtual void guiEvent(ofxUIEventArgs &e);
    
    ofxUIRadio  *backgrounds;
    
    ofShader    shader;
    ofMesh      mesh;
    ofTexture   texture;
    
    bool        bMeshReady;
};