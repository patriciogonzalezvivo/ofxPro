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
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
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
            
            ofPoint _steer;
            ofPoint desired = _target - *this;
            float d = desired.length();
            if (d > 0.001) {
                if (d < 1.0)
                    desired *= (d);
                
                _steer = desired - vel;
                acc += _steer;
                update(_speed);
            } else {
                _steer = ofPoint(0,0,0);
                acc *= 0;
                vel *= 0;
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