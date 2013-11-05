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
}

void UICamera::setupUI(){
    
    gui->addSlider("FOV", 0, 180, &FOV);
    
    gui->addSpacer();
    vector<string> views;
    views.push_back("TOP");
    views.push_back("BOTTOM");
    views.push_back("FRONT");
    views.push_back("BACK");
    views.push_back("RIGHT");
    views.push_back("LEFT");
    views.push_back("3D");
    views.push_back("DISABLE");
    
    ofxUIDropDownList *ddl = gui->addDropDownList("VIEW", views);
    ddl->setAutoClose(false);
    ddl->setShowCurrentSelected(true);
    ddl->activateToggle("DISABLE");
    
    gui->autoSizeToFitWidgets();
}

void UICamera::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name == "FOV"){
        camera->setFov(FOV);
    }
//    else if(name == "ROT-X"){
//        xRot->setPosAndHome(xRot->getPos());
//    } else if(name == "ROT-Y") {
//        yRot->setPosAndHome(yRot->getPos());
//    } else if(name == "ROT-Z"){
//        zRot->setPosAndHome(zRot->getPos());
//    } else if(name == "RESET"){
//        ofxUIButton *b = (ofxUIButton *) e.widget;
//        if(b->getValue()){
//            camera.reset();
//        }
//    }else if(name == "TOP"){
//        ofxUIToggle *t = (ofxUIToggle *) e.widget;
//        if(t->getValue()){
//            xRot->setHome(0);
//            yRot->setHome(0);
//            zRot->setHome(0);
//        }
//    } else if(name == "BOTTOM"){
//        ofxUIToggle *t = (ofxUIToggle *) e.widget;
//        if(t->getValue()){
//            xRot->setHome(-180);
//            yRot->setHome(0);
//            zRot->setHome(0);
//        }
//    } else if(name == "BACK"){
//        ofxUIToggle *t = (ofxUIToggle *) e.widget;
//        if(t->getValue())
//        {
//            xRot->setHome(-90);
//            yRot->setHome(0);
//            zRot->setHome(180);
//        }
//    } else if(name == "FRONT"){
//        ofxUIToggle *t = (ofxUIToggle *) e.widget;
//        if(t->getValue()){
//            xRot->setHome(-90);
//            yRot->setHome(0);
//            zRot->setHome(0);
//        }
//    } else if(name == "RIGHT"){
//        ofxUIToggle *t = (ofxUIToggle *) e.widget;
//        if(t->getValue())
//        {
//            xRot->setHome(-90);
//            yRot->setHome(0);
//            zRot->setHome(90);
//        }
//    } else if(name == "LEFT") {
//        ofxUIToggle *t = (ofxUIToggle *) e.widget;
//        if(t->getValue()){
//            xRot->setHome(-90);
//            yRot->setHome(0);
//            zRot->setHome(-90);
//            
//        }
//    } else if(name == "3D") {
//        ofxUIToggle *t = (ofxUIToggle *) e.widget;
//        if(t->getValue())
//        {
//            xRot->setHome(-70);
//            yRot->setHome(0);
//            zRot->setHome(45);
//        }
//    }
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

bool UICamera::save(string savePath){
    if (camera != NULL){
        if(saveOfCam((*camera), savePath)){
            
            if(type == "easyCam"){
                ofBuffer buffer = ofBufferFromFile(savePath);
                buffer.append("--------------ofEasyCam parameters--------------\n");
                buffer.append("target\n" + ofToString(((ofEasyCam*)camera)->getTarget().getPosition()) + "\n" );
                buffer.append("bEnableMouseMiddleButton\n" + ofToString(((ofEasyCam*)camera)->getMouseMiddleButtonEnabled())+"\n");
                buffer.append("bMouseInputEnabled\n" + ofToString(((ofEasyCam*)camera)->getMouseInputEnabled())+"\n");
                buffer.append("drag\n" + ofToString(((ofEasyCam*)camera)->getDrag())+"\n");
                buffer.append("doTranslationKey\n" + ofToString(((ofEasyCam*)camera)->getTranslationKey())+"\n");
                if(ofBufferToFile(savePath, buffer)){
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

bool UICamera::load(string loadPath){
    if (camera != NULL){
        
        if(type == "easyCam"){
            ((ofEasyCam*)camera)->reset();
        }
        
        if(loadOfCam(*camera, loadPath)){
            if(type == "easyCam"){
                
                ofBuffer buffer = ofBufferFromFile(loadPath);
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