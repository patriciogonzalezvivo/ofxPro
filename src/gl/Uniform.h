//
//  Uniform.h
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/20/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxUI.h"

enum UniformType{
	UNIFORM_VEC3,
    UNIFORM_VEC2,
    UNIFORM_FLOAT
};

struct Uniform {
    Uniform(UniformType _type, string _name){
        type = _type;
        name = _name;
        bUpdated = false;
        bNeedUI = true;
    }
    
    UniformType type;
    string      name;
    ofVec3f     value;
    
    bool        bUpdated;
    bool        bNeedUI;
};