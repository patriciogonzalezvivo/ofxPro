//
//  UIGradientLayer.h
//
//  Created by Patricio Gonzalez Vivo on 9/20/13.
//
//

#pragma once

#include "UIBackground.h"

class UISuperBackground : public UIBackground {
public:

    UISuperBackground();
    
    void        draw();
    
    float       value;
    
protected:
    virtual void setupUI();
	virtual void update(ofEventArgs & args);
    
    ofFbo       fbo;
    ofShader    shader;
    ofMesh      mesh;
};