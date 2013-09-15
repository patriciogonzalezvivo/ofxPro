//
//  Grid.h
//
//  Created by Patricio Gonzalez Vivo on 8/10/13.
//
//

#pragma once

#include "ofMain.h"

class Grid : public ofRectangle {
public:
    
    Grid();
    
    float   getCols(){return cols;};
    float   getRows(){return rows;};
    float   getResolution(){return resolution;};
    ofPoint getOffset(){return offset;};
    
    void    setResolution(float _resolution, bool _calculateOffset = true);
    void    setSkipList( vector<ofRectangle> &_skip);
    void    forceOffSet( ofPoint _offset );
    
    bool    fixResolutionAndOffSet();
    virtual void    makeGrid();
    
    void    print();
    void    draw();
    
    ofFloatColor    color;
    
    float   margins;
    float   resolution;
    float   thickness;

    bool    bJump;
    
protected:
    vector<ofRectangle> skip;
    
    ofVboMesh       mesh;
    ofPoint         offset;
    float           realWidth,realHeight;
    int             cols,rows;
    
private:
    vector<ofRectangle*> getVertColitionAreasFor(ofPoint _top,ofPoint _bottom);
    vector<ofRectangle*> getHoriColitionAreasFor(ofPoint _left,ofPoint _right);
};