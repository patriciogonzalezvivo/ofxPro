//
//  aColor.h
//
//  Created by Patricio Gonzalez Vivo on 11/5/13.
//
//

#pragma once
#include "ofMain.h"
#include "Particle.h"

class aColor : public ofFloatColor{
public:
    
    aColor():speed(0.1){};
    virtual ~aColor(){};
    
    virtual void set( ofFloatColor const & _color ){
        hue = _color.getHue();
        sat = _color.getSaturation();
        bri = _color.getBrightness();
        bChange = true;
    }
    
    virtual void update(){
//        if(bChange){
            ofPoint HSBTarget(hue,sat,bri);
            
            if (HSB!=HSBTarget){
                HSB.addForceTo(HSBTarget,true);
                HSB.update(speed);
                setHsb(HSB.x, HSB.y, HSB.z);
                bChange = true;
            } else {
                bChange = false;
            }
//        }
    }
    
    float       hue,sat,bri;
    float       speed;
    
protected:
    Particle    HSB;
    
    bool    bChange;
};
