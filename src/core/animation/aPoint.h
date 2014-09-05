//
//  aPoint.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once
#include "ofMain.h"

class aPoint : public ofPoint {
public:
    
    aPoint():
    vel(0.0f,0.0f,0.0f),
    acc(0.0f,0.0f,0.0f)
    {
        x = y = z = 0.1f;
    }
    
    aPoint(const float &_x, const float &_y, const float &_z):
    vel(0.0f,0.0f,0.0f),
    acc(0.0f,0.0f,0.0f)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    
    virtual void goTo(const ofPoint &_target, float _speed = 0.1){
        if( ((ofPoint)*this) != _target){
            
            ofPoint desired = _target - (ofPoint)*this;
            float d = desired.length();
            if (d > 0.001) {
                if (d < 1.0)
                    desired *= (d);
                acc += desired - vel;
                update(_speed);
            } else {
                acc *= 0.;
                vel *= 0.;
                set(_target);
            }
        }
    }
    
    virtual void update(const float &_speed){
        vel += acc;
        vel *= _speed;
        *this += vel;
        acc *= 0;
    }
    
protected:
    ofPoint vel,acc;
};