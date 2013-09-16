//
//  RectangleBooks.h
//  cLibrary
//
//  This is a class that extends from ofRectangle to deal with
//  the boxes and books (points).
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

class DraggableRectangle: public ofRectangle{
public:
    
    bool    loadSettings(string _fileConfig);
    bool    loadSettings(int _num, string _fileConfig);
    bool    loadSettings(string _name, string _fileConfig);
    
    bool    saveSettings(string _fileConfig);
    bool    saveSettings(int _num , string _fileConfig);
    bool    saveSettings(string _name, string _fileConfig);
    
    void    mouseDragged(ofPoint _mouse);
    
    void    draw();

    string  name;
    
protected:
    void    drawBrakets(ofRectangle _rect, float size, float margin);
};
