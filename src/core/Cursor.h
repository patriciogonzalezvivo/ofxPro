//
//  Cursor.h
//
//  Based on Elliot Woods http://www.kimchiandchips.com
//  From https://github.com/elliotwoods/ofxGrabScene/blob/master/src/ofxGrabScene/Cursor.h
//
//

#pragma once
#include "ofMain.h"

//----------
class Cursor3D {
public:
    Cursor3D();
    ofVec2f screen;
    ofVec3f world;
    bool worldValid;
    bool button[3];
    bool captured;
    
    virtual void draw() const;
};

//----------
class MovingCursor : public Cursor3D {
public:
    MovingCursor();
    
    void start(int button);
    void end(int button);
    
    void draw() const;
    
    float startTime; ///< time of birth
    ofVec2f startScreen;
    ofVec3f startWorld;
    Cursor3D lastFrame;
    int lastUpdate;
    bool dragged;
    
    ofVec3f worldViewFrameDifference;
    ofVec3f viewFrameDifference;
    
    float getAge() const;
    ofVec2f getScreenFrameDifference() const;
    ofVec3f getWorldFrameDifference() const;
    ofVec2f getScreenVelocity() const;
    ofVec3f getNormalisedScreenVelocity() const;
    ofVec3f getWorldVelocity() const;
    float getActionAmplitude() const;
};