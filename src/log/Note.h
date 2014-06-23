//
//  Note.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once
#include "ofMain.h"

#include "BrushedLine.h"

class Note {
public:
    Note():bPenDown(false),bEnable(true),BL(NULL){};
    virtual ~Note(){}
    
    void    load(string _file);
    void    save(string _file);
    
    void    addPoint( ofPoint _point );
    void    penUp();
    
    void    clearAll();
    void    clearLast();
    
    void    draw();
    void    smoothDraw();
    
    
    BrushedLine *BL;
    string  name;
    bool    bEnable;
    
protected:
    ofPolyline actualLine;
    vector<ofPolyline> lines;
    
    bool    bPenDown;
};
