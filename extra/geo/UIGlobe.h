//
//  UIGlobe.h
//  DV
//
//  Created by Patricio Gonzalez Vivo on 8/11/13.
//
//

#pragma once

#include "UIClass.h"
#include "Location.h"

class UIGlobe : public Sphere , public UIClass {
public:
    
    UIGlobe();
    
    void    setupUI();
    void    loadMap( string _dir );
    void    loadCities( string _dir );
    
    string  getClassName(){return "GLOBE"; }
    
    void    update();
    void    draw();
    
    ofFloatColor    haloColor, coastColor, riversColor, citiesColor;
    float           haloScale;
    
protected:
    void        guiEvent(ofxUIEventArgs &e);
    void        loadLatLonToVbo(ofVboMesh &_vbo, string _file);
    void        loadPositionsToVbo(ofVboMesh &_vbo, string _file);
    
    ofImage     colorSampleImage;
    ofVboMesh   coastVbo,   riversVbo;
    ofShader    haloShader;
};
