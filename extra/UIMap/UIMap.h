//
//  UIMap.h
//
//  Created by Patricio Gonzalez Vivo on 9/25/13 based
//  Tom Carden's ( https://github.com/RandomEtc/modestmaps-of ) ModestMap API (http://modestmaps.com/ ) for openFrameworks
//

#pragma once

#include "UIClass.h"

#include "AbstractMapProvider.h"
#include "OpenStreetMapProvider.h"
#include "BingMapsProvider.h"
#include "TemplatedMapProvider.h"

#include "Location.h"
#include "Coordinate.h"
#include "TileLoader.h"

#include "MapExtent.h"

// limit simultaneous calls to loadImage
#define MAX_PENDING  8

// limit tiles in memory
// 256 would be 64 MB, you may want to lower this quite a bit for your app
// (we'll always keep as many images as needed to fill the screen though)
#define MAX_IMAGES_TO_KEEP 256

// upping this can help appearances when zooming out, but also loads many more tiles
#define GRID_PADDING 1

class UIMap : public UIClass, public ofBaseHasTexture {
public:
    
    UIMap();
    virtual ~UIMap();
    
    void    setupUI();
    
    void    allocate(float _width, float _height);
    
    void    loadMap(MapProviderRef _provider);
    void    loadMap(string _predef);
    
    void    setMapProvider( MapProviderRef _provider);
    void    setExtent( const MapExtent &extent, bool forceIntZoom );
    
    void    setZoom(const double &_zoom);
    void    setCenter(const Coordinate &center);
	void    setCenter(const Location &location);
    void    setUseTexture(bool bUseTex);
    
    double      getAngle(){return rotation;};
    double      getZoom() const;
	MapExtent   getExtent() const;
    Location    getCenter() const;
    Coordinate  getCenterCoordinate(){return centerCoordinate;};
    
    string      getClassName(){ return "Map";}
    
    float       getWidth(){return width;}
	float       getHeight(){return height;};
    ofTexture&  getTextureReference();
    
    //  Conversions
    //
    ofPoint     coordinatePoint(const Coordinate &coord) const;
	Coordinate  pointCoordinate(const ofPoint &point) const;
	ofPoint     locationPoint(const Location &location) const;
	Location    pointLocation(const ofPoint &point) const;
    
    //  Actions
    void    panBy(const ofPoint &_dir);
    void    scaleBy(const double &_scale, const ofPoint &_pos);
    void    rotateBy(const double &_radians, const ofPoint &_pos);
    
    void    tileDone(Coordinate coord, ofImage *img);
    
    void    update();
    
	void    draw(float _x=0, float _y=0,float _width = -1, float _height=-1);
    
protected:
    void    guiEvent(ofxUIEventArgs &e);
    void    grabTile(Coordinate coord);
    
	map<Coordinate, TileLoader> pending;        // loading tiles
	map<Coordinate, ofImage*>   images;         // loaded tiles
	vector<Coordinate>          queue;          // coords waiting to load
	vector<ofImage*>            recentImages;   // a list of the most recent MAX_IMAGES_TO_KEEP ofImages we've seen
    std::set<Coordinate>        visibleKeys;    // keep track of what we can see already:
	
    MapProviderRef              provider;
    ofxUIRadio                  *providers;
    
    ofFbo       fbo;
    Coordinate  centerCoordinate;
	double      rotation;
    float       width, height;
    float       zoom,lat,lon;
};