//
//  UIStreetView.h
//
//  Created by Patricio Gonzalez Vivo on 3/10/14.
//
//

#pragma once

#include "UIMap.h"
#include "Location.h"

struct DepthMapPlane : public ofPoint {
    float d;
};

struct Link {
    float yaw_deg;
    string pano_id;
};

class UIStreetView : public UIClass {
public:
    
    UIStreetView();
	UIStreetView(double _lat, double _lng);
	UIStreetView(const Location &location);
    virtual ~UIStreetView();
    
    void    setLocation(const Location &_loc);
    void    setLatLon(double _lat, double _lng);
    
    string  getClassName(){return "StreetView";};
    
    void    urlResponse(ofHttpResponse & response);
    
protected:
    void    setupUI();
    void    guiEvent(ofxUIEventArgs &e);
    
    ofFbo           fbo;
    vector<ofImage> images;
    
    string  pano_id;
    
    float  lat, lng, original_lat, original_lng;
    float  pano_yaw_deg,tilt_yaw_deg,tilt_pitch_deg;
    int    num_zoom_levels;

    //Depth map information
    int     mapWidth, mapHeight;
    vector<unsigned char> depthmapIndices;
    vector<struct DepthMapPlane> depthmapPlanes;
    
    vector<struct Link> links;
};
