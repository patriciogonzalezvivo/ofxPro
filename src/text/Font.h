//
//  Font.h
//
//  Created by Patricio Gonzalez Vivo on 9/26/13.
//
//

#pragma once

class Font: public ofTrueTypeFont {
public:
    
    float getCharacterWidth(char ch){
        if (ch==' ') ch='i';
        if (ch=='\xE0') ch='a'; //à
        if (ch=='\xE1') ch='a'; //á
        if (ch=='\xE2') ch='a'; //â
        if (ch=='\xE3') ch='a'; //ã
        if (ch=='\xE4') ch='a'; //ä
        if (ch=='\xE6') ch='a'; //æ
        if (ch=='\xE8') ch='e'; //è
        if (ch=='\xE9') ch='e'; //é
        if (ch=='\xEA') ch='e'; //ê
        if (ch=='\xEB') ch='e'; //ë
        if (ch=='\xEC') ch='i'; //ì
        if (ch=='\xED') ch='i'; //í
        if (ch=='\xEE') ch='i'; //î
        if (ch=='\xEF') ch='i'; //ï
        if (ch=='\xF2') ch='o'; //ò
        if (ch=='\xF3') ch='o'; //ó
        if (ch=='\xF4') ch='o'; //ô
        if (ch=='\xF5') ch='o'; //õ
        if (ch=='\xF6') ch='o'; //ö
        if (ch=='\xF9') ch='u'; //ù
        if (ch=='\xFA') ch='u'; //ú
        if (ch=='\xFB') ch='u'; //û
        if (ch=='\xFC') ch='u'; //ü
        if (ch=='\xC7') ch='c'; //ç
        return cps[ch-NUM_CHARACTER_TO_START].setWidth;
    }
    
};

typedef struct{
    string character;
    string code;
} charSubstitution;