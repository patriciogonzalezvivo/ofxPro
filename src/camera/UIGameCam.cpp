//
//  UIGameCam.cpp
//
//  Based on https://github.com/Flightphase/ofxGameCamera
//
//

#include "UIGameCam.h"

UIGameCam::UIGameCam(){
    FOV = 60;
    
    camera = new ofCamera();

    textField = NULL;
    locations = NULL;
    newLocationName = "";
    bNewLocation = false;
    
	sensitivityX = 0.15f;
	sensitivityY = 0.15f;
	speed = 10.0f;
	
	invertControls = false;
	bArrowKeys = false;
	applyRotation = true;
	applyTranslation = true;
    
	rollSpeed = 0.1;
	justResetAngles = false;
    
    bReset = false;
    
    reset();
    
    ofAddListener(ofEvents().update, this, &UIGameCam::update);
}

UIGameCam::~UIGameCam(){
    ofRemoveListener(ofEvents().update, this, &UIGameCam::update);
    
    if(camera != NULL){
        delete camera;
        camera = NULL;
    }
}

void UIGameCam::setupUI(){
    gui->addSlider("FOV", 0, 180, &FOV);
    gui->addSlider("Lerp", 0.0, 1.0, &lerpPct);
    gui->addSlider("Moving_Speed", 0.0, 1.0, &speed);
    gui->addSlider("Rolling_Speed", 0.0, 1.0, &rollSpeed);
    gui->addToggle("Use_Arrow_Keys", &bArrowKeys);
    gui->addToggle("Reset", &bReset);
    
    gui->addSpacer();
    textField = gui->addTextInput("ADD", "", OFX_UI_FONT_SMALL);
    gui->setTriggerWidgetsUponLoad(false);
    vector<string> clear;
    locations = gui->addRadio("LOCATIONS", clear);
}


void UIGameCam::update(ofEventArgs& args){
    if(bEnable&&camera!=NULL){
        
        if(bReset){
            reset();
            bReset = false;
        }
        
        bool rotationChanged = false;
        bool positionChanged = false;
        
        //lastRot == lastRot << this is nananers check
        if( (lastRot == lastRot && lastRot != camera->getOrientationQuat()) ||
           (lastPos == lastPos && lastPos != camera->getPosition())){
            movedManually();
        }
        
        if(applyTranslation){
            int multiplier = invertControls ? -1 : 1;
            if(ofGetKeyPressed('w') || (bArrowKeys && ofGetKeyPressed(OF_KEY_UP)) ){
                camera->dolly(-speed);
                positionChanged = true;
            }
            
            if(ofGetKeyPressed('s') || (bArrowKeys && ofGetKeyPressed(OF_KEY_DOWN)) ){
                camera->dolly(speed);
                positionChanged = true;
            }
            
            if(ofGetKeyPressed('a') || (bArrowKeys && ofGetKeyPressed(OF_KEY_LEFT)) ){
                camera->truck(-speed);
                positionChanged = true;
            }
            
            if(ofGetKeyPressed('d') || (bArrowKeys && ofGetKeyPressed(OF_KEY_RIGHT)) ){
                camera->truck(speed);
                positionChanged = true;
            }
            
            if(ofGetKeyPressed('c') || (bArrowKeys && ofGetKeyPressed(OF_KEY_PAGE_DOWN)) ){
                camera->boom(-speed*multiplier);
                positionChanged = true;
            }
            
            if(ofGetKeyPressed('e') || (bArrowKeys && ofGetKeyPressed(OF_KEY_PAGE_UP)) ){
                camera->boom(speed*multiplier);
                positionChanged = true;
            }
        }
        
        
        if(positionChanged){
            currLookTarget = camera->getPosition() + camera->getLookAtDir();
        }
        
        ofVec2f mouse( ofGetMouseX(), ofGetMouseY() );
        ofVec2f lastMouse(ofGetPreviousMouseX(),ofGetPreviousMouseY());
        
        if(!justResetAngles){
            if(bMouse && applyRotation && ofGetMousePressed(0)){
                
                float dx = (mouse.x - lastMouse.x) * sensitivityX;
                float dy = (mouse.y - lastMouse.y) * sensitivityY;
                
                currLookTarget.rotate(-dx, camera->getPosition(), currentUp);
                ofVec3f sideVec = (currentUp).getCrossed(currLookTarget - camera->getPosition());
                camera->lookAt(currLookTarget, currentUp);
                
                currLookTarget.rotate(dy, camera->getPosition(), sideVec);
                currentUp = ( currLookTarget - camera->getPosition()  ).getCrossed(sideVec);
                camera->lookAt(currLookTarget, currentUp);
                
                rotationChanged = true;
            }
            
            if(applyRotation){
                if(ofGetKeyPressed('r')){
                    currentUp.rotate(rollSpeed,camera->getLookAtDir());
                    camera->lookAt(currLookTarget, currentUp);
                    rotationChanged = true;
                }
                if(ofGetKeyPressed('q')){
                    currentUp.rotate(-rollSpeed,camera->getLookAtDir());
                    camera->lookAt(currLookTarget, currentUp);
                    rotationChanged = true;
                }
            }
        }
        
        justResetAngles = false;
        lastRot = camera->getOrientationQuat();
        lastPos = camera->getPosition();

        //  If it's move from outside
        //
        if(pct>0.1){
            FOV = camera->getFov()*(pct)+targetLocation.FOV*(1.0-pct);
            camera->setFov( FOV );
            camera->setPosition(camera->getPosition().getInterpolated(targetLocation.position, 1.0-pct) );
            ofQuaternion q;
            q.slerp(1.0-pct,camera->getOrientationQuat(),targetLocation.orientation);
            camera->setOrientation(q);
            pct *= (1.0- powf(10.0, (1.0-speed)*-3.0 ) );
        }
        
        if(bNewLocation){
            addLocation(newLocationName);
            newLocationName = "";
            bNewLocation = false;
        }
    }
}

void UIGameCam::movedManually(){
	currLookTarget = camera->getPosition() + camera->getLookAtDir();
	currentUp = camera->getUpDir();
	justResetAngles = true;
}

void UIGameCam::reset(){
    
	currentUp = ofVec3f(0,1,0);
	currLookTarget = ofVec3f(0,0,1);
    
    camera->setPosition(ofVec3f(0,0,0));
    camera->setOrientation(ofQuaternion());
	
	movedManually();
}