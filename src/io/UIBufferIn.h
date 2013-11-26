//
//  UIBufferIn.h
//
//  Created by Patricio Gonzalez Vivo on 11/19/13.
//
//
#pragma once
#include "UIClass.h"

class UIBufferIn : public UIClass, public ofBaseSoundInput {
public:
    
    UIBufferIn();
    virtual ~UIBufferIn();
    
    void    setup(int _sampleRate, int _bufferSize);
    
    void    start();
    void    stop();
    
    int     getBufferSize(){return bufferSize;};
    float   *left;
    float   *right;
    
    string  getClassName(){return "BUFFER-IN";};
    
protected:
    void    setupUI();
    void    guiEvent(ofxUIEventArgs &e);
    void    audioReceived( float * input, int bufferSize, int nChannels );
    
    ofSoundStream stream;
    
    int     sampleRate;
    int     bufferSize;
};