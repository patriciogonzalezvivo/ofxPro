//
//  Cursor.cpp
//
//  Based on Elliot Woods http://www.kimchiandchips.com
//  From https://github.com/elliotwoods/ofxGrabScene/blob/master/src/ofxGrabScene/Cursor.cpp
//
//

#include "Cursor.h"

//----------
Cursor::Cursor() {
    memset(this->button, 0, 3);
    this->captured = false;
    this->worldValid = false;
}

//----------
void Cursor::draw() const {
    ofPushMatrix();
    ofTranslate(world);
    ofDrawBox(0.1);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    ofScale(0.1f, 0.1f, 0.1f);
    ofPopMatrix();
    
    stringstream message;
    message << "screen " << screen << endl;
    message << "world " << world << endl;
    
    ofDrawBitmapString(message.str(), ofPoint());
}

//----------
MovingCursor::MovingCursor() {
    this->lastUpdate = 0;
}

//----------
void MovingCursor::start(int button) {
    this->startTime = ofGetElapsedTimef();
    this->startScreen = screen;
    this->startWorld = world;
    this->button[button] = true;
}

//----------
void MovingCursor::end(int button) {
    this->button[button] = false;
}

//----------
void MovingCursor::draw() const {
    Cursor::draw();
    ofLine(this->world, this->lastFrame.world);
}

//----------
ofVec2f MovingCursor::getScreenFrameDifference() const {
    return this->screen - this->lastFrame.screen;
}

//----------
ofVec2f MovingCursor::getScreenVelocity() const {
    return this->getScreenFrameDifference() / ofGetLastFrameTime();
}

//----------
ofVec3f MovingCursor::getNormalisedScreenVelocity() const {
    ofVec3f v = this->getScreenVelocity();
    v.z = 0.0f;
    v.x /= ofGetWidth() / 2.0f;
    v.y /= ofGetHeight() / 2.0f;
    return v;
}

//----------
ofVec3f MovingCursor::getWorldFrameDifference() const {
    if (this->worldValid && this->lastFrame.worldValid)
        return this->world - this->lastFrame.world;
    else
        return ofVec3f();
}

//----------
ofVec3f MovingCursor::getWorldVelocity() const {
    return this->getWorldFrameDifference() / ofGetLastFrameTime();
}

//----------
float MovingCursor::getActionAmplitude() const {
    return (this->getScreenVelocity() / ofVec2f(ofGetScreenWidth(), ofGetScreenHeight())).dot(ofVec2f(1.0f, -1.0f));
}