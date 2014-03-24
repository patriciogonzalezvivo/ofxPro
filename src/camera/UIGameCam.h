//
//  UIGameCam.h
//
//  Created by Patricio Gonzalez Vivo on 3/16/14.
//
//

#pragma once

#include "UICamera.h"


class UIGameCam : public UICamera {
public:
    
    UIGameCam();
    virtual ~UIGameCam();
    
    void    setupUI();
    
    string  getClassName(){return "GAMECAM";};
    
    //adjusting threw code manually
	void movedManually(); //call this whenever you update orientation or position manually
    void reset(); //sets the camera back to zero
    
protected:
    void    update(ofEventArgs& args);
    
    ofVec3f currentUp;
	ofVec3f currLookTarget;
	
	ofQuaternion lastRot;
	ofVec3f lastPos;
    
	void updateRotation();
    
	bool justResetAngles;
    
    //tweak controls
	bool invertControls;
	float sensitivityX;
	float sensitivityY;
	float rollSpeed;
	float speed;
	
	//enable, disable
	bool bArrowKeys;
	bool applyRotation;
	bool applyTranslation;
    bool bReset;
};