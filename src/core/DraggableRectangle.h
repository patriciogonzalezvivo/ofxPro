//
//  DraggableRectangle.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

class DraggableRectangle: public ofRectangle {
public:
    
    virtual bool loadSettings(string _fileConfig, string _tagName = "rect", int _num = 0 );
    virtual bool saveSettings(string _fileConfig, string _tagName = "rect", int _num = 0 );
    
    bool mouseDragged(ofPoint _mouse);
    
    void draw();
    
protected:
    
    virtual void loadCustomValues(ofxXmlSettings &_XML){};
    virtual void saveCustomValues(ofxXmlSettings &_XML){};
    
    void    drawBrakets(ofRectangle _rect, float size, float margin);
    string  name;
    
};
