//
//  UIEasyCam.cpp
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#include "UIEasyCam.h"

UIEasyCamera::UIEasyCamera(){
    FOV = 60;
    
    camera = new ofEasyCam();
    camera->setFov(FOV);
    ((ofEasyCam*)camera)->setDistance(200);;
    
    textField = NULL;
    locations = NULL;
    newLocationName = "";
    bNewLocation = false;
    
    ofAddListener(ofEvents().update, this, &UIEasyCamera::update);
}

UIEasyCamera::~UIEasyCamera(){
    ofRemoveListener(ofEvents().update, this, &UIEasyCamera::update);
    
    if(camera != NULL){
        delete ((ofEasyCam*)camera);
        camera = NULL;
    }
}

CameraLocation UIEasyCamera::getCameraLocation(){
    CameraLocation rta;
    rta.FOV      = ((ofEasyCam*)camera)->getFov();
    rta.distance = ((ofEasyCam*)camera)->getDistance();
    rta.position = ((ofEasyCam*)camera)->getPosition();
    rta.orientation = ((ofEasyCam*)camera)->getOrientationQuat();
    return rta;
}

void UIEasyCamera::enableMouseInput(){
    bMouse = true;
    ((ofEasyCam*)camera)->enableMouseInput();
}

void UIEasyCamera::disableMouseInput(){
    bMouse = false;
    ((ofEasyCam*)camera)->disableMouseInput();
}

void UIEasyCamera::update(ofEventArgs& args){
    UICamera::update(args);
}