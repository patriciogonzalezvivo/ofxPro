//
//  SpherePoint.h
//  PrintMaking
//
//  Created by Patricio Gonzalez Vivo on 9/24/13.
//
//

#pragma once
#include "ofMain.h"

class SpherePoint: public ofPoint {
public:
    ofQuaternion getQuaternion(){ return quaternion; };
    
protected:
    ofQuaternion quaternion;
};