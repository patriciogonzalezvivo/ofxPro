//
//  UILog.h
//
//  Created by Patricio Gonzalez Vivo on 11/10 /13.
//
//

#pragma once
#include "UIClass.h"

class UIRpiCam : public UIClass, public ofBaseHasTexture {
public:
    
    UIRpiCam();
    virtual ~UIRpiCam();
    
    void    setUseTexture(bool bUseTex);
    ofTexture&  getTextureReference();
    string  getClassName(){return "RPI-CAM";};
    
    void    urlResponse(ofHttpResponse & response);
    
    float   width;
    float   height;
    float   quality;    // 0 100
    float   sharpness;  // -100 100
    float   contrast;   // -100 100
    float   brightness; // 0 100
    float   saturation; // -100 100
    float   rotation;   // 0 359
    bool    bHFlip;
    bool    bVFlip;
    
protected:
    void    setupUI();
    void    guiEvent(ofxUIEventArgs &e);
    
    ofFbo   tmp;
    ofImage image;

    string   exposure;
    string   awb;
    string   imxfx;
    string   colfx;
    string   metering;

    bool    bLoading;
};