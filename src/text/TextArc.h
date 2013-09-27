//
//  ofxTextArc.h
//
//  Created by Patricio González Vivo on 4/23/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//
//  Based on: https://github.com/companje/ofxArcText by campanje

#pragma once

#include "TextShape.h"

typedef struct {
    float       angle;
    float       width;
    float       radius;
    float       scale;
} LetterArc;

class TextArc : public TextShape {
public:
    TextArc();
    
    virtual void    loadFont(string _fontLocation, float _fontSize, int _dpi = 90);
    
    virtual void    set(float _x, float _y, float _w, float _h ){x = _x; y = _y; setRadius((_w+_h)*0.5); };
    virtual void    setText(string _inputText);
    
    virtual void    setRadius(float _radius){ radius = _radius; _loadWords(); };
    virtual void    setAngle(float _angle){ angle = _angle; _loadWords(); };
    
    virtual void    draw(){ setRadius( MIN(width,height)*0.5 ); draw(getCenter().x, getCenter().y); };
    virtual void    draw(float _x, float _y);
    
protected:
    virtual void    _loadWords();
    
    LetterArc       *letters;
    float           angle, totalAngle, radius;
};