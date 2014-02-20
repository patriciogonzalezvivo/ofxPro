#include "Spring.h"

//---------------------------------------------------------------------
Spring::Spring(){
	A = NULL;
	B = NULL;
    length = 0;
}

//---------------------------------------------------------------------
void Spring::update(float _speed){
    
    if ((A == NULL) || (B == NULL)){
		return;
	}
	
    float mutualDist = (*A - *B).length();
    float springForce = (k * (length - mutualDist));
    ofPoint frcToAdd = (*A - *B).normalized() * springForce;
    
    float mult = 1.0;
    if ( (A->bFix && !B->bFix) || (!A->bFix && B->bFix)){
        mult *=2.0;
    }
    
    if( A->bFix == false){
        A->addForce(frcToAdd*mult*_speed);
    }

    if( B->bFix == false){
        frcToAdd *= -1.0;
        B->addForce(frcToAdd*mult*_speed);
    }
}

//---------------------------------------------------------------------
void Spring::draw(){
	
	if ((A == NULL) || (B == NULL)){
		return;
	}
	
	ofLine(*A, *B);
}