//
//  UILog.h
//
//  Created by Patricio Gonzalez Vivo on 11/10 /13.
//
//

#pragma once
#include "UIClass.h"

enum RpiCamExposure{
    EXPO_OFF = 0,
    EXPO_AUTO,
    EXPO_NIGH, EXPO_NIGHT_PREVIEW,
    EXPO_BLACK_LIGHT, EXPO_SPOT_LIGHT,
    EXPO_SPORT, EXPO_SNOW, EXPO_BEACH,
    EXPO_VERY_LONG, EXPO_FIXED_FPS, EXPO_ANTI_SHAKE, EXPO_FIREWORKS
};

enum RpiCamAWB{
    AWB_OFF = 0,
    AWB_AUTO,
    AWB_SUN, AWB_CLOUD, AWB_SHADE,
    AWB_TUNGSTEN,AWB_FLUORESCENT, AWB_INCADESCENT, AWB_FLASH,
    AWB_HORIZON
};

enum RpiCamEffect{
    FX_NONE = 0,
    FX_NEGATIVE,
    FX_SOLARISE,
    FX_SKETCH,
    FX_DENOISE,
    FX_EMBOS,
    FX_OILPAINT,
    FX_HATCH,
    FX_GPEN,
    FX_PASTEL,
    FX_WATERCOLOUR,
    FX_FILM,
    FX_BLUR,
    FX_SATURATION,
    FX_COLOURSWAP,
    FX_WASHEDOUT,
    FX_POSTERISE,
    FX_COLOURPOINT,
    FX_COLOURBALANCE,
    FX_CARTOON
};

class UIRpiCam : public UIClass, public ofBaseHasTexture {
public:
    
    UIRpiCam();
    virtual ~UIRpiCam();
    
    void    setRaspberryPiHost(string _host = "raspberrypi.local");
    void    setExposure(RpiCamExposure _expo);
    void    setAWD(RpiCamAWB _awb);
    void    setEffect(RpiCamEffect _fx);
    void    setUseTexture(bool bUseTex);
    
    ofTexture&  getTextureReference();
    string  getClassName(){return "RPI-CAM";};
    
    bool    isLoading(){return bLoading;}
    
    void    load();
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

    ofxUITextInput  *textField;
    string  host;
    
    string  exposure;
    string  awb;
    string  imxfx;

    bool    bLoading;
};