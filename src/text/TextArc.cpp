//
//  TextArc.cpp
//
//  Created by Patricio González Vivo on 4/24/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#include "TextArc.h"

TextArc::TextArc(){
    letters = NULL;
    
    angle   = 0;
    scale   = 1.0;
    
    x = ofGetWidth()*0.5;
    y = ofGetHeight()*0.5;
    radius  = 300;
    
    hAlignment  =   TEXT_ALIGN_CENTER;
    vAlignment  =   TEXT_ALIGN_TOP;
}

void TextArc::loadFont(string _fontLocation, float _fontSize, int _dpi ){
    font.loadFont(_fontLocation, _fontSize, true, true);
    font.setGlobalDpi(_dpi);
    font.setLetterSpacing(1.1);
}

void TextArc::setText(string _inputText){
    rawText     = _inputText;
    _subsChars(rawText);
    _loadWords();
}

void TextArc::draw(float _x, float _y){
    
    ofPushMatrix();
    ofTranslate(_x,_y);
    
    switch (hAlignment) {
        case TEXT_ALIGN_LEFT: ofRotateZ(ofRadToDeg(angle)); break;
        case TEXT_ALIGN_CENTER: ofRotateZ(ofRadToDeg(angle + totalAngle*0.5)); break;
        case TEXT_ALIGN_RIGHT: ofRotateZ(ofRadToDeg(angle + totalAngle)); break;
        case TEXT_ALIGN_JUSTIFIED: ofRotateZ(ofRadToDeg(angle)); break;
    }
    
    //  Draw debug lines
    //
    /*
    ofPushStyle(); 
    ofSetColor(50);
    ofNoFill();
    ofCircle(0,0,radius);
    ofLine(0,0,0,radius);
    ofPushMatrix();
    for (int i=0; i < rawText.length(); i++) {
        ofRotateZ(-ofRadToDeg(letters[i].angle));
        ofLine(0,0,0,radius);
    }
    ofPopMatrix();
    ofPopStyle();
    */
    
    for (int i = 0; i < rawText.length(); i++) {
        ofPushMatrix();
        ofTranslate( 0, letters[i].radius);
        ofRotateZ( -ofRadToDeg(letters[i].angle)*0.5 ); //local rotation, use this or center letter before rotating
        ofScale(letters[i].scale, letters[i].scale);
        font.drawString( ofToString( (char)rawText.at(i) ), 0, 0);
        ofPopMatrix();
        
        ofRotateZ( -ofRadToDeg(letters[i].angle) );
    }
    
    ofPopMatrix();
}

void TextArc::_loadWords(){
    letters = new LetterArc[rawText.length()];
    
    totalAngle = 0;
    float averageHeight = font.stringHeight(rawText);
    
    for (int i = 0; i < rawText.length(); i++) {
        
        letters[i].scale  = scale;
        letters[i].radius = radius;
        letters[i].width = font.getCharacterWidth(rawText.at(i)) * font.getLetterSpacing();
        letters[i].angle = atan(letters[i].width / letters[i].radius);
        
        totalAngle += letters[i].angle;
    }
}