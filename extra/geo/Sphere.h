//
//  Sphere.h
//
//  Created by Patricio Gonzalez Vivo on 8/13/13.
//
//

#pragma once
#include "Location.h"

class Sphere : public ofSpherePrimitive{
public:
    
    Sphere();
    
    void draw();
    
    ofFloatColor    wireColor, solidColor;
    float           wireScale, solidScale;
};

