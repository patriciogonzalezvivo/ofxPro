//
//  TextShape.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/26/13.
//
//

#include "TextShape.h"

TextShape::TextShape(){
    rawText     =   "";
    scale       =   1.0f;
    hAlignment  =   TEXT_ALIGN_LEFT;
    vAlignment  =   TEXT_ALIGN_TOP;
}

void TextShape::loadFont(string _fontLocation, float _fontSize, int _dpi ){
    font.loadFont(_fontLocation, _fontSize, true, true);
    font.setGlobalDpi(_dpi);
}

void TextShape::setText(string _inputText){
    _subsChars(rawText);
    _loadWords();
}

void TextShape::setScale(float _scale){
    scale = _scale;
}

void TextShape::setAlignment( TextAlignHorizontal _hAlignment , TextAlignVertical _vAlignment ){
    hAlignment = _hAlignment;
    vAlignment = _vAlignment;
}

string TextShape::getText() {
    return rawText;
}

int TextShape::length(){
    return rawText.length();
}

void TextShape::clear(){
    rawText.clear();
}

TextShape& TextShape::operator += (string _text){
    setText(rawText + _text);
}

TextShape& TextShape::operator = (string _text){
    setText(_text);
}

TextShape& TextShape::operator -- (){
    if( rawText.length()>0){
        rawText.erase( rawText.begin() );
        setText(rawText);
    }
}

TextShape& TextShape::operator -- (int){
    if( rawText.length()>0){
        rawText.erase(rawText.end()-1);
        setText(rawText);
    }
}

void TextShape::_subsChars(string & origString){
    
    static charSubstitution chars[]={ {"√†","\xE0"}, {"√°","\xE1"}, {"√¢","\xE2"}, {"√£","\xE3"}, {"√§","\xE4"}, {"√¶","\xE6"}, {"√≤","\xF2"},{"√≥","\xF3"}, {"√¥","\xF4"}, {"√µ","\xF5"}, {"√∂","\xF6"}, {"√π","\xF9"}, {"√∫","\xFA"}, {"√ª","\xFB"}, {"√º","\xFC"}, {"√®","\xE8"}, {"√©","\xE9"}, {"√™","\xEA"}, {"√´","\xEB"}, {"√¨","\xEC"}, {"√≠","\xED"}, {"√Æ","\xEE"}, {"√Ø","\xEF"}, {"√ß","\xE7"}, {"√á","\xC7"} };
    
    for(int i=0; i<24; i++){
        while(origString.find(chars[i].character) !=string::npos){
            origString = origString.substr(0,origString.find(chars[i].character)) + chars[i].code + origString.substr(origString.find(chars[i].character)+2);
        }
    }
}