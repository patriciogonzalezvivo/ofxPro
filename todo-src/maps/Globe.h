//
//  Globe.h
//  DV
//
//  Created by Patricio Gonzalez Vivo on 8/11/13.
//
//

#pragma once

#include "ofMain.h"
#include "Sphere.h"

class GlobePoint : public SpherePoint {
public:

    void setGeoPosition(Sphere *_sphere, float _lat, float _long){
        ofVec3f displacementFromCenter = ofVec3f(0,0, _sphere->getRadius() );
        
        ofQuaternion latRot;
        latRot.makeRotate( _lat, 1, 0, 0);
        
        ofQuaternion longRot;
        longRot.makeRotate( _long, 0, 1, 0);
        
        quaternion = latRot * longRot;
        set(latRot * longRot * displacementFromCenter);
    };

};

class Globe : public Sphere{
public:
    
    Globe();
    
    void    update();
    void    draw();
    
    ofFloatColor    haloColor, coastColor, riversColor;
    float           haloScale;
    
private:
    void        loadLatLonToVbo(ofVboMesh &_vbo, string _file);
    void        loadPositionsToVbo(ofVboMesh &_vbo, string _file);
    void        loadCities();
   
    ofVboMesh   coastVbo,   riversVbo;
    ofShader    haloShader;
};
