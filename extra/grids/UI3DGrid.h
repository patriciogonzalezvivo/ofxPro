//
//  UI3DGrid.h
//  example
//
//  Created by Patricio Gonzalez Vivo on 11/3/13.
//
//


#pragma once
#include "UIClass.h"

class UI3DGrid : public UIClass {
public:
    
    UI3DGrid();

    string  getClassName(){ return "3DGRID"; }
    
    void    draw();
    
protected:
    virtual void    setupUI();
    virtual void    guiEvent(ofxUIEventArgs &e);

    ofImage colorSampleImage;
    
	ofVbo   gridMajor;
    ofVbo   grid;

    ofShader shader;
    
    ofColor gridColor;
    ofColor gridMajorColor;
    
    float   gridLineWidth;
    float   gridScale;
    float   majorGridLineWidth;
    float   gridDim;
    float   fogFalloffDistance;
    float   fogFalloffExpo;
    float   fogFalloffScale;
    float   gridAlphaScale;
    float   gridMajorScale;
    
    float   gridAlpha;
    float   gridMajorAlpha;
    float   gridBrightness;
    float   gridMajorBrightness;
    
	int     numGridVertices;
    int     numGridMajorVertices;
};