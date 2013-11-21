//
//  UIAudioIn.cpp
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 11/19/13.
//
//

#include "UIAudioIn.h"

UIAudioIn::UIAudioIn(){
    bufferSize = 512;
    sampleRate = 44100;
    left    = new float[bufferSize];
	right   = new float[bufferSize];
    
    for(int i = 0; i < bufferSize; i++){
        left[i] = 0;
        right[i] = 0;
    }
}

void UIAudioIn::setup(int _sampleRate, int _bufferSize){
    sampleRate = _sampleRate;
    bufferSize = _bufferSize;
}

UIAudioIn::~UIAudioIn(){
    delete []left;
    delete []right;
}

void UIAudioIn::start(){
    stream.setup(0, 2, sampleRate, bufferSize, 4);
    stream.setInput(this);
}

void UIAudioIn::audioReceived(float * input, int bufferSize, int nChannels ){
    for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
	}
}

void UIAudioIn::stop(){
    stream.stop();
    
}

void UIAudioIn::setupUI(){
    gui->addWaveform("LEFT", left, bufferSize);
    gui->addWaveform("RIGHT", right, bufferSize);
}

void UIAudioIn::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name == "ENABLE"){
        if(bEnable){
            start();
        } else {
            stop();
        }
    }
}