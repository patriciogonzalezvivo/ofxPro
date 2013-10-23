/*
 *  Particle.cpp
 *
 *  Created by Patricio González Vivo on 24/01/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#include "Particle.h"

Particle::Particle(){
    this->set(0,0,0);
    vel.set(0,0,0);
    acc.set(0,0,0);
	
	
    color.set(1);
    tailLength = 0;
    tailGradient = false;
}

ofPoint Particle::getVel(){
    return vel;
}

void Particle::addForce(ofPoint _force){
    acc += _force;
};

void Particle::addForceTo(ofPoint _target, bool _slowdown){
    acc += steer(_target, _slowdown);
}

void Particle::addRepulsionForce(ofPoint posOfForce, float radius, float scale){
    
	// ----------- (2) calculate the difference & length
	ofPoint diff	= *this - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
        diff.normalize();
        acc += diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addRepulsionForce(Particle *p, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(*p);
	
	// ----------- (2) calculate the difference & length
	ofVec2f diff	= *this - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc += diff * scale * pct;
		p->acc -= diff * scale * pct;
    }
}

void Particle::addAttractionForce(ofPoint posOfForce, float radius, float scale){
	
	// ----------- (2) calculate the difference & length
	ofPoint diff	= *this - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addAttractionForce(Particle *p, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(*p);
	
	// ----------- (2) calculate the difference & length
	ofPoint diff	= *this - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc -= diff * scale * pct;
		p->acc += diff * scale * pct;
    }
	
}

ofPoint Particle::steer(ofPoint target, bool arrival) {
    ofPoint steer;
	
	if (arrival){
		ofPoint desired = target - *this;
		float d = desired.length();
        
		if (d > 0) {
			desired.normalize();
			if (d < 1) 
				desired *= (d); // This damping is somewhat arbitrary
			//else 
			//desired *= 2;
			// Steering = Desired minus Velocity
			steer = desired - vel; //PVector.sub(desired,vel);
			//steer.limit(0.9);  // Limit to maximum steering force
		} else
			steer = ofPoint(0,0,0);

	} else {
		steer = target - *this;
		//steer.limit(maxforce);
	}
	
    return steer;
}

//------------------------------------------------------------
void Particle::addClockwiseForce(Particle *p, float radius, float scale){
	
	// ----------- (1) make a vector of where this Particle p is:
	ofPoint posOfForce;
	posOfForce.set(*p);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
        addForce(ofPoint(diff.y * scale * pct * -1,
                         diff.x * scale * pct));
        p->addForce(ofPoint(diff.y * scale * pct,
                            diff.x * scale * pct * -1));
    }
}

//------------------------------------------------------------
void Particle::addCounterClockwiseForce(Particle *p, float radius, float scale){
	
	// ----------- (1) make a vector of where this Particle p is:
	ofPoint posOfForce;
	posOfForce.set(*p);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
        addForce(ofPoint(diff.y * scale * pct,
                         diff.x * scale * pct * -1));
        p->addForce(ofPoint(diff.y * scale * pct * -1,
                            diff.x * scale * pct));
    }
}

void Particle::update(float _speed){
	
    //  Update Physics
    //
    vel += acc;
    vel *= _speed;
    *this += vel;
    acc *= 0;
    
    //  Add Tail
    //
    if (tailLength > 0){
        tail.push_back(*this);
        while (tail.size() > tailLength) {
            tail.erase(tail.begin());
        }
    }
}

void Particle::draw(){
    ofPushStyle();
    
    if (tailLength > 0){
        if (tailGradient){
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            for (int i = 0; i < tail.size(); i++){
                float alpha = ofMap(i+1, 1,tail.size(), 0.0, 0.9);
                
                mesh.addColor(ofFloatColor(color, alpha) );
                mesh.addVertex(tail[i]);
            }
            ofFill();
            ofSetColor( 255 );
            mesh.draw();
        } else {
            ofSetColor(color);
            ofNoFill();
            ofBeginShape();
            for(int i = 0; i < tail.size(); i++){
                ofVertex(tail[i]);
            }
            ofEndShape();
        }
        
    }
    
    {
        ofSetColor(color);
//        glBegin(GL_POINTS);
//        glNormal3f(1, 0, 0);
//        glVertex3f(x,y,z);
//        glEnd();
    }
    
    ofPopStyle();
}