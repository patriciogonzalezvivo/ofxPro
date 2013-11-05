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
    
    float       grainPct;
    
protected:
	virtual void update(ofEventArgs & args);
    
    ofFbo       fbo;
    ofShader    shader;
    ofMesh      mesh;
};