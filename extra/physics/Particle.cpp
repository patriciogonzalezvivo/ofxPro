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
	
	size = 0;
    color.set(1);
    tailLength = 0;
    tailGradient = false;
    
    bFix = false;
}

ofPoint Particle::getVel(){
    return vel;
}

void Particle::addSlowdonForceTo(ofPoint _target, float _scale, bool _normal){
    ofPoint _steer;
    ofPoint desired = _target - *this;
    float d = desired.length();
    
    if (_normal)
        desired.normalize();
    
    if (d > 0.0) {
        if (d < 1.0)
            desired *= (d);
        
        _steer = desired - vel;
    } else
        _steer = ofPoint(0,0,0);

    acc += _steer*_scale;
};

void Particle::addRepulsionForce(ofPoint _posOfForce, float _radius, float _scale){
    
	// ----------- (2) calculate the difference & length
	ofPoint diff	= *this - _posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	bool bAmCloseEnough = true;
    if (_radius > 0){
        if (length > _radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
	if (bAmCloseEnough == true){
		float pct = 1 - (length / _radius);  // stronger on the inside
        diff.normalize();
        acc += diff * _scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addRepulsionForce(Particle *p, float _scale){
    addRepulsionForce(p, (size+ p->size)*2.5, _scale);
}

void Particle::addRepulsionForce(Particle *p, float _radius, float _scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(*p);
	
	// ----------- (2) calculate the difference & length
	ofVec2f diff	= *this - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	bool bAmCloseEnough = true;
    if (_radius > 0){
        if (length > _radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
	if (bAmCloseEnough == true){
		float pct = 1 - (length / _radius);  // stronger on the inside
		diff.normalize();
		acc += diff * _scale * pct;
		p->acc -= diff * _scale * pct;
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
	if (bAmCloseEnough){
		float pct = 1.0 - (length / radius);  // stronger on the inside
//		diff.normalize();
		acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addAttractionForce(Particle *p, float _scale){
    addAttractionForce(p, (size+p->size)*2.5 ,_scale);
}

void Particle::addAttractionForce(Particle *p, float _radius, float _scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(*p);
	
	// ----------- (2) calculate the difference & length
	ofPoint diff	= *this - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	bool bAmCloseEnough = true;
    if (_radius > 0){
        if (length > _radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
	if (bAmCloseEnough == true){
		float pct = 1 - (length / _radius);  // stronger on the inside
		diff.normalize();
		acc -= diff * _scale * pct;
		p->acc += diff * _scale * pct;
    }
	
}

//------------------------------------------------------------
void Particle::addClockwiseForce(Particle *p, float _scale){
    addClockwiseForce(p, size*2.0 + p->size*2.0, _scale );
}

void Particle::addClockwiseForce(Particle *p, float _radius, float _scale){
	
	// ----------- (1) make a vector of where this Particle p is:
	ofPoint posOfForce;
	posOfForce.set(*p);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (_radius > 0){
        if (length > _radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / _radius);  // stronger on the inside
		diff.normalize();
        addForce(ofPoint(diff.y * _scale * pct * -1,
                         diff.x * _scale * pct));
        p->addForce(ofPoint(diff.y * _scale * pct,
                            diff.x * _scale * pct * -1));
    }
}

//------------------------------------------------------------
void Particle::addCounterClockwiseForce(Particle *p, float _radius, float _scale){
	
	// ----------- (1) make a vector of where this Particle p is:
	ofPoint posOfForce;
	posOfForce.set(*p);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (_radius > 0){
        if (length > _radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / _radius);  // stronger on the inside
		diff.normalize();
        addForce(ofPoint(diff.y * _scale * pct,
                         diff.x * _scale * pct * -1));
        p->addForce(ofPoint(diff.y * _scale * pct * -1,
                            diff.x * _scale * pct));
    }
}

void Particle::update(float _speed){
    aPoint::update(_speed);
    
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
        
        if (size > 0){
            ofCircle(*this, size);
        } else {
#ifdef TARGET_OPENGLES
        
#else
        glBegin(GL_POINTS);
        glNormal3f(1, 0, 0);
        glVertex3f(x,y,z);
        glEnd();
        
#endif
        }
    }
    
    ofPopStyle();
}