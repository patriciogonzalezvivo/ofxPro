//
//  PostRender.h
//  LeapMotion
//
//  Created by Patricio Gonzalez Vivo on 9/6/13.
//
//

#pragma once

#include "UIClass.h"

class PostRender : public UIClass {
public:
    
    PostRender();
    
    string getClassName(){return "POST-RENDER"; }
    void setupUI();
    
    void begin();
    void end();
    
protected:
    
    void        guiEvent(ofxUIEventArgs &e);
    
    //  Post
    //
    ofShader    shader;
    ofTexture   texture;
    float       chromaDist;
    float       displacement;
    float       grainFreq;
    float       grainPct;
    float       bEnable;
};