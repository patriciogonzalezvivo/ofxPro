//
//  UIRpiCam.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/10/13.
//
//

#include "UIRpiCam.h"

UIRpiCam::UIRpiCam(){
    width = 2592;
    height = 1944;
    quality = 85;
    sharpness = 0;
    contrast = 0;
    brightness = 50;
    saturation = 0;
    rotation = 0;
    
    exposure = "auto";
    awb = "auto";
    imxfx = "none";
    
//    float   colfx;
//    float   metering;
    bHFlip = false;
    bVFlip = false;
    
    bLoading = false;
    ofRegisterURLNotification(this);
}

UIRpiCam::~UIRpiCam(){
    ofUnregisterURLNotification(this);
}

void UIRpiCam::setupUI(){
    gui->addSlider("width", 0.0, 2592, &width);
    gui->addSlider("height", 0.0, 2592, &height);
    gui->addSlider("quality", 0.0, 100, &quality);
    
    gui->addSlider("sharpness", -100, 100, &sharpness);
    gui->addSlider("contrast", -100, 100, &contrast);
    gui->addSlider("brightness", 0, 100, &brightness);
    gui->addSlider("saturation", -100, 100, &saturation);
    gui->addSlider("rotation",0,359,&rotation);
    
    gui->addToggle("hFlip", &bHFlip);
    gui->addToggle("vFlip", &bVFlip);
    
    gui->setTriggerWidgetsUponLoad(false);
    gui->addToggle("LOAD", &bLoading);
    
//    float   exposure;
//    float   awb;
//    float   imxfx;
}

void UIRpiCam::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if( name == "LOAD" ){
        if(bLoading){
            image.clear();
            string command = "http://rpi.local/rpicam/?width="+ofToString(width);
            command +="&height="+ofToString(height);
            command +="&quality="+ofToString(quality);
            command +="&sharpness="+ofToString(sharpness);
            command +="&contrast="+ofToString(contrast);
            command +="&brightness="+ofToString(brightness);
            command +="&saturation="+ofToString(saturation);
            command +="&rotation="+ofToString(rotation);
            command +="&exposure="+exposure;
            command +="&awb="+awb;
            if(bHFlip){
                command +="&-hflip=1";
            }
            if(bVFlip){
                command +="&-vflip=1";
            }
            ofLoadURLAsync(command,"rpiCam");
        }
    }
}

void UIRpiCam::urlResponse(ofHttpResponse & response){
    if(response.status==200 && response.request.name == "rpiCam"){
		image.loadImage(response.data);
		bLoading=false;
	}else{
		cout << response.status << " " << response.error << endl;
		if(response.status!=-1) bLoading=false;
	}
}

void UIRpiCam::setUseTexture(bool bUseTex){
    image.setUseTexture(bUseTex);
}

ofTexture& UIRpiCam::getTextureReference(){
    if (image.bAllocated()&&!bLoading&&bEnable) {
        return image.getTextureReference();
    } else {
        tmp.allocate(width,height);
        tmp.begin();
        ofClear(0,0);
        if (bLoading&&bEnable){
            ofDrawBitmapString("Loading...", width*0.5-30, height*0.5-4);
        } else if(!image.bAllocated()&&bEnable){
            ofDrawBitmapString("RPI-CAM", width*0.5-30, height*0.5-4);
        }
        tmp.end();
        return tmp.getTextureReference();
    }
}



