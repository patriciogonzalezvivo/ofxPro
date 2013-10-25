//
//  SuperParticle.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/15/13.
//
//

#pragma once
#include "Particle.h"

class SuperParticle : public Particle {
public:
    
    SuperParticle();
    virtual ~SuperParticle(){};
    
    void    addNoise(float _angle, float _turbulence);
    void	applyGravityForceTo(ofPoint *_pos, float _pct);
    void	applyFlockingForce(ofPoint *_offSet, float _neighbordhood, float _independece);
    void    applyAttractionToNodes(float _pct);
    
    ofPoint localOffset;
};