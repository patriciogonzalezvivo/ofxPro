#ifndef SPRING
#define SPRING

#include "ofMain.h"
#include "Particle.h"

class Spring {
public:

    Spring();
		
    void update(float _speed = 1.0);
    void draw();
    
    Particle * A;
    Particle * B;
		
    float k;	 // this is the k, Springiness constant
    float length;
};

#endif