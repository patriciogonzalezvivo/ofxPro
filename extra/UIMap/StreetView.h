//
//  StreetView.h
//
//  Created by Patricio Gonzalez Vivo on 3/10/14.
//  Credits to:
//              Paul Wagener https://github.com/PaulWagener/Streetview-Explorer
//              http://maps.google.com/cbk?output=xml&cb_client=maps_sv&v=4&dm=1&pm=1&ph=1&hl=en&panoid=
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
    
    void        setZoom(int _zoom);
    
    void        setUseTexture(bool _bUseTex);
    bool        isTextureLoaded(){return bPanoLoaded;};
    float       getWidth();
	float       getHeight();
    int         getZoom(){return zoom;}
    
    ofTexture&  getTextureReference();
    
    ofTexture   getTextureAt(float _deg, float _amp); // 0 - North
    
    Location    getLocation();
    string      getAddress(){return street_range + " " + text;}
    string      getRegion(){return region;}
    string      getCountry(){return country;}
    string      getPanoId(){return pano_id;};
    string      getCloseLinkTo(float _deg);
    double      getElevation(){return elevation;}
    double      getDirection(){return pano_yaw_deg;}
    double      getTiltPitch(){return tilt_pitch_deg;}
    double      getTiltYaw(){return tilt_yaw_deg;}
    double      getGroundHeight();
    
    int         getDepthMapWidth(){return mapWidth;}
    int         getDepthMapHeight(){return mapHeight;}
    ofImage     getDepthMap();
    ofVboMesh&  getDethMesh(){return meshDepth;};
    vector<unsigned char> depthmapIndices;
    vector<DepthMapPlane> depthmapPlanes;
    
    void        urlResponse(ofHttpResponse & response);
    void        clear();
    
    void        update();
    
    vector<Link> links;
    
protected:
    void        downloadPanorama();
    vector<ofImage> panoImages;
    ofFbo       panoFbo;
    
    void        makeDepthMesh();
    bool        isDepthVertexVisible(int x, int y);
    bool        isDepthVertexTransparant(int x, int y, int horizontal_step);
    void        addDepthVertexAtAzimuthElevation(int x, int y);
    ofVboMesh   meshDepth;
    
    Location    loc;
    string      text,street_range,region,country;
    string      data_url;
    string      pano_id;
    double      pano_yaw_deg,tilt_yaw_deg,tilt_pitch_deg;
    double      elevation;
    int         num_zoom_levels,zoom;

    //Depth map information
    int     mapWidth, mapHeight, maxDistance;
    string  depth_map_base64;
    
    //  Flags
    //
    bool    bDataLoaded;
    bool    bPanoLoaded;
    bool    bRegister;
    bool    bTexture;
};
