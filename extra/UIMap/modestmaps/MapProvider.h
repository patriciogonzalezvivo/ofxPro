//
//  MapProvider.h
//
//  Created by Patricio Gonzalez Vivo on 9/30/13.
//
//

#pragma once

#include "ofMain.h"

#include "Location.h"
#include "Coordinate.h"

class MapProvider {
public:
    
    virtual ~MapProvider() { std::cout << "Map Provider destroyed" << std::endl; }
    virtual vector<string> getTileUrls(const Coordinate &coord )=0;
    
    // facts about the tiles that exist:
    virtual ofPoint getTileSize() = 0;
    virtual int getMaxZoom() = 0;
    virtual int getMinZoom() = 0;
    
    // how the map turns geography into tiles:
    virtual Coordinate locationCoordinate(const Location &location) = 0;
    virtual Location coordinateLocation(const Coordinate &coordinate) = 0;
    
//    // called from a background thread:
    virtual ofImage getImage( const Coordinate &coord ) = 0;
};

typedef std::shared_ptr<MapProvider> MapProviderRef;
