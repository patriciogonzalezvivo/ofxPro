//
//  StreetView.h
//
//  Created by Patricio Gonzalez Vivo on 3/10/14.
//  Credits to:
//              Paul Wagener https://github.com/PaulWagener/Streetview-Explorer
//

#pragma once

#define PANOID_LENGTH 22

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "Location.h"

struct RenderSettings {
    bool transparancy;
    int verticalAccuracy;
    int horizontalDetail;
};

struct DepthMapPlane {
    float x, y, z;
    float d;
};

struct Link {
    float yaw_deg;
    string pano_id;
};

class StreetView : public ofBaseHasTexture {
public:
    
    StreetView();
    StreetView(string _pano_id);
	StreetView(double _lat, double _lng);
	StreetView(Location _location);
    virtual     ~StreetView();
    
    void        setPanoId(string _pano_id);
    void        setLocation(Location _loc);
    void        setLatLon(double _lat, double _lng);
    
    void        setAutoDownload(bool _enable);
    void        setUseTexture(bool bUseTex){};
    
    string      getCloseLinkTo(float _deg);
    Location    getLocation();
    string      getPanoId(){return pano_id;};
    float       getDirection(){return pano_yaw_deg;}
    float       getGroundHeight();

    float       getWidth();
	float       getHeight();
    ofTexture&  getTextureReference();
    
    ofTexture   getLookAt(float _deg, float _amp); // 0 - North
    ofTexture   getLookTo(float _deg, float _amp); // 0 is the direction of the Google Street Car
    
    bool        isTextureLoaded(){return bPanoLoaded;};
    void        downloadPanorama();
    void        urlResponse(ofHttpResponse & response);
    
    void        clear();
    void        update();
    void        updateModel();
    
    vector<Link> links;
    
    void        draw();
    
    
protected:
    bool    isVisible(int x, int y);
    bool    isTransparant(int x, int y, int horizontal_step);
    
    void    addVertexAtAzimuthElevation(int x, int y);
    
    ofFbo           fbo;
    vector<ofImage> images;
    
    string      pano_id;
    Location    loc;
    
    float       pano_yaw_deg,tilt_yaw_deg,tilt_pitch_deg;
    int         num_zoom_levels;

    //Depth map information
    int     mapWidth, mapHeight;
    string  depth_map_base64;
    vector<unsigned char> depthmapIndices;
    vector<struct DepthMapPlane> depthmapPlanes;
    
    //Panomap information
    string  pano_map_base64;
    vector<unsigned char> panomapIndices;
    char panoids[200][PANOID_LENGTH+1];
    int ownPanomapIndex;

    ofVboMesh   mesh;
    //  Flags
    //
    bool    bAutoDownload;
    bool    bDataLoaded;
    bool    bPanoLoaded;
};
