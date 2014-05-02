//
//  SuperParticle.cpp
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/15/13.
//
//

#include "SuperParticle.h"

SuperParticle::SuperParticle(){
    localOffset.set(ofRandom(1.0f),ofRandom(1.0f),ofRandom(1.0f));
}

void SuperParticle::addNoise(float _angle, float _turbulence){
    
    float angle = ofSignedNoise( x * 0.005f, y * 0.005f) * _angle;
    
    ofPoint noiseVector( cos( angle ), sin( angle ) );
    
    acc += noiseVector * _turbulence;
}

void SuperParticle::applyFlockingForce(ofPoint * _offset, float _neighbordhood, float _independece){
	acc += ofVec3f(ofNoise(x / _neighbordhood + _offset->x + localOffset.x * _independece,
                           y / _neighbordhood,
                           z / _neighbordhood)
                   - .5,
                   ofNoise(
                           x / _neighbordhood,
                           y / _neighbordhood + _offset->y  + localOffset.y * _independece,
                           z / _neighbordhood)
                   - .5,
                   ofNoise(
                           x / _neighbordhood,
                           y / _neighbordhood,
                           z / _neighbordhood + _offset->z + localOffset.z * _independece)
                   - .5);
}

void SuperParticle::applyFlockingForce(ofPoint * _offset, float _neighbordhood, float _independece, float _scale){
	acc += ofVec3f(ofNoise(x / _neighbordhood + _offset->x + localOffset.x * _independece,
                           y / _neighbordhood,
                           z / _neighbordhood)
                   - .5,
                   ofNoise(
                           x / _neighbordhood,
                           y / _neighbordhood + _offset->y  + localOffset.y * _independece,
                           z / _neighbordhood)
                   - .5,
                   ofNoise(
                           x / _neighbordhood,
                           y / _neighbordhood,
                           z / _neighbordhood + _offset->z + localOffset.z * _independece)
                   - .5)*_scale;
}