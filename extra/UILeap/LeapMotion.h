//
//  LeapMotion.h
//
//  Created by Patricio Gonzalez Vivo on 9/5/13.
//  
//

/* Note on OS X, you must have this in the Run Script Build Phase of your project.
 where the first path ../../../addons/ofxUILeap/ is the path to the ofxLeapMotion addon.
 
 cp -f ../../../addons/ofxLeapMotion/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
 
 If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib
 */

#pragma once

#include "ofMain.h"
#include "Leap.h"
#include "Poco/Mutex.h"

#include "LeapHand.h"

#include "UIClass.h"

using namespace Leap;

class LeapMotion : public Listener, public UIClass {
public:
    LeapMotion();
    virtual ~LeapMotion();
    
    string  getClassName(){return "LEAP"; };
    
    void    startGestures();
    void    stopGestures();
    
    void    resetMapping();
    void    setMappingX(float minX, float maxX, float outputMinX, float outputMaxX);
    void    setMappingY(float minY, float maxY, float outputMinY, float outputMaxY);
    void    setMappingZ(float minZ, float maxZ, float outputMinZ, float outputMaxZ);
    
    ofPoint getofPoint(Vector v);
    ofPoint getMappedofPoint( Vector v );
    
    void    open();
    void    close();
    
    void    draw();
    
    vector< LeapHand* > hands;
    
    //  Gestures
    //
    ofEvent< Leap::ScreenTapGesture >   onScreenTap;
    ofEvent< Leap::KeyTapGesture >      onKeyTap;
    ofEvent< Leap::SwipeGesture >       onSwipe;
    ofEvent< Leap::CircleGesture >      onCircle;
    
    //  Hands
    //
    ofEvent<LeapHand>   handAdd;
    ofEvent<LeapHand>   handMove;
    ofEvent<LeapHand>   handDel;
    
    //  Fingers
    //
    ofEvent<LeapFinger> fingerAdd;
    ofEvent<LeapFinger> fingerMove;
    ofEvent<LeapFinger> fingerDel;
    
protected:
    virtual void setupUI();
    virtual void guiEvent(ofxUIEventArgs &e);
    
    void onInit(const Controller& controller);
    void onConnect(const Controller& contr);
    void onFrame(const Controller& contr);
    void onDisconnect(const Controller& contr);
    void onExit(const Controller& contr);
    
    
    
    void onFingerAdd(LeapFinger &_finger);
    void onFingerMove(LeapFinger &_finger);
    void onFingerDel(LeapFinger &_finger);
    
    shared_ptr<Leap::Controller> ourController;
    Poco::FastMutex     ourMutex;
    
    ofPoint calibration;
    float   calibrationElevation;
    float   xOffsetIn, xOffsetOut, xScale;
    float   yOffsetIn, yOffsetOut, yScale;
    float   zOffsetIn, zOffsetOut, zScale;
    float   trackXeno, trackLerp;
    
    int64_t idCounter;
    
    float   palmSize;
    float   circleResolution;
    
    bool    bGestures, bOpen;
};