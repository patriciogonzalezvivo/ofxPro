//
//  LeapHand.cpp
//  LeapMotion
//
//  Created by Patricio Gonzalez Vivo on 9/5/13.
//
//

#include "LeapHand.h"

bool LeapHand::operator==(const LeapHand &_otherHand) const {
  
    if( fingers.size() > 0 ){
        int machingFingers = 0;
        
        //  TODO Improve
        //
        for(int i = 0; i < fingers.size(); i++){
            for(int j = 0; j < _otherHand.fingers.size(); j++){
                if ( fingers[i].id == _otherHand.fingers[j].id ){
                    machingFingers++;
                    break;
                }
            }
        }
        
        return (machingFingers > 0)? true : false;
    } else if (palmPos.distance( _otherHand.palmPos ) < *palmSize){
        return true;
    } else {
        return false;
    }
}

bool LeapHand::operator==(const Leap::Hand &_otherHand) const {
    
    Leap::FingerList otherFingers = _otherHand.fingers();
    
    if( fingers.size() > 0 ){
        int machingFingers = 0;
        
        for(int i = 0; i < fingers.size(); i++){
            for(int j = 0; j < otherFingers.count() ; j++){
                if ( fingers[i].id == otherFingers[i].id() ){
                    machingFingers++;
                    break;
                }
            }
        }
        
        return (machingFingers > 0)? true : false;
    } else {
        return false;
    }
}

void LeapHand::lerp( LeapHand &_otherHand, float _amount){
    
    palmPos     += (_otherHand.palmPos - palmPos)*_amount;
    palmNormal  += (_otherHand.palmNormal - palmNormal)*_amount;
    
    direction   += (_otherHand.direction - direction)*_amount;
    
    sphereCenter += (_otherHand.sphereCenter - sphereCenter)*_amount;
    sphereRadius += (_otherHand.sphereRadius - sphereRadius)*_amount;
    
    life         = _otherHand.life;
    
    //  Learp maching fingers
    //
    for(int i = 0; i < fingers.size(); i++){
        fingers[i].bSync = false;
        
        for(int j = _otherHand.fingers.size()-1; j >= 0 ; j--){
            if ( fingers[i].id == _otherHand.fingers[j].id ){
                ofNotifyEvent(fingerMove, fingers[i], this);
                fingers[i].lerp( _otherHand.fingers[j], _amount );
                
                fingers[i].bSync = true;
                _otherHand.fingers.erase( _otherHand.fingers.begin()+j );
                break;
            }
        }
    }
    
    //  Add does that are new
    //
    for(int i = 0; i < _otherHand.fingers.size() ; i++){
        _otherHand.fingers[i].bSync = true;
        fingers.push_back( _otherHand.fingers[i] );
        ofNotifyEvent(fingerAdd, fingers[ fingers.size() ], this);
    }
    
    //  Delete everything that wasn't updated
    //
    for (int i = fingers.size()-1; i >= 0; i--) {
        if( !fingers[i].bSync ){
            ofNotifyEvent(fingerDel, fingers[i], this);
            fingers.erase(fingers.begin() + i);
        }
    }
}

void LeapHand::xeno( LeapHand &_otherHand, float _pct ){
    palmPos     = _pct * _otherHand.palmPos + (1.0-_pct) * palmPos;
    palmNormal  = _pct * _otherHand.palmNormal + (1.0-_pct) * palmNormal;
    
    direction   = _pct * _otherHand.direction + (1.0-_pct) * direction;
    
    sphereCenter = _pct * _otherHand.sphereCenter + (1.0-_pct) * sphereCenter;
    sphereRadius = _pct * _otherHand.sphereRadius + (1.0-_pct) * sphereRadius;
    
    life         = _otherHand.life;
    
    //  Learp maching fingers
    //
    for(int i = 0; i < fingers.size(); i++){
        fingers[i].bSync = false;
        
        for(int j = _otherHand.fingers.size()-1; j >= 0 ; j--){
            if ( fingers[i].id == _otherHand.fingers[j].id ){
                fingers[i].xeno( _otherHand.fingers[j], _pct );
                ofNotifyEvent(fingerMove, fingers[i], this);
                
                fingers[i].bSync = true;
                _otherHand.fingers.erase( _otherHand.fingers.begin()+j );
                break;
            }
        }
    }
    
    //  Add does that are new
    //
    for(int i = 0; i < _otherHand.fingers.size() ; i++){
        _otherHand.fingers[i].bSync = true;
        fingers.push_back(_otherHand.fingers[i]);
        ofNotifyEvent(fingerAdd, fingers[ fingers.size() ], this);
    }
    
    //  Delete everything that wasn't updated
    //
    for (int i = fingers.size()-1; i >= 0; i--) {
        if( !fingers[i].bSync ){
            ofNotifyEvent(fingerDel, fingers[i], this);
            fingers.erase(fingers.begin() + i);
        }
    }
}

void LeapHand::draw(){
    float phi = (1.0+sqrtf(5.0))/2.0;
    palmBase = palmPos+(direction*- *palmSize *2.0);
    
    ofPushStyle();
    ofSetColor( ofGetStyle().color, ofClamp(life,0.0, 1.0)*255 );
    
    ofPushMatrix();
    ofTranslate(palmPos);
    
    {
        ofPushMatrix();
        ofQuaternion q;
        q.makeRotate(ofPoint(0, -1, 0), palmNormal);
        ofMatrix4x4 m;
        q.get(m);
        glMultMatrixf(m.getPtr());
        ofNoFill();
        
        ofRotate(90, 1, 0, 0);
        ofCircle(0, 0, *palmSize );
        ofTranslate(0, 0, -10);
        ofCircle(0, 0, *palmSize *(phi-1.0));
        
        ofPopMatrix();
    }

    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(palmBase);
    rotateTo(direction);
    {
        ofPushMatrix();
        ofRotate(90, -1, 0, 0);
        ofCircle(0, 0, *palmSize * (1.0-phi));
        ofPopMatrix();
    }
    
    {
        ofTranslate(0, 100, 0);
        ofPushMatrix();
        ofRotate(90, -1, 0, 0);
        ofCircle(0, 0, *palmSize * (1.0-phi)*1.2 );
        ofPopMatrix();
    }
    
    ofPopMatrix();
    
    for(int i = 0; i < fingers.size(); i++){
        ofSetColor( ofGetStyle().color, ofClamp( fingers[i].life,0.0, 1.0)*255 );
        ofLine( fingers[i], fingers[i].end );
        ofLine( fingers[i].end, palmBase);
        
        ofPushMatrix();
        ofTranslate( fingers[i] );
        
//        ofPushStyle();
//        ofSetColor(255, 0, 0);
//        ofDrawBitmapString(ofToString( fingers[i].id ), 15,15);
//        ofPopStyle();
        
        rotateTo(fingers[i].dir);
        
        ofRotate(90, 1, 0, 0);
        ofCircle(0, 0, 3);
        
        float lenght = fingers[i].distance( palmBase );// fingers[i].end ) ;
        
        for (int i = 0; i < 2; i++){
            lenght -= lenght*(phi-1.0);
            ofPushMatrix();
            ofTranslate(0, 0, -lenght);
            ofCircle(0, 0, 5+(2-i)*2);
            ofPopMatrix();
        }
        
        ofPopMatrix();
    }
    
    ofPopStyle();
}

void LeapHand::rotateTo(ofPoint _dir){
    ofVec3f newTarget = _dir;
    newTarget.normalize();
    ofVec3f up( 0, -1, 0 );
    ofVec3f axis = up.crossed( newTarget );
    axis.normalize();
    float angle = up.angle( newTarget );
    ofRotate( angle, axis.x, axis.y, axis.z );
}
