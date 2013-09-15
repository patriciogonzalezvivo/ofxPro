/*
 *  Particle.h
 *
 *  Created by Patricio González Vivo on 24/01/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"

class Particle : public ofPoint {
public:
	Particle();
    
    ofPoint getVel();
    
    void    addForce(ofPoint _force);
    void    addForceTo( ofPoint _target, bool _slowdown = false);
    
    void    addRepulsionForce(ofPoint p, float radius, float scale);
    void    addRepulsionForce(Particle *p, float radius, float scale);
    
    void    addAttractionForce(ofPoint p, float radius, float scale);
    void    addAttractionForce(Particle *p, float radius, float scale);
    
    void    addClockwiseForce(Particle *p, float radius, float scale);
    void    addCounterClockwiseForce(Particle *p, float radius, float scale);

    void    update(float _speed);
    void    draw();
    
    ofFloatColor    color;
    
    vector<ofPoint> tail;
    int             tailLength;
    bool            tailGradient;
    
protected:
    ofPoint	steer(ofPoint _target, bool _slowdown);
    
    ofPoint vel,acc;
};
