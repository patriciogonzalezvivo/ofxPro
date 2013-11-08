//
//  CameraLocation.h
//
//  Created by Patricio Gonzalez Vivo on 11/8/13.
//
//

#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"

struct CameraLocation{
    
    ofQuaternion orientation;
    ofPoint      position;
    float        distance;
    float        FOV;
    
    inline bool operator==(const CameraLocation& _other){
        return (position == _other.position && orientation == _other.orientation)?true:false;
    }
    
    inline bool operator!=(const CameraLocation& _other){return !(*this==_other);}
    
    bool save(string _fileConfig){
        bool fileFound = false;
        
        ofxXmlSettings XML;
        XML.setValue("camera:fov",FOV);
        XML.setValue("camera:distance",distance);
        XML.setValue("camera:position:x", position.x);
        XML.setValue("camera:position:y", position.y);
        XML.setValue("camera:position:z", position.z);
        XML.setValue("camera:orientation:x", orientation.x());
        XML.setValue("camera:orientation:y", orientation.y());
        XML.setValue("camera:orientation:z", orientation.z());
        XML.setValue("camera:orientation:w", orientation.w());
        return XML.saveFile(_fileConfig);
    }
    
    bool load(string _fileConfig){
        bool fileFound = false;
        ofxXmlSettings XML;
        if (XML.loadFile(_fileConfig)){
            if (XML.pushTag("camera") ){
                fileFound = true;
                
                FOV      = XML.getValue("fov", FOV);
                distance = XML.getValue("distance", distance);
                position.x = XML.getValue("position:x", position.x);
                position.y = XML.getValue("position:y", position.y);
                position.z = XML.getValue("position:z", position.z);
                orientation.set(XML.getValue("orientation:x", orientation.x()),
                                XML.getValue("orientation:y", orientation.y()),
                                XML.getValue("orientation:z", orientation.z()),
                                XML.getValue("orientation:w", orientation.w()));
            }
        }
        return fileFound;
    }
};