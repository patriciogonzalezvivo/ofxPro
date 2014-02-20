//
//  LeapHand.h
//  LeapMotion
//
//  Created by Patricio Gonzalez Vivo on 9/5/13.
//
//

#pragma once

#include "ofMain.h"

#include "Leap.h"
#include "LeapFinger.h"

class LeapHand {
public:
    
    bool    operator==(const LeapHand &_otherHand) const;
    bool    operator==(const Leap::Hand &_otherHand) const;
    
    void    lerp( LeapHand &_otherHand, float _amount );
    void    xeno( LeapHand &_otherHand, float _pct );
    void    draw();
    
    vector <LeapFinger>  fingers;
    
    ofPoint palmPos;
    ofPoint palmNormal;
    ofPoint palmBase;
    
    float   *palmSize;

    ofPoint direction;
    ofPoint sphereCenter;
    float   sphereRadius;
    float   life;
    
    int64_t id;
    
    bool    bSync;
    
    ofEvent<LeapFinger> fingerAdd;
    ofEvent<LeapFinger> fingerMove;
    ofEvent<LeapFinger> fingerDel;
    
protected:
    void rotateTo(ofPoint _dir);
    
};
