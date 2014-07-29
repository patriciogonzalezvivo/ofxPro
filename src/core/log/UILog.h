//
//  UILog.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once
#include "UIClass.h"
#include "UICamera.h"

#include "Flickr.h"
#include "Recorder.h"
#include "FileUploader.h"

class UILog : public UIClass {
public:
    
    UILog();

    void    linkDataPath(string _path){dataPath=_path;}
    void    linkCamera(UICamera *_camera){camera=_camera;};
    void    linkRenderTarget(ofBaseHasTexture *_tex);
    
    string  getClassName(){return "LOG";};

    void    screenShot(string _post = "");

    void    record(bool _state);
    void    recordAddFrame();
    bool    isRecording(){return bRecording;};
    
    void    upload();
    
    void    drawStatus();
    
protected:
    void    setupUI();
    void    guiEvent(ofxUIEventArgs &e);
    
    void    uploadCompleted(string &_result);
    vector<FileUploader*> uploadingFiles;
    
    UICamera        *camera;
    ofBaseHasTexture *renderTarget;
    ofPixels        pixels;
    string          dataPath;
    
    Flickr::API     flickrAPI;
    Recorder        recorder;
    string          lastPicture;
	string          lastRercord;
    bool            bRecording;
};