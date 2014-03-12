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
    float       getGroundHeight();

    float       getWidth();
	float       getHeight();
    ofTexture&  getTextureReference();
    
    bool        isTextureLoaded(){return bPanoLoaded;};
    void        downloadPanorama();
    void        urlResponse(ofHttpResponse & response);
    
    void        clear();
    void        update();
    
    vector<Link> links;
    
    void    draw();
    
protected:
    bool    isVisible(int x, int y);
    bool    isTransparant(int x, int y, int horizontal_step);
    
    void    draw(UtmPosition referencePoint, bool drawAll);
    void    drawActual(UtmPosition referencePoint, bool drawAll, RenderSettings settings);
    void    drawVertexAtAzimuthElevation(int x, int y, RenderSettings settings);
    
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
    
    RenderSettings  compiledRenderSettings;
    UtmPosition     utmLoc;
    int compileList;
    int threeSixtyCompileList;
    int mipmapping;
    int wireframe;
    int horizontal_accuracy;
    int vertical_accuracy;
    int zoom_level;
    
    //  Flags
    //
    bool    bAutoDownload;
    bool    bDataLoaded;
    bool    bPanoLoaded;
};
