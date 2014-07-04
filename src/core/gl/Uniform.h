//
//  Uniform.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
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