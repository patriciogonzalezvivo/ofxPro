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
    
    host = "rpi.local";

    bHFlip = false;
    bVFlip = false;
    
    bLoading = false;
    ofRegisterURLNotification(this);
}

UIRpiCam::~UIRpiCam(){
    ofUnregisterURLNotification(this);
}

void UIRpiCam::setupUI(){
    gui->addLabel("RaspberryPi Host");
    textField = gui->addTextInput("HOST", "", OFX_UI_FONT_SMALL);
    
    gui->addSpacer();
    gui->addSlider("width", 0.0, 2592, &width);
    gui->addSlider("height", 0.0, 2592, &height);
    gui->addSlider("quality", 0.0, 100, &quality);
    
    gui->addSlider("sharpness", -100, 100, &sharpness);
    gui->addSlider("contrast", -100, 100, &contrast);
    gui->addSlider("brightness", 0, 100, &brightness);
    gui->addSlider("saturation", -100, 100, &saturation);
    gui->addSpacer();
    gui->addSlider("rotation",0,359,&rotation);
    gui->addSpacer();
    
    vector<string> expo;
    expo.push_back("EXPO_OFF");
    expo.push_back("EXPO_AUTO");
    expo.push_back("EXPO_NIGH");
    expo.push_back("EXPO_NIGHT_PREVIEW");
    expo.push_back("EXPO_BLACK_LIGHT");
    expo.push_back("EXPO_SPOT_LIGHT");
    expo.push_back("EXPO_SPORT");
    expo.push_back("EXPO_SNOW");
    expo.push_back("EXPO_BEACH");
    expo.push_back("EXPO_VERY_LONG");
    expo.push_back("EXPO_FIXED_FPS");
    expo.push_back("EXPO_ANTI_SHAKE");
    expo.push_back("EXPO_FIREWORKS");
    gui->addDropDownList("EXPO", expo)->setAutoClose(true);
    
    vector<string> awb_opt;
    awb_opt.push_back("AWB_OFF");
    awb_opt.push_back("AWB_AUTO");
    awb_opt.push_back("AWB_SUN");
    awb_opt.push_back("AWB_CLOUD");
    awb_opt.push_back("AWB_SHADE");
    awb_opt.push_back("AWB_TUNGSTEN");
    awb_opt.push_back("AWB_FLUORESCENT");
    awb_opt.push_back("AWB_INCADESCENT");
    awb_opt.push_back("AWB_FLASH");
    awb_opt.push_back("AWB_HORIZON");
    gui->addDropDownList("AWB", awb_opt)->setAutoClose(true);
    
    vector<string> fx;
    fx.push_back("FX_NONE");
    fx.push_back("FX_NEGATIVE");
    fx.push_back("FX_SOLARISE");
    fx.push_back("FX_SKETCH");
    fx.push_back("FX_DENOISE");
    fx.push_back("FX_EMBOS");
    fx.push_back("FX_OILPAINT");
    fx.push_back("FX_HATCH");
    fx.push_back("FX_GPEN");
    fx.push_back("FX_PASTEL");
    fx.push_back("FX_WATERCOLOUR");
    fx.push_back("FX_FILM");
    fx.push_back("FX_BLUR");
    fx.push_back("FX_SATURATION");
    fx.push_back("FX_COLOURSWAP");
    fx.push_back("FX_WASHEDOUT");
    fx.push_back("FX_POSTERISE");
    fx.push_back("FX_COLOURPOINT");
    fx.push_back("FX_COLOURBALANCE");
    fx.push_back("FX_CARTOON");
    gui->addDropDownList("FX", fx)->setAutoClose(true);
    
    gui->addSpacer();
    gui->addToggle("hFlip", &bHFlip);
    gui->addToggle("vFlip", &bVFlip);
    gui->addSpacer();
    gui->setTriggerWidgetsUponLoad(false);
    gui->addToggle("LOAD", &bLoading);
}

void UIRpiCam::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if( name == "LOAD" ){
        if(bLoading){
            load();
        }
    } else if (name == "HOST"){
        host = textField->getTextString();
    } else if(name == "EXPO_OFF"){
        setExposure(EXPO_OFF);
    } else if(name == "EXPO_AUTO"){
        setExposure(EXPO_AUTO);
    } else if(name == "EXPO_NIGH"){
        setExposure(EXPO_NIGH);
    } else if(name == "EXPO_NIGHT_PREVIEW"){
        setExposure(EXPO_NIGHT_PREVIEW);
    } else if(name == "EXPO_BLACK_LIGHT"){
        setExposure(EXPO_BLACK_LIGHT);
    } else if(name == "EXPO_SPOT_LIGHT"){
        setExposure(EXPO_SPOT_LIGHT);
    } else if(name == "EXPO_SPORT"){
        setExposure(EXPO_SPORT);
    } else if(name == "EXPO_SNOW"){
        setExposure(EXPO_SNOW);
    } else if(name == "EXPO_BEACH"){
        setExposure(EXPO_BEACH);
    } else if(name == "EXPO_VERY_LONG"){
        setExposure(EXPO_VERY_LONG);
    } else if(name == "EXPO_FIXED_FPS"){
        setExposure(EXPO_FIXED_FPS);
    } else if(name == "EXPO_ANTI_SHAKE"){
        setExposure(EXPO_ANTI_SHAKE);
    } else if(name == "EXPO_FIREWORKS"){
        setExposure(EXPO_FIREWORKS);
    } else if(name == "AWB_OFF"){
        setAWD(AWB_OFF);
    } else if(name == "AWB_AUTO"){
        setAWD(AWB_AUTO);
    } else if(name == "AWB_OFF"){
        setAWD(AWB_OFF);
    } else if(name == "AWB_SUN"){
        setAWD(AWB_SUN);
    } else if(name == "AWB_CLOUD"){
        setAWD(AWB_CLOUD);
    } else if(name == "AWB_SHADE"){
        setAWD(AWB_SHADE);
    } else if(name == "AWB_TUNGSTEN"){
        setAWD(AWB_TUNGSTEN);
    } else if(name == "AWB_FLUORESCENT"){
        setAWD(AWB_FLUORESCENT);
    } else if(name == "AWB_INCADESCENT"){
        setAWD(AWB_INCADESCENT);
    } else if(name == "AWB_FLASH"){
        setAWD(AWB_FLASH);
    } else if(name == "AWB_HORIZON"){
        setAWD(AWB_HORIZON);
    } else if(name == "FX_NONE"){
        setEffect(FX_NONE);
    } else if(name == "FX_NEGATIVE"){
        setEffect(FX_NEGATIVE);
    } else if(name == "FX_SOLARISE"){
        setEffect(FX_SOLARISE);
    } else if(name == "FX_SKETCH"){
        setEffect(FX_SKETCH);
    } else if(name == "FX_DENOISE"){
        setEffect(FX_DENOISE);
    } else if(name == "FX_EMBOS"){
        setEffect(FX_EMBOS);
    } else if(name == "FX_OILPAINT"){
        setEffect(FX_OILPAINT);
    } else if(name == "FX_HATCH"){
        setEffect(FX_HATCH);
    } else if(name == "FX_GPEN"){
        setEffect(FX_GPEN);
    } else if(name == "FX_PASTEL"){
        setEffect(FX_PASTEL);
    } else if(name == "FX_WATERCOLOUR"){
        setEffect(FX_WATERCOLOUR);
    } else if(name == "FX_FILM"){
        setEffect(FX_FILM);
    } else if(name == "FX_BLUR"){
        setEffect(FX_BLUR);
    } else if(name == "FX_SATURATION"){
        setEffect(FX_SATURATION);
    } else if(name == "FX_COLOURSWAP"){
        setEffect(FX_COLOURSWAP);
    } else if(name == "FX_WASHEDOUT"){
        setEffect(FX_WASHEDOUT);
    } else if(name == "FX_POSTERISE"){
        setEffect(FX_POSTERISE);
    } else if(name == "FX_COLOURPOINT"){
        setEffect(FX_COLOURPOINT);
    } else if(name == "FX_COLOURBALANCE"){
        setEffect(FX_COLOURBALANCE);
    } else if(name == "FX_CARTOON"){
        setEffect(FX_CARTOON);
    } else if(name == "FX_NONE"){
        setEffect(FX_NONE);
    } else if(name == "FX_NONE"){
        setEffect(FX_NONE);
    }
}

void UIRpiCam::setRaspberryPiHost(string _host){
    host = _host;
}

void UIRpiCam::setExposure(RpiCamExposure _expo){
    string values[] = {"off", "auto", "night", "nightpreview", "backlight", "spotlight", "sports", "snow", "beach", "verylong", "fixedfps", "antishake", "fireworks"};
    exposure = values[_expo];
}

void UIRpiCam::setAWD(RpiCamAWB _awb){
    string values[] = {"off", "auto", "sun", "cloud", "shade", "tungsten", "fluorescent", "incandescent", "flash", "horizon"};
    awb = values[_awb];
}

void UIRpiCam::setEffect(RpiCamEffect _fx){
    string values[] = {"none", "negative", "solarise", "sketch", "denoise", "emboss", "oilpaint", "hatch", "gpen", "pastel", "watercolour", "film", "blur", "saturation", "colourswap", "washedout", "posterise", "colourpoint", "colourbalance", "cartoon"};
    imxfx = values[_fx];
}

void UIRpiCam::setUseTexture(bool bUseTex){
     image.setUseTexture(bUseTex);
}

void UIRpiCam::load(){
    image.clear();
    string command = "http://"+host+"/rpicam/?width="+ofToString(width);
    command +="&height="+ofToString(height);
    command +="&quality="+ofToString(quality);
    command +="&sharpness="+ofToString(sharpness);
    command +="&contrast="+ofToString(contrast);
    command +="&brightness="+ofToString(brightness);
    command +="&saturation="+ofToString(saturation);
    command +="&rotation="+ofToString(rotation);
    command +="&exposure="+exposure;
    command +="&awb="+awb;
    command +="&imxfx="+imxfx;
    if(bHFlip){
        command +="&-hflip=1";
    }
    if(bVFlip){
        command +="&-vflip=1";
    }
    ofLoadURLAsync(command,"rpiCam");
    bLoading = true;
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