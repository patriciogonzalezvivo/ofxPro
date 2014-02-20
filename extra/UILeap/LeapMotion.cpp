//
//  LeapMotion.cpp
//  LeapMotion
//
//  Created by Patricio Gonzalez Vivo on 9/5/13.
//
//

#include "LeapMotion.h"
#define STRINGIFY(A) #A

LeapMotion::LeapMotion(){
    
    xOffsetIn = 0;
    yOffsetIn = 0;
    zOffsetIn = 0;
    
    xOffsetOut = 0;
    yOffsetOut = 0;
    zOffsetOut = 0;
    
    xScale = 1;
    yScale = 1;
    zScale = 1;
    
    bEnable = false;
    bGestures = false;
    
    palmSize = 60;
    idCounter = 0;
}

LeapMotion::~LeapMotion(){
    if (gui != NULL){
        gui->disable();
        ofRemoveListener(gui->newGUIEvent, this, &LeapMotion::guiEvent);
    }
    
    close();
}

void LeapMotion::startGestures(){
    
    open();
    
    if (!bGestures){
        ourController->enableGesture(Gesture::TYPE_SCREEN_TAP, true);
        ourController->enableGesture(Gesture::TYPE_KEY_TAP, true);
        ourController->enableGesture(Gesture::TYPE_SWIPE, true);
        ourController->enableGesture(Gesture::TYPE_CIRCLE, true);
        
        bGestures = true;
    }
}

void LeapMotion::stopGestures(){
    if (bGestures){
        ourController->enableGesture(Gesture::TYPE_SCREEN_TAP, false);
        ourController->enableGesture(Gesture::TYPE_KEY_TAP, false);
        ourController->enableGesture(Gesture::TYPE_SWIPE, false);
        ourController->enableGesture(Gesture::TYPE_CIRCLE, false);
        
        bGestures = false;
    }
}

void LeapMotion::open(){
    
    if (!bOpen){
        shared_ptr<Leap::Controller> newControler( new Leap::Controller() );
        ourController = newControler;
        ourController->addListener(*this);
        bOpen = true;
    }
}

void LeapMotion::close(){
    
    if (bOpen){
        bOpen = false;
        ourMutex.lock();
        ourController->removeListener(*this);
        ourMutex.unlock();
    }
}

//-------------------------------------------------------------- Mapping
void LeapMotion::setMappingX(float minX, float maxX, float outputMinX, float outputMaxX){
    xOffsetIn	= minX;
    xOffsetOut	= outputMinX;
    xScale  =   ( outputMaxX - outputMinX ) / ( maxX - minX );
}

void LeapMotion::setMappingY(float minY, float maxY, float outputMinY, float outputMaxY){
    yOffsetIn	= minY;
    yOffsetOut	= outputMinY;
    yScale  =   ( outputMaxY - outputMinY ) / ( maxY - minY );
}
void LeapMotion::setMappingZ(float minZ, float maxZ, float outputMinZ, float outputMaxZ){
    zOffsetIn	= minZ;
    zOffsetOut	= outputMinZ;
    zScale  =   ( outputMaxZ - outputMinZ ) / ( maxZ - minZ );
}

//-------------------------------------------------------------- TOOLS
ofPoint LeapMotion::getofPoint(Vector v){
    return ofPoint(v.x, v.y, v.z);
}

ofPoint LeapMotion::getMappedofPoint( Vector v ){
    ofPoint p = getofPoint(v);
    p.x = xOffsetOut + (p.x - xOffsetIn) * xScale;
    p.y = yOffsetOut + (p.y - yOffsetIn) * yScale;
    p.z = zOffsetOut + (p.z - zOffsetIn) * zScale;
    
    return p;
}

//-------------------------------------------------------------- GUI
void LeapMotion::setupUI(){
    
    gui->addLabel("Calibration");
    gui->addSlider("Calibration_Elevation", 0, 200, &calibrationElevation);
    gui->addSlider("Calibration_X", 0, 500, &calibration.x);
    gui->addSlider("Calibration_Y", 0, 500, &calibration.y);
    gui->addSlider("Calibration_Z", 0, 500, &calibration.z);
    
    gui->addLabel("Tracking");
    gui->addSlider("Xenon",0.3,1.0,&trackXeno);
    
    gui->addLabel("Render");
//    gui->addSlider("Scale", 10.0, 100.0, &palmSize);
    gui->addSlider("Circle_Resolution", 3, 20, &circleResolution);

}

void LeapMotion::guiEvent(ofxUIEventArgs &e){
    
    if (gui != NULL){
        string name = e.widget->getName();
        
        if (name == "Calibration_X"){
            setMappingX(-calibration.x, calibration.x, -ofGetScreenWidth()*0.5, ofGetScreenWidth()*0.5);
        } else if (name == "Calibration_Elevation" || name == "Calibration_Y"){
            setMappingY(calibrationElevation, calibrationElevation+calibration.y, -ofGetScreenHeight()*0.5, ofGetScreenHeight()*0.5);
        } else if (name == "Calibration_Z"){
            setMappingZ(-calibration.z, calibration.z, -500, 500);
        }
    }
}

//-------------------------------------------------------------- LEAP EVENTS
void LeapMotion::onInit(const Controller& controller){
    ofLogVerbose("ofxLeapMotionApp - onInit");
}

//--------------------------------------------------------------
void LeapMotion::onConnect(const Controller& contr){
    ofLogWarning("ofxLeapMotionApp - onConnect");
}

//--------------------------------------------------------------
void LeapMotion::onDisconnect(const Controller& contr){
    ofLogWarning("ofxLeapMotionApp - onDisconnect");
}

//--------------------------------------------------------------
void LeapMotion::onExit(const Controller& contr){
    ofLogWarning("ofxLeapMotionApp - onExit");
}

void LeapMotion::onFrame(const Controller& contr){
    ofLogVerbose("ofxLeapMotionApp - onFrame");


    if(bEnable){
        ourMutex.lock();
        const Leap::Frame & frame = contr.frame();
        
        //  Update Hands
        //
        {
            const HandList & handList = frame.hands();
            
            vector<LeapHand> newHands;
            for(int i = 0; i < handList.count(); i++){
                
                if (handList[i].isValid() ){
                    LeapHand curHand;
                    
                    curHand.palmPos      = getMappedofPoint( handList[i].stabilizedPalmPosition() );// palmPosition() );
                    curHand.palmNormal   = getofPoint( handList[i].palmNormal() );
                    curHand.palmSize     = &palmSize;
                    curHand.direction    = getofPoint( handList[i].direction() );
                    curHand.sphereCenter = getMappedofPoint( handList[i].sphereCenter() );
                    curHand.sphereRadius = abs( curHand.palmPos.distance( curHand.sphereCenter ) );
                    curHand.life         = handList[i].timeVisible();
                    
                    for(int j = 0; j < handList[i].fingers().count(); j++){
                        const Finger & finger = handList[i].fingers()[j];
                        
                        if ( finger.isValid()){
                            LeapFinger f;
                            f.set ( getMappedofPoint( finger.stabilizedTipPosition() ) );// .tipPosition() );
                            f.vel = getMappedofPoint( finger.tipVelocity() );
                            f.dir = getofPoint( finger.direction() );
                            f.lenght = finger.length();
                            f.end =  ofPoint(f) + (f.dir * -f.lenght)*ofPoint(xScale,yScale,zScale) ;
                            f.life = finger.timeVisible();
                            
                            f.id  = finger.id();
                            
                            curHand.fingers.push_back( f );
                        }
                    }
                    
                    newHands.push_back( curHand );
                }
            }
            
            //  Lerp Maching Hands
            //
            for(int i = 0; i < hands.size(); i++){
                hands[i]->bSync = false;
                for(int j = newHands.size()-1; j >= 0; j--){
                    if ( *hands[i] == newHands[j]){
                        hands[i]->xeno(newHands[j],trackXeno);
                        ofNotifyEvent(handMove, *hands[i], this);
                        
                        hands[i]->bSync = true;
                        newHands.erase( newHands.begin() + j);
                        break;
                    }
                }
            }
            
            //  Add new Hands
            //
            for(int i = 0; i < newHands.size(); i++){
                newHands[i].id = idCounter;
                idCounter++;
                newHands[i].bSync = true;
                
                LeapHand *nHand = new LeapHand(newHands[i]);
                
                ofAddListener(nHand->fingerAdd, this, &LeapMotion::onFingerAdd);
                ofAddListener(nHand->fingerMove, this, &LeapMotion::onFingerMove);
                ofAddListener(nHand->fingerDel, this, &LeapMotion::onFingerDel);
                
                for (int j = 0; j < nHand->fingers.size(); j++) {
                    ofNotifyEvent(fingerAdd, nHand->fingers[j], this);
                }
                
                ofNotifyEvent(handAdd, *nHand, this);
                hands.push_back( nHand );
            }
            
            //  Delete those not sync
            //
            for (int i = hands.size()-1; i >= 0; i--) {
                if ( !hands[i]->bSync ){
                    ofNotifyEvent(handDel, *hands[i], this);
                    
                    ofRemoveListener(hands[i]->fingerAdd, this, &LeapMotion::onFingerAdd);
                    ofRemoveListener(hands[i]->fingerMove, this, &LeapMotion::onFingerMove);
                    ofRemoveListener(hands[i]->fingerDel, this, &LeapMotion::onFingerDel);
                    
                    for (int j = 0; j < hands[i]->fingers.size(); j++) {
                        ofNotifyEvent(fingerDel, hands[i]->fingers[j], this);
                    }
                    
                    delete hands[i];
                    hands.erase( hands.begin() + i);
                }
            }
        }
        
        //  Notify gestures
        //
        if (bGestures){
            Leap::GestureList gestures = frame.gestures();
            size_t numGestures = gestures.count();
            
            for (size_t i=0; i < numGestures; i++) {
                
                if (gestures[i].type() == Leap::Gesture::TYPE_SCREEN_TAP) {
                    Leap::ScreenTapGesture tap = gestures[i];
                    ofVec3f tapLoc = getMappedofPoint(tap.position());
                    
                    ofNotifyEvent(onScreenTap, tap, this);
                    
                } else if (gestures[i].type() == Leap::Gesture::TYPE_KEY_TAP) {
                    Leap::KeyTapGesture tap = gestures[i];
                    ofNotifyEvent(onKeyTap, tap, this);
                    
                } else if (gestures[i].type() == Leap::Gesture::TYPE_SWIPE) {
                    Leap::SwipeGesture swipe = gestures[i];
                    ofNotifyEvent(onSwipe, swipe, this);
                    
                } else if (gestures[i].type() == Leap::Gesture::TYPE_CIRCLE) {
                    Leap::CircleGesture circle = gestures[i];
                    ofNotifyEvent(onCircle, circle, this);
                }
            }
        }
        
        ourMutex.unlock();
    }
}

void LeapMotion::onFingerAdd( LeapFinger &_finger ){
    ofNotifyEvent(fingerAdd, _finger, this);
}

void LeapMotion::onFingerMove( LeapFinger &_finger ){
    ofNotifyEvent(fingerMove, _finger, this);
}

void LeapMotion::onFingerDel( LeapFinger &_finger ){
    ofNotifyEvent(fingerDel, _finger, this);
}

void LeapMotion::draw(){

    if(bEnable){
        ourMutex.lock();
        ofPushStyle();
        ofSetCircleResolution(circleResolution);
        
        for(int i = 0; i < hands.size(); i++){
            hands[i]->draw();
        }
        ofPopStyle();
        ourMutex.unlock();
    }
}