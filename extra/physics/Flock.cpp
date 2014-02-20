/*
 *  Flock.cpp
 *  flock
 *
 *  Copyright 2010 Patricio Gonzalez Vivo http://www.patriciogonzalezvivo.com
 *	All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/

#include "Flock.h"

Flock::Flock() {
	color.r = 255;
	color.g = 255;
	color.b = 255;
	flocking = true;
	//loaded = false;
    
    
}

void Flock::setup(int _size, int _nBoids){
    space.xLeft = -_size;
    space.xRight = _size;
    space.yTop = -_size;
    space.yBottom = _size;
    space.zFront = -_size;
    space.zBack = _size;
    
    nBoids = _nBoids;
    
    boids = new (nothrow) Boid[nBoids];
    
    for (int i = 0; i < nBoids; i++){
        boids[i] = Boid();
        boids[i].setNBoids(&nBoids);
        boids[i].setColor(&color);
        boids[i].setSpace(&space);
        boids[i].loc.set(ofRandom(space.xLeft, space.xRight),ofRandom(space.yTop, space.yBottom),ofRandom(space.zFront,space.zBack));
        boids[i].setFlockingState(&flocking);
    }
}

void Flock::update() {
	for (int i = 0; i < nBoids; i++)
		boids[i].update(boids);
}

void Flock::draw() {
	for (int i = 0; i < nBoids; i++)
		boids[i].draw();
		//boids[i].draw(ofMap(boids[i].loc.z,0,-4000,255,10));
}

bool Flock::isOver(int x, int y){
	bool overSomething = false;
	
	for (int i = 0; i < nBoids; i++){
		if (boids[i].isOver(x,y) ) 
			overSomething = true;
	}
	
	return overSomething;
}