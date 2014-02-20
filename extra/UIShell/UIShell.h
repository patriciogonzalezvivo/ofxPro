//
//  Shell.h
//
//  Created by Patricio Gonzalez Vivo on 10/27/13.
//
//

#pragma once
#include "UIClass.h"

class UIShell : public UIClass {
public:
    
    UIShell();
    virtual ~UIShell(){};
    
    string      getClassName(){return "SHELL";};
    
    void        feed(vector<float> &_newAverage);
    
    void        update();
    void        draw();
    
    float       buffer;
    float       noise;
    float       radio;
    float       translation;
    float       rotation;
    
private:
    
    void        setupUI();
    void        guiEvent(ofxUIEventArgs &e);
    
    ofPolyline  freqArc( vector<float> &_vector, const ofPoint &_center, float _angleBegin, float _angleEnd, float _minRad , bool _bSmooth = false);
    
    void        createSkin( int _width );
    void        addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c);
    void        addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d);

    vector< vector<float> > averages;
    vector< ofPoint >       points;
    
    ofVboMesh   mesh;
    ofPoint     offSet;
    
    int         bufferSize;
};