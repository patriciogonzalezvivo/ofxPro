//
//  aColor.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once
#include "ofMain.h"
#include "aPoint.h"

class aColor : public ofFloatColor{
public:
    
    aColor(){
        r = 0.0;
        g = 0.0;
        b = 0.0;
        hue=0.0;
        sat=0.0;
        bri=0.0;
        alpha=1.0;
    };
    virtual ~aColor(){};
    
    virtual void set( ofFloatColor const & _color ){
        hue = _color.getHue();
        sat = _color.getSaturation();
        bri = _color.getBrightness();
        alpha = _color.a;
    }
    
    virtual void update(){
        ofPoint HSBTarget(hue,sat,bri);
        
        if (HSB!=HSBTarget || a != alpha){
            HSB.goTo(HSBTarget,0.09);
            setHsb(HSB.x, HSB.y, HSB.z);
            a += (alpha - a)*0.01;
        }
    }
    
    float   hue,sat,bri;
    float   alpha;
    
protected:
    aPoint    HSB;
};