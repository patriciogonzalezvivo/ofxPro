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
    
    void        setupUI();
    void        draw();
    
protected:
	virtual void update(ofEventArgs & args);
    
    ofFbo       fbo;
    ofShader    shader;
    ofMesh      mesh;
    
    float       grainPct;
};