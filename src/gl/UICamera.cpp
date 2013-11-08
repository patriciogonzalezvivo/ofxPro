//
//  UICamera.cpp
//  example
//
//  Created by Patricio Gonzalez Vivo on 11/4/13.
//
//

#include "UICamera.h"

UICamera::UICamera(){
    FOV = 60;
    
    camera = new ofEasyCam();
    camera->setFov(FOV);
    ((ofEasyCam*)camera)->setDistance(200);
    type = "easyCam";
    
    locations = NULL;
    bSaveLocation = false;
    
    ofAddListener(ofEvents().update, this, &UICamera::update);
}

void UICamera::setupUI(){
    gui->addSlider("FOV", 0, 180, &FOV);
    gui->addSlider("Speed", 0.0, 1.0, &speed);
    gui->addButton("SAVE", &bSaveLocation);
    gui->addSpacer();
    
    vector<string> clear;
    locations = gui->addRadio("LOCATIONS", clear);
}

void UICamera::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name == "FOV"){
        camera->setFov(FOV);
    } else if(name == "SAVE"){
        if(bSaveLocation){
            string locationName = ofSystemTextBoxDialog("Save Location as");
            if(locationName.length()){
                addLocation(locationName);
            }
        }
    } else if (name == "LOCATIONS"){
        
    } else if (name == "ENABLE"){
        
    } else if (name == "FOV"){
        
    } else if (name == "Speed"){
        
    } else {
        ofxUIToggle *t = (ofxUIToggle*)e.widget;
        if (t != NULL){
            if (t->getValue()){
                if(targetLocation.load(pathToLocations+name+".cam")){
                    pct = 1.0;
                }
            }
        }
    }
}

void UICamera::loadLocations(string _path){
    pathToLocations = _path;
    
    if(locations!=NULL){
        locations->clearEmbeddedWidgets();
        
        ofDirectory dir(pathToLocations);
        if(dir.exists()){
            dir.listDir();
            int total = dir.getFiles().size();
            for (int i = 0; i < total; i++) {
                ofxUIToggle *t = gui->addToggle(dir.getFiles()[i].getBaseName(),false);
                locations->addToggle(t);
            }
        } else {
            dir.createDirectory(pathToLocations);
        }
    }
}

void UICamera::addLocation(string _name){
    CameraLocation now = getCameraLocation();
    if( now.save(pathToLocations+_name+".cam") ){
        ofxUIToggle *t = gui->addToggle(_name,false);
        locations->addToggle(t);
        gui->autoSizeToFitWidgets();
    }
}

void UICamera::setCameraLocation( const CameraLocation &_camPos ){
    targetLocation = _camPos;
    pct = 1.0;
}

CameraLocation UICamera::getCameraLocation(){
    CameraLocation rta;
    if(type == "easyCam"){
        rta.FOV      = ((ofEasyCam*)camera)->getFov();
        rta.distance = ((ofEasyCam*)camera)->getDistance();
        rta.position = ((ofEasyCam*)camera)->getPosition();
        rta.orientation = ((ofEasyCam*)camera)->getOrientationQuat();
    } else {
        rta.distance = 0;
        rta.FOV      = camera->getFov();
        rta.position = camera->getPosition();
        rta.orientation = camera->getOrientationQuat();
    }
    return rta;
}

void UICamera::enableMouseInput(){
    if(type == "easyCam"){
        ((ofEasyCam*)camera)->enableMouseInput();
    }
}

void UICamera::disableMouseInput(){
    if(type == "easyCam"){
        ((ofEasyCam*)camera)->disableMouseInput();
    }
}

void UICamera::update(ofEventArgs& args){
    if(bEnable){
        if(pct>0.1){
            FOV = camera->getFov()*(pct)+targetLocation.FOV*(1.0-pct);
            camera->setFov( FOV );
            camera->setPosition(camera->getPosition().getInterpolated(targetLocation.position, 1.0-pct) );
            ofQuaternion q;
            q.slerp(1.0-pct,camera->getOrientationQuat(),targetLocation.orientation);
            camera->setOrientation(q);
            
            pct *= (1.0- powf(10.0, (1.0-speed)*-3.0 ) );
        }
    }
}

bool UICamera::save(string _savePath){
    CameraLocation noteCamLoc = getCameraLocation();
    noteCamLoc.save(_savePath);
}

bool UICamera::load(string _loadPath){
    CameraLocation noteCamLoc = getCameraLocation();
    noteCamLoc.load(_loadPath);
    
    if (targetLocation != noteCamLoc ){
        pct = 1.0;
    }
    targetLocation = noteCamLoc;
}

void UICamera::billboard(){
    ofVec3f objectLookAt = ofVec3f(0.0,0.0,1.0);
    ofVec3f objToCam = camera->getGlobalPosition();
    objToCam.normalize();
    float theta = objectLookAt.angle(objToCam);
    
    ofVec3f axisOfRotation = objToCam.crossed(objectLookAt);
    axisOfRotation.normalize();
    
    glRotatef(-theta, axisOfRotation.x, axisOfRotation.y, axisOfRotation.z);
}