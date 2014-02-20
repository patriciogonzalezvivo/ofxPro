//
//  UIMap.h
//
//  Created by Patricio Gonzalez Vivo on 9/25/13 based
//  Tom Carden's ( https://github.com/RandomEtc/modestmaps-of ) ModestMap API (http://modestmaps.com/ ) for openFrameworks
//

#pragma once

#include "UIClass.h"
#include "DraggableRectangle.h"

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
#define GRID_PADDING 0

class UIMap : public UIClass, public DraggableRectangle, public ofBaseHasTexture {
public:
    
    UIMap();
    
    void    setupUI();
    void    setup( MapProviderRef _provider);
    
    void    loadMap(string _predef);
    void    loadCustomValues(ofxXmlSettings &_XML);
    void    saveCustomValues(ofxXmlSettings &_XML);
    
    void    setSize(ofPoint _size);
    void    setSize(double _width, double _height);
    void    setMapProvider( MapProviderRef _provider);
    void    setExtent( const MapExtent &extent, bool forceIntZoom );
    
    void    setZoom(const double &_zoom);
    void    setCenter(const Coordinate &center);
	void    setCenter(const Location &location);
	void    setCenterZoom(const Location &location, const double &_zoom);
    void    setUseTexture(bool bUseTex);
    
    double      getZoom() const;
    ofPoint     getSize() const;
	MapExtent   getExtent() const;
    Location    getCenter() const;
	Coordinate  getCenterCoordinate() const;
    
    string      getClassName(){ return name; }
    ofTexture&  getTextureReference();
    
    //  Helpers
    //
    ofPoint     coordinatePoint(const Coordinate &coord) const;
	Coordinate  pointCoordinate(const ofPoint &point) const;
	ofPoint     locationPoint(const Location &location) const;
	Location    pointLocation(const ofPoint &point) const;
    
    void    update();
	void    draw();
    void    drawDebug();
    
    //  Accions
    //
    void    panTo(const Location &location);
    
    void    zoomBy(const double &dir);
    void    panBy(const double &x, const double &y);
	void    panBy(const ofPoint &d);
    void    scaleBy(const double &s);
	void    scaleBy(const double &s, const double &x, const double &y);
	void    scaleBy(const double &s, const ofPoint &c);
	void    rotateBy(const double &r, const double &x, const double &y);
	void    rotateBy(const double &r, const ofPoint &c);
    
    void    zoomIn();
	void    zoomOut();
    
    void    panUp();
	void    panDown();
	void    panLeft();
	void    panRight();
    
    void    panAndZoomIn(const Location &location);
    
	void    grabTile(Coordinate coord);
	void    tileDone(Coordinate coord, ofImage *img);
	void    processQueue();
    
    //  Events
    //
	void    keyPressed(int key);
	void    keyReleased(int key);
	void    mouseDragged(int x, int y, int button);
    void    mouseDraggedDebug(ofPoint _mouse);
	void    mousePressed(int x, int y, int button);
	void    mouseReleased(int x, int y, int button);
	
protected:
    void    guiEvent(ofxUIEventArgs &e);
    
	// loading tiles
	map<Coordinate, TileLoader> pending;
	// loaded tiles
	map<Coordinate, ofImage*> images;
	// coords waiting to load
	vector<Coordinate> queue;
	// a list of the most recent MAX_IMAGES_TO_KEEP ofImages we've seen
	vector<ofImage*> recentImages;
	// keep track of what we can see already:
    std::set<Coordinate> visibleKeys;
	
    // pan and zoom
    ofFbo   fbo;
    
    // pan and zoom
	Coordinate  centerCoordinate;
    
    // what kinda maps?
	MapProviderRef provider;

    // angle in radians
    //
	double      rotation;
    
    string      providerName;
    float       zoom;
    float       lat,lon;
    bool        bLoaded;
};