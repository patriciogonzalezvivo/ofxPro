//
//  UICamera.h
//  example
//
//  Created by Patricio Gonzalez Vivo on 11/4/13.
//
//

#pragma once

#include "UIClass.h"

struct CameraLocation{
    float        distance;
    ofPoint      position;
    ofQuaternion orientation;
    
    inline bool operator==(const CameraLocation& _other){
        return (position == _other.position && orientation == _other.orientation)?true:false;
    }
    
    inline bool operator!=(const CameraLocation& _other){return !(*this==_other);}
    
    bool save(string _fileConfig){
        bool fileFound = false;
        
        ofxXmlSettings XML;
        if (XML.loadFile(_fileConfig)){
            if ( XML.pushTag("camera") ){
                XML.setValue("distance",distance);
                XML.setValue("position:x", position.x);
                XML.setValue("position:y", position.y);
                XML.setValue("position:z", position.z);
                XML.setValue("orientation:x", orientation.x());
                XML.setValue("orientation:y", orientation.y());
                XML.setValue("orientation:z", orientation.z());
                XML.setValue("orientation:w", orientation.w());
                XML.popTag();
                fileFound = XML.saveFile();
            }
        } else {
            XML.addTag("camera");
            XML.pushTag("camera");
            XML.setValue("distance",distance);
            XML.addTag("position");
            XML.setValue("position:x", position.x);
            XML.setValue("position:y", position.y);
            XML.setValue("position:z", position.z);
            XML.addTag("orientation");
            XML.setValue("orientation:x", orientation.x());
            XML.setValue("orientation:y", orientation.y());
            XML.setValue("orientation:z", orientation.z());
            XML.setValue("orientation:w", orientation.w());
            XML.popTag();
            fileFound = XML.saveFile(_fileConfig);
        }
        return fileFound;
    }
    
    bool load(string _fileConfig){
        bool fileFound = false;
        ofxXmlSettings XML;
        if (XML.loadFile(_fileConfig)){
            if (XML.pushTag("camera") ){
                fileFound = true;
                
                distance = XML.getValue("distance", 0.0);
                position.x = XML.getValue("position:x", 0.0);
                position.y = XML.getValue("position:y", 0.0);
                position.z = XML.getValue("position:z", 0.0);
                orientation.set(XML.getValue("orientation:x", 0.0),
                                XML.getValue("orientation:y", 0.0),
                                XML.getValue("orientation:z", 0.0),
                                XML.getValue("orientation:w", 0.0));
            }
        }
        return fileFound;
    }
};

class UICamera : public UIClass {
public:
    
    UICamera();
    virtual ~UICamera(){};
    
    bool    save(string _savePath);
    bool    load(string _savePath);
    
    void    addLocation(string _name);
    void    loadLocations(string _path);
    
    void    setupUI();
    
    void            setCameraLocation( const CameraLocation &_camPos );
    CameraLocation  getCameraLocation();
    
    string          getClassName(){return "CAMERA";};
    
    ofCamera*       getCameraPtn(){return camera;};
    
    void    enableMouseInput();
    void    disableMouseInput();

    void    billboard();
    
private:
    void    update(ofEventArgs& args);
    void    guiEvent(ofxUIEventArgs &e);
    
    ofCamera *camera;
    string  type;
    float   FOV;
    
    CameraLocation  targetLocation;
    ofxUIRadio      *locations;
    string          pathToLocations;
    float           pct;
    float           speed;
    bool            bSaveLocation;
};


