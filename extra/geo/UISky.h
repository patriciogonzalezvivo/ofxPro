//
//  UISky.h
//
//  Created by Patricio Gonzalez Vivo on 8/13/13.
//
//

#pragma once
#include "UIClass.h"
#include "Star.h"

class UISky : public Sphere, public UIClass {
public:
    UISky();
    
    void set(float _size);
    void setupUI();
    void loadStarts( string _file );
    
    string getClassName(){return "START SKY"; }
    
    void draw();
    
protected:
    void guiEvent(ofxUIEventArgs &e);
    vector< Star* > stars;
    vector< string > constelations;
    
    string  selectedConstelation;
    float   constelationMin,constelationMax, constelationRnd, constelationLerp;
    
    ofVboMesh   starsVbo;
};