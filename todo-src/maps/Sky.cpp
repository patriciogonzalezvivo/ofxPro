//
//  Sky.cpp
//  DV
//
//  Created by Patricio Gonzalez Vivo on 8/13/13.
//
//

#include "Sky.h"

Sky::Sky(){
    ofSpherePrimitive::set(1000, 60);
}

void Sky::set(float _size){
    ofSpherePrimitive::set(_size, 60);
}

void Sky::loadStarts( string _file){
    ofBuffer buffer = ofBufferFromFile(_file);
    
    string lastConstelation = "";
    
    while(!buffer.isLastLine()) {
        string temp = buffer.getNextLine();
        
        if(temp.length() != 0) {
            vector<string> values = ofSplitString(temp, ",");
            
            if ( values[0] != lastConstelation ){
                constelations.push_back(values[0]);
            }
            
            lastConstelation = values[0];
            
            Star *a = new Star();
            a->place(this, ofToFloat(values[2]), ofToFloat(values[1]) );
            a->constelationName = lastConstelation;
            
            Star *b = new Star();
            b->place(this, ofToFloat(values[4]), ofToFloat(values[3]) );
            b->constelationName = lastConstelation;
            b->connect = a;
            
            stars.push_back(a);
            stars.push_back(b);
        }
    }
}