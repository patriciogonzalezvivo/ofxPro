//
//  ParseTextTools.h
//
//  Created by Patricio Gonzalez Vivo on 7/8/14.
//
//

#pragma once

void jumpNextWord(int &_index, vector<string> &_words){
    _index++;
    
    while( ((_words[ _index].size() <= 1) || (_words[ _index] == "\t")) && ( _index < _words.size())){
        _index++;
    }
}

void eraseSemiColon(string &_word){
    int end = _word.find(';') ;
    for(int i = _word.size()-1; i >= end ; i--){
        _word.erase(_word.begin()+i);
    }
}
