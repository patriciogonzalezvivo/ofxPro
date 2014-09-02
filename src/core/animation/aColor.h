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
    
    aColor():
    hue(0.0f),sat(0.0f),bri(0.0),alpha(0.0f),
    HSB(0.0f,0.0f,0.0f)
    {
        r = 0.0;
        g = 0.0;
        b = 0.0;
        a = 1.0;
    };
    
    aColor(const float &_r, const float &_g, const float &_b, const float &_a):
    hue(0.0f),sat(0.0f),bri(0.0),alpha(0.0f),
    HSB(0.0f,0.0f,0.0f)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    };
    
    virtual void set( ofFloatColor const & _color ){
        hue = _color.getHue();
        sat = _color.getSaturation();
        bri = _color.getBrightness();
        alpha = _color.a;
    }
    
    virtual void update(){
        float speed = 0.09;
        
        ofPoint HSBTarget(hue,sat,bri);
        if (HSB!=HSBTarget||a != alpha){
            HSB.goTo(HSBTarget,speed);
            setHsb(HSB.x, HSB.y, HSB.z,a+(alpha - a)*speed);
        }
    }
    
    float   hue,sat,bri;
    float   alpha;
    
protected:
    aPoint    HSB;
};
