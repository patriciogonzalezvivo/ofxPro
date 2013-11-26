//
//  UIBufferIn.cpp
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 11/19/13.
//
//

#include "UIBufferIn.h"

UIBufferIn::UIBufferIn(){
    bufferSize = 512;
    sampleRate = 44100;
    left    = new float[bufferSize];
	right   = new float[bufferSize];
    
    for(int i = 0; i < bufferSize; i++){
        left[i] = 0;
        right[i] = 0;
    }
}

void UIBufferIn::setup(int _sampleRate, int _bufferSize){
    sampleRate = _sampleRate;
    bufferSize = _bufferSize;
}

UIBufferIn::~UIBufferIn(){
    delete []left;
    delete []right;
}

void UIBufferIn::start(){
    stream.setup(0, 2, sampleRate, bufferSize, 4);
    stream.setInput(this);
}

void UIBufferIn::audioReceived(float * input, int bufferSize, int nChannels ){
    for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
	}
}

void UIBufferIn::stop(){
    stream.stop();
    
}

void UIBufferIn::setupUI(){
    gui->addWaveform("LEFT", left, bufferSize);
    gui->addWaveform("RIGHT", right, bufferSize);
}

void UIBufferIn::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name == "ENABLE"){
        if(bEnable){
            start();
        } else {
            stop();
        }
    }
}