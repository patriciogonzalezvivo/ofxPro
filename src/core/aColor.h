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
    
    aColor(){
        r = 0.0;
        g = 0.0;
        b = 0.0;
        hue=0.0;
        sat=0.0;
        bri=0.0;
    };
    virtual ~aColor(){};
    
    virtual void set( ofFloatColor const & _color ){
        hue = _color.getHue();
        sat = _color.getSaturation();
        bri = _color.getBrightness();
    }
    
    virtual void update(){
        ofPoint HSBTarget(hue,sat,bri);
        
        if (HSB!=HSBTarget){
            HSB.goTo(HSBTarget,0.09);
            setHsb(HSB.x, HSB.y, HSB.z);
        }
    }
    
    float       hue,sat,bri;
    
protected:
    Particle    HSB;
};
