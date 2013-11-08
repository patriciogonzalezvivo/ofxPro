//
//  Note.h
//
//  Created by Patricio Gonzalez Vivo on 11/7/13.
//
//

#pragma once
#include "ofMain.h"

class Note {
public:
    Note():bPenDown(false),bEnable(true){}
    virtual ~Note(){}
    
    void    load(string _file);
    void    save(string _file);
    
    void    addPoint( ofPoint _point );
    void    penUp();
    
    void    clearAll();
    void    clearLast();
    
    void    draw();
    
    string  name;
    bool    bEnable;
    
protected:
    ofPolyline actualLine;
    vector<ofPolyline> lines;
    
    bool    bPenDown;
};
