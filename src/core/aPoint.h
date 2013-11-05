//
//  aPoint.h
//
//  Created by Patricio Gonzalez Vivo on 11/5/13.
//
//

#pragma once
#include "ofMain.h"

class aPoint : public ofPoint {
public:
	aPoint(){};
    virtual ~aPoint(){};
    
    virtual void goTo(const ofPoint &_target, float _speed = 0.1){
        if( ((ofPoint)*this) != _target){
            acc += steer(_target, true);
            update(_speed);
        }
    }
    
    virtual void update(const float &_speed){
        vel += acc;
        vel *= _speed;
        *this += vel;
        acc *= 0;
    }
    
protected:
    ofPoint steer(const ofPoint &_target, const bool &_slowdown){
        ofPoint _steer;
        
        if (_slowdown){
            ofPoint desired = _target - *this;
            float d = desired.length();
            
            if (d > 0.0) {
                if (d < 1.0)
                    desired *= (d);
                
                _steer = desired - vel;
            } else
                _steer = ofPoint(0,0,0);
            
        } else {
            _steer = _target - *this;
        }
        
        return _steer;
    }
    
    ofPoint vel,acc;
};