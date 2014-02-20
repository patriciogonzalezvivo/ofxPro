#pragma once

#include "MapProvider.h"
#include "AbstractProjection.h"
#include "Coordinate.h"
#include "Location.h"

class AbstractMapProvider : public MapProvider{
public:
	
	ProjectionRef   projection;
	
	AbstractMapProvider(ProjectionRef _projection): projection(_projection) {}
    
	virtual ~AbstractMapProvider() { cout << "Abstract Map Provider destroyed" << endl; }
	virtual vector<string> getTileUrls(const Coordinate &coord)=0;
    
    ofPoint getTileSize() {
		return ofPoint(256,256);
	}
    
    int getMaxZoom() {
		return 18;
	}
	
	int getMinZoom() {
		return 0;
	}
    
	Coordinate locationCoordinate(const Location &location) {
		return projection->locationCoordinate(location);
	}
	
	Location coordinateLocation(const Coordinate &coordinate) {
		return projection->coordinateLocation(coordinate);
	}
	
	Coordinate sourceCoordinate(const Coordinate &coordinate) {
		double gridSize = pow(2.0, coordinate.zoom);
		
		double wrappedColumn = coordinate.column;
		if(wrappedColumn >= gridSize) {
			wrappedColumn = fmod(wrappedColumn, gridSize);
		} else {
			while (wrappedColumn < 0) {
				wrappedColumn += gridSize;
			}
		}
		
		return Coordinate(coordinate.row, wrappedColumn, coordinate.zoom);
	}
    
    ofImage getImage( const Coordinate &coord ) {
        std::vector<std::string> urls = getTileUrls(coord);
        
        ofImage image;
        
        if (urls.size() > 0) {
//            try {
//                image = Surface( loadImage( loadUrl( urls[0] ) ) );
//            }
//            catch( ... ) {
//                //std::cout << "Failed to load: " << url.str() << std::endl;
//                // create a dummy tile
//                Vec2i size = getTileSize();
//                image = Surface( size.x, size.y, true );
//                ip::fill( &image, Color( 1.0f, 0.0f, 0.0f ) );
//            }
        }
        
        return image;
    }
	
};