//  UIText.h
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/26/13.
//
//

#pragma once

#include "UIClass.h"
#include "DraggableRectangle.h"

#include "TextBlock.h"
#include "TextArc.h"

class UIText: public UIClass, public DraggableRectangle {
public:
    
    UIText();
    
    void    setupUI();
    void    setText(string _text);
    void    setFontsDir( string _dir );
    
    string  getClassName(){ return name; }
    string  getText();
    
    void    mouseDragged(ofPoint _mouse);
    
    void    draw();
    void    debugDraw();
    
protected:
    virtual void loadCustomValues(ofxXmlSettings &_XML);
    virtual void saveCustomValues(ofxXmlSettings &_XML);
    
    void    guiEvent(ofxUIEventArgs &e);
    string  fontDir;
    vector<string> fonts;
    
    ofImage colorSampleImage;
    ofFloatColor    fontColor;
    
    string  fontName;
    float   fontSize;
    float   fontScale;
    
    string              text;
    TextAlignVertical   textAlignV;
    TextAlignHorizontal textAlignH;
    
    TextShapeType       shapeType;
    TextShape           *shape;
    
    float               arcAngle;
};