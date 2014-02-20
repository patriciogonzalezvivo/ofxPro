/*
 *  Particle.h
 *
 *  Created by Patricio González Vivo on 24/01/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */
#pragma once
#include "aPoint.h"

class Particle : public aPoint {
public:
	Particle();
    virtual ~Particle(){};
    
    ofPoint getVel();
    
    void    addForce(ofPoint _force){acc += _force;}
    void    addSlowdonForceTo(ofPoint _target, float _scale, bool _normal = true);
    void    addRepulsionForce(ofPoint p, float radius, float scale);
    void    addRepulsionForce(Particle *p, float radius, float scale);
    void    addRepulsionForce(Particle *p, float scale);
    
    void    addAttractionForce(ofPoint p, float radius, float scale);
    void    addAttractionForce(Particle *p, float radius, float scale);
    void    addAttractionForce(Particle *p, float scale);
    
    void    addClockwiseForce(Particle *p, float radius, float scale);
    void    addClockwiseForce(Particle *p, float scale);
    
    void    addCounterClockwiseForce(Particle *p, float radius, float scale);
    void    addCounterClockwiseForce(Particle *p, float scale);

    void    update(float _speed);
    void    draw();
    
    ofFloatColor    color;
    
    float           size;
    
    vector<ofPoint> tail;
    int             tailLength;
    bool            tailGradient;
    
    bool    bFix;
};
