//
//  Star.cpp
//  PrintMaking
//
//  Created by Patricio Gonzalez Vivo on 9/24/13.
//
//

#include "Star.h"

Star::Star(){
    connect = NULL;
    constelationAlpha = 0.1;
}

void Star::place( Sphere *_sphere, float _ra, float _dec ){
    float ra = _ra*(360.0/24.0)-180;
    float dec = _dec;
    
    ofQuaternion decRot, raRot;
    decRot.makeRotate( dec, 1, 0, 0);
    raRot.makeRotate( ra, 0, 1, 0);
    quaternion = decRot * raRot;
    
    this->set(quaternion * ofPoint(0,0, _sphere->getRadius() ));
}

void Star::draw(){
    ofPushStyle();
    
//    ofSetColor(ofFloatColor(1.0,ofNoise(x,y,z*ofGetElapsedTimef()*0.005)*1.5 ));
//    glBegin(GL_POINTS);
//    glVertex3f(x,y,z);
//    glEnd();
    
    if (connect != NULL && constelationAlpha != 0.0 ){
        ofSetColor(ofFloatColor(0.8,constelationAlpha));
        ofLine(*this, *connect);
    }
    
    ofPopStyle();
}