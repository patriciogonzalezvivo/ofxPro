//
//  Star.h
//  PrintMaking
//
//  Created by Patricio Gonzalez Vivo on 9/24/13.
//
//

#pragma once
#include "Sphere.h"

class Star : public ofPoint {
public:
    
    Star();
    void    place( Sphere *_sphere, float _ra, float _dec );

    ofQuaternion getQuaternion(){ return quaternion; };
    
	void	draw();
    
    string  constelationName;
    float   constelationAlpha;
    
    Star    *connect;
    
protected:
    ofQuaternion quaternion;
};