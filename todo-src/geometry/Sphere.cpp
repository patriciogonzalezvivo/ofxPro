//
//  Sphere.cpp
//  DV
//
//  Created by Patricio Gonzalez Vivo on 8/13/13.
//
//

#include "Sphere.h"
Sphere::Sphere(){
    wireScale = 0;
    solidScale = 1.0;
    
    wireColor.set(1.0);
    solidColor.set(1.0);
    
    radius = 100;
};

void Sphere::draw(){
    
    ofPushStyle();
    
    if (wireScale != 0 || wireColor.a != 0){
        ofPushMatrix();
        ofNoFill();
        ofSetColor(wireColor);
        ofScale(wireScale, wireScale,wireScale);
        ofSpherePrimitive::draw();
        ofPopMatrix();
    }
    
    if(solidScale != 0 || solidColor.a != 0){
        ofPushMatrix();
        ofFill();
        ofSetColor(solidColor);
        ofScale(solidScale, solidScale,solidScale);
        ofSpherePrimitive::draw();
        ofPopMatrix();
    }
    
    ofPopStyle();
}