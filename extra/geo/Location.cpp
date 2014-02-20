//
//  GlobePoint.cpp
//  PrintMaking
//
//  Created by Patricio Gonzalez Vivo on 9/24/13.
//
//

#include "Location.h"

void Location::setLatLon(double _lat, double _lon){
    lat = _lat;
    lon = _lon;
}

void Location::setInGlobe(Sphere *_sphere ){
    ofVec3f displacementFromCenter = ofVec3f(0,0, _sphere->getRadius() );
    
    ofQuaternion latRot;
    latRot.makeRotate( lat, 1, 0, 0);
    
    ofQuaternion longRot;
    longRot.makeRotate( lon, 0, 1, 0);
    
    quaternion = latRot * longRot;
    set(latRot * longRot * displacementFromCenter);
};

bool Location::operator == (const Location &l) const{
    return l.lat == lat && l.lon == lon;
}

