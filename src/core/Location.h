//
//  Location.h
//
//  Created by Patricio Gonzalez Vivo on 9/24/13.
//
//

#pragma once
#include "ofMain.h"

class Location : public ofPoint {
public:

	Location() : lat(0), lon(0) {}
	Location(double _lat, double _lon) : lat(_lat), lon(_lon) {}
	Location(const Location &location) : lat(location.lat), lon(location.lon) {}
    
    virtual void setLatLon(double _lat, double _lon){ lat=_lat; lon=_lon; }
    
	friend ostream &operator<<(ostream &stream, Location l){
        stream << "(" << l.lat << ", " << l.lon << ")";
        return stream;
    };
    
    bool operator == (const Location &l) const{
        return l.lat == lat && l.lon == lon;
    };
    
    ofQuaternion getQuaternion(){
        ofQuaternion latRot;
        latRot.makeRotate( lat, 1, 0, 0);
        
        ofQuaternion longRot;
        longRot.makeRotate( lon, 0, 1, 0);
        
        ofQuaternion quaternion = latRot * longRot;
        
        return latRot*longRot;
    };
    
    ofPoint getSpherePosition(float _radius){
        return getQuaternion() * ofVec3f(0,0, _radius);
    };
    
    double lat, lon;
};