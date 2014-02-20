//
//  UISky.cpp
//  DV
//
//  Created by Patricio Gonzalez Vivo on 8/13/13.
//
//

#include "UISky.h"

UISky::UISky(){
    ofSpherePrimitive::set(1000, 60);
}

void UISky::setupUI(){
    gui->addSlider("Constelation_Min", 0.0, 1.0, &constelationMin);
    gui->addSlider("Constelation_Max", 0.0, 1.0, &constelationMax);
    gui->addSlider("Constelation_Lerp", 0.0, 0.1, &constelationLerp);
    gui->addSlider("Constelation_Randomizer", 0.0, 100, &constelationRnd);
}

void UISky::guiEvent(ofxUIEventArgs &e){
    
}

void UISky::set(float _size){
    ofSpherePrimitive::set(_size, 60);
}

void UISky::loadStarts( string _file ){
    
    ofBuffer buffer = ofBufferFromFile(_file);
    starsVbo.clear();
    starsVbo.setMode(OF_PRIMITIVE_POINTS);
    string lastConstelation = "";
    
    while(!buffer.isLastLine()) {
        string temp = buffer.getNextLine();
        
        if(temp.length() != 0) {
            vector<string> values = ofSplitString(temp, ",");
            
            if ( values[0] != lastConstelation ){
                constelations.push_back(values[0]);
            }
            
            lastConstelation = values[0];
            
            Star *a = new Star();
            a->place(this, ofToFloat(values[2]), ofToFloat(values[1]) );
            a->constelationName = lastConstelation;
            
            Star *b = new Star();
            b->place(this, ofToFloat(values[4]), ofToFloat(values[3]) );
            b->constelationName = lastConstelation;
            b->connect = a;
            
            stars.push_back(a);
            stars.push_back(b);
            
            starsVbo.addVertex( *a );
            starsVbo.addVertex( *b );
        }
    }
}

void UISky::draw(){
    
    if (bEnable){
        ofPushStyle();
        
        if ( (int)ofRandom(constelationRnd) == 1){
            selectedConstelation = constelations[ ofRandom(constelations.size()) ];
            for(int i = 0; i < stars.size(); i++){
                if (stars[i]->constelationName == selectedConstelation && constelationRnd >= 1.0 ){
                    stars[i]->constelationAlpha = ofLerp(stars[i]->constelationAlpha,constelationMax,constelationLerp);
                } else {
                    stars[i]->constelationAlpha = ofLerp(stars[i]->constelationAlpha,constelationMin,constelationLerp);
                }
                stars[i]->draw();
            }
        }
        
        ofSetColor(255);
        glPointSize(2);
        starsVbo.draw();
        ofPopStyle();
    }
}