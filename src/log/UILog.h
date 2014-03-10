//
//  UILog.h
//
//  Created by Patricio Gonzalez Vivo on 11/8/13.
//
//

#pragma once
#include "UIClass.h"
#include "UICamera.h"

#include "Note.h"
#include "Flickr.h"
#include "Recorder.h"

#include "FileUploader.h"
#include "BrushedLine.h"

class UILog : public UIClass {
public:
    
    UILog();
    virtual ~UILog();

    void    linkDataPath(string _path){dataPath=_path;}
    void    linkCamera(UICamera &_camera){camera=&_camera;};
    
    string  getClassName(){return "LOG";};

    void    screenShot(string _post = "");

    void    record(bool _state);
    void    recordAddFrame(ofBaseHasTexture &_texBase);
    bool    isRecording(){return bRecording;};
    
    bool    isTakingNotes(){return bEnable&&bNoteTaking;};
    bool    penDown(ofPoint _mouse);
    bool    penUp();
    
    void    upload();
    
    void    logSave(string _name);
    void    logLoad(string _name);
    
    void    draw();
    void    drawStatus();
    
protected:
    void    setupUI();
    void    guiEvent(ofxUIEventArgs &e);
    
    void    uploadCompleted(string &_result);
    vector<FileUploader*> uploadingFiles;
    
    UICamera        *camera;
    ofFbo           *renderTarget;
    string          dataPath;
    
    Flickr::API     flickrAPI;
    Recorder        recorder;
    string          lastPicture;
	string          lastRercord;
    bool            bRecording, bRecordAll;
    
    //  Notes
    BrushedLine     BL;
    ofTexture       BLTexture;
    ofShader        shader;
    vector<Note*>   notes;
    Note            *actualNote;
    ofxUITextInput  *textField;
    bool            bNoteTaking;
    bool            addNewNote;
};