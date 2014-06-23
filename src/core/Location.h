//
//  Location.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//  Credits to:
//              Paul Wagener https://github.com/PaulWagener/Streetview-Explorer
//              Tom Carden https://github.com/RandomEtc/modestmaps-of

#pragma once
#include "ofMain.h"

struct UtmPosition {
    int zoneX;
    char zoneY;
    
    double easting;
    double northing;
};

class Location {
public:

	Location() : lat(0), lon(0) {}
	Location(double _lat, double _lon) : lat(_lat), lon(_lon) {}
	Location(const Location &location) : lat(location.lat), lon(location.lon) {}
    
    void setLatLon(double _lat, double _lon){lat=_lat; lon=_lon;}
    void setFromUTM(UtmPosition _utm);
    
    bool operator == (const Location &l) const;
	friend ostream &operator<<(ostream &stream, Location l){
        stream << "(" << l.lat << ", " << l.lon << ")";
        return stream;
    };
    
    UtmPosition getUTM();

    double       getRadTo(Location _loc);
    double       getDegTo(Location _loc);
    double       getDistanceTo(Location _loc); // In Meters
    
    ofQuaternion getQuaternion();
    ofPoint     getSpherePosition(float _radius);
    
    double lat, lon;
};