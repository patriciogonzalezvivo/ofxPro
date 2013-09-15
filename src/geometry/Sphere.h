//
//  Sphere.h
//
//  Created by Patricio Gonzalez Vivo on 8/13/13.
//
//

#pragma once
#include "ofMain.h"

class SpherePoint: public ofPoint {
    ofQuaternion getQuaternion(){return quaternion;};
    
protected:
    ofQuaternion quaternion;
};

class Sphere : public ofSpherePrimitive{
public:
    
    Sphere();
    
    void draw();
    
    ofFloatColor    wireColor, solidColor;
    float           wireScale,solidScale;

protected:
    vector<SpherePoint> ripplePoints;
};

