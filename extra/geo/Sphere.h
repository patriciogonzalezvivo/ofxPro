//
//  Sphere.h
//
//  Created by Patricio Gonzalez Vivo on 8/13/13.
//
//

#pragma once
#include "SpherePoint.h"

class Sphere : public ofSpherePrimitive{
public:
    
    Sphere();
    
    void draw();
    
    ofFloatColor    wireColor, solidColor;
    float           wireScale, solidScale;

protected:
    vector<SpherePoint> ripplePoints;
};

