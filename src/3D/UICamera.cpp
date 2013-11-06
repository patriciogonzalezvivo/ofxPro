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
        
    } else {
        if(targetLocation.load(pathToLocations+name+".xml")){
            pct = 1.0;
        }
    }
}

void UICamera::addLocation(string _name){
    CameraLocation now = getCameraLocation();
    if( now.save(pathToLocations+_name+".xml") ){
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
        rta.distance = ((ofEasyCam*)camera)->getDistance();
        rta.position = ((ofEasyCam*)camera)->getPosition();
        rta.orientation = ((ofEasyCam*)camera)->getOrientationQuat();
    } else {
        rta.distance = 0;
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
            if(type=="easyCam"){
//                ((ofEasyCam*)camera)->setTarget(ofPoint(0,0,0));
//                ((ofEasyCam*)camera)->setDistance(((ofEasyCam*)camera)->getDistance()*(1.0-pct) + targetLocation.distance*pct);
            }
            camera->setPosition(camera->getPosition().getInterpolated(targetLocation.position, 1.0-pct) );
            ofQuaternion q;
            q.slerp(1.0-pct,camera->getOrientationQuat(),targetLocation.orientation);
            camera->setOrientation(q);
            
            pct *= (1.0- powf(10.0, (1.0-speed)*-3.0 ) );
        }
    }
}

static bool saveOfCam(ofCamera &cam, string savePath){
    ofBuffer buffer;
    buffer.append("--------------ofCamera parameters--------------\n");
    buffer.append("transformMatrix\n" + ofToString(cam.getGlobalTransformMatrix()) + "\n" );
    buffer.append("fov\n" + ofToString(cam.getFov())+"\n");
    buffer.append("near\n" + ofToString(cam.getNearClip())+"\n");
    buffer.append("far\n" + ofToString(cam.getFarClip())+"\n");
    buffer.append("lensOffset\n" + ofToString(cam.getLensOffset())+"\n");
#ifndef USE_DEVELOP_BRANCH
    buffer.append("forceAspectRatio\n" + ofToString(cam.getForceAspectRatio())+"\n");
    buffer.append("aspectRatio\n" + ofToString(cam.getAspectRatio()) + "\n");
#endif
    buffer.append("isOrtho\n" + ofToString(cam.getOrtho()) + "\n");
    
    if(ofBufferToFile(savePath, buffer)){
        ofLogNotice("ofCamera saved successfully!");
        return true;
    }else{
        ofLogWarning("failed to save ofCamera!");
        return false;
    }
    
}

static bool loadOfCam(ofCamera &cam, string loadPath){
    ofFile file(loadPath);
	
	if(!file.exists()){
		ofLogError("The file " + loadPath + " is missing");
        return false;
	}
	float aRatio;
    bool bForceAspect =false;
	ofBuffer buffer(file);
	while (!buffer.isLastLine()) {
		string line = buffer.getNextLine();
        if (line == "transformMatrix") {
            string str =buffer.getNextLine() + "\n";
            str += buffer.getNextLine() + "\n";
            str += buffer.getNextLine() + "\n";
            str += buffer.getNextLine();
            
            ofMatrix4x4 m;
            istringstream iss;
            iss.str(str);
            iss >> m;
            cam.setTransformMatrix(m);
        }else if(line == "fov"){
            cam.setFov(ofToFloat(buffer.getNextLine()));
        }else if(line == "near"){
            cam.setNearClip(ofToFloat(buffer.getNextLine()));
        }else if(line == "far"){
            cam.setFarClip(ofToFloat(buffer.getNextLine()));
        }else if(line == "lensOffset"){
            vector<string> vals = ofSplitString(buffer.getNextLine(), ", ");
            if (vals.size()==2) {
                cam.setLensOffset(ofVec2f(ofToFloat(vals[0]), ofToFloat(vals[1])));
            }
#ifndef USE_DEVELOP_BRANCH
        }else if(line == "forceAspectRatio"){
            bForceAspect = ofToBool(buffer.getNextLine());
            cam.setForceAspectRatio(bForceAspect);
        }else if(line == "aspectRatio"){
#endif
            aRatio = ofToFloat(buffer.getNextLine());
        }else if(line == "isOrtho"){
            if(ofToBool(buffer.getNextLine())){
                cam.enableOrtho();
            }else{
                cam.disableOrtho();
            }
        }
	}
    if (bForceAspect){
        cam.setAspectRatio(aRatio);
    }
    
    return true;
    
}

bool UICamera::save(string _savePath){
    
    if (camera != NULL){
        if(saveOfCam((*camera), _savePath)){
            
            if(type == "easyCam"){
                ofBuffer buffer = ofBufferFromFile(_savePath);
                buffer.append("--------------ofEasyCam parameters--------------\n");
                buffer.append("target\n" + ofToString(((ofEasyCam*)camera)->getTarget().getPosition()) + "\n" );
                buffer.append("bEnableMouseMiddleButton\n" + ofToString(((ofEasyCam*)camera)->getMouseMiddleButtonEnabled())+"\n");
                buffer.append("bMouseInputEnabled\n" + ofToString(((ofEasyCam*)camera)->getMouseInputEnabled())+"\n");
                buffer.append("drag\n" + ofToString(((ofEasyCam*)camera)->getDrag())+"\n");
                buffer.append("doTranslationKey\n" + ofToString(((ofEasyCam*)camera)->getTranslationKey())+"\n");
                if(ofBufferToFile(_savePath, buffer)){
                    ofLogNotice("ofEasyCam saved successfully!");
                    return true;
                }else{
                    ofLogWarning("failed to save ofEasyCam!");
                    return false;
                }
            }
            
        }else{
            return false;
        }
    }
}

bool UICamera::load(string _loadPath){
    if (camera != NULL){
        
        if(type == "easyCam"){
            ((ofEasyCam*)camera)->reset();
        }
        
        if(loadOfCam(*camera, _loadPath)){
            if(type == "easyCam"){
                
                ofBuffer buffer = ofBufferFromFile(_loadPath);
                while (!buffer.isLastLine()) {
                    string line = buffer.getNextLine();
                    
                    if (line == "target") {
                        vector<string> vals = ofSplitString(buffer.getNextLine(), ", ");
                        if (vals.size()==3) {
                            ((ofEasyCam*)camera)->getTarget().setPosition(ofVec3f(ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2])));
                        }
                    }
                    else if(line == "drag"){
                        ((ofEasyCam*)camera)->setDrag(ofToFloat(buffer.getNextLine()));
                    }else if(line == "bEnableMouseMiddleButton"){
                        if(ofToBool(buffer.getNextLine())){
                            ((ofEasyCam*)camera)->enableMouseMiddleButton();
                        }else{
                            ((ofEasyCam*)camera)->disableMouseMiddleButton();
                        }
                    }else if(line == "bMouseInputEnabled"){
                        if(ofToBool(buffer.getNextLine())){
                            ((ofEasyCam*)camera)->enableMouseInput();
                        }else{
                            ((ofEasyCam*)camera)->disableMouseInput();
                        }
                    }else if(line == "doTranslationKey"){
                        ((ofEasyCam*)camera)->setTranslationKey(ofToChar(buffer.getNextLine()));
                    }
                }
                return true;
            }else {
                return false;
            }
        }
    }
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