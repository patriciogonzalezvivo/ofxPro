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
    
    virtual bool loadSettings(string _fileConfig, string _tagName = "rect", int _num = 0 );
    virtual bool saveSettings(string _fileConfig, string _tagName = "rect", int _num = 0 );
    
    void mouseDragged(ofPoint _mouse);
    
    void draw();
    
protected:
    
    virtual void loadCustomValues(ofxXmlSettings &_XML){};
    virtual void saveCustomValues(ofxXmlSettings &_XML){};
    
    void    drawBrakets(ofRectangle _rect, float size, float margin);
    string  name;
    
};
