//
//  GlobePoint.h
//  PrintMaking
//
//  Created by Patricio Gonzalez Vivo on 9/24/13.
//
//

#pragma once
#include "ofMain.h"
#include "Sphere.h"

class Location : public ofPoint {
public:

	Location() : lat(0), lon(0) {}
	Location(double _lat, double _lon) : lat(_lat), lon(_lon) {}
	Location(const Location &location) : lat(location.lat), lon(location.lon) {}
	
    void setLatLon(double _lat, double _lon);
//    void setInGlobe(Sphere *_sphere);
    
	friend ostream &operator<<(ostream &stream, Location l){
        // TODO: number format
        stream << "(" << l.lat << ", " << l.lon << ")";
        return stream;
    };
    
    bool operator == (const Location &l) const;
    
    ofQuaternion getQuaternion(){ return quaternion; };
    
    double lat, lon;
    
protected:
    ofQuaternion quaternion;
};