//
//  ofxTextShape.h
//
//  Created by Patricio González Vivo on 4/26/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "Font.h"
#include "TextSyles.h"

class TextShape : public ofRectangle {
public:
    
    TextShape();
    
    virtual void loadFont(string _fontLocation, float _fontSize, int _dpi = 90);

    virtual void setText(string _inputText);
    virtual void setScale(float _scale);
    virtual void setAlignment( TextAlignHorizontal _hAlignment , TextAlignVertical _vAlignment  = TEXT_ALIGN_TOP);
    
    virtual string  getText();
    virtual int     length();
    virtual void    clear();
    
    virtual TextShape& operator += (string _text);
    virtual TextShape& operator = (string _text);
    virtual TextShape& operator -- ();
    virtual TextShape& operator -- (int);
    
    virtual void  draw(){};
    
protected:
    virtual void _loadWords(){};
    virtual void _subsChars(string & origString);
    
    Font                font;
    TextAlignHorizontal hAlignment;
    TextAlignVertical   vAlignment;
    string              rawText;
    float               scale;
    
};
