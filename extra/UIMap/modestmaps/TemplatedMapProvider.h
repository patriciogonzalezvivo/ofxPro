//
//  TemplatedMapProvider.h
//  PrintMaking
//
//  Created by Patricio Gonzalez Vivo on 9/29/13.
//
//

#pragma once

#include "AbstractMapProvider.h"
#include "MercatorProjection.h"

class TemplatedMapProvider : public AbstractMapProvider {
public:
    static MapProviderRef create( const string &urlTemplate ){
        return MapProviderRef( new TemplatedMapProvider( urlTemplate ) );
    }
    
    static MapProviderRef create( const string &urlTemplate, const int &minZoom, const int &maxZoom ){
        return MapProviderRef( new TemplatedMapProvider( urlTemplate, minZoom, maxZoom ) );
    }
    
	int getMaxZoom() {
		return mMaxZoom;
	}
	
	int getMinZoom() {
		return mMinZoom;
	}
    
	vector<string> getTileUrls(const Coordinate &rawCoordinate) {
		vector<string> urls;
		if (rawCoordinate.zoom >= 0 && rawCoordinate.zoom <= 19
			&& rawCoordinate.row >= 0 && rawCoordinate.row < pow(2, rawCoordinate.zoom)) {
			Coordinate coordinate = sourceCoordinate(rawCoordinate);
			string url(mUrlTemplate);
			url.replace(url.find("{Z}"), 3, ofToString(coordinate.zoom));
			url.replace(url.find("{X}"), 3, ofToString(coordinate.column));
			url.replace(url.find("{Y}"), 3, ofToString(coordinate.row));
			urls.push_back(url);
		}
		return urls;
	}
    
protected:
    
    string mUrlTemplate;
    int mMinZoom;
    int mMaxZoom;
    
	TemplatedMapProvider( const string &urlTemplate ):
    mUrlTemplate(urlTemplate), mMinZoom(0), mMaxZoom(18), AbstractMapProvider( MercatorProjection::createWebMercator() ) { }
    
	TemplatedMapProvider( const string &urlTemplate, const int &minZoom, const int &maxZoom ):
    mUrlTemplate(urlTemplate), mMinZoom(minZoom), mMaxZoom(maxZoom), AbstractMapProvider( MercatorProjection::createWebMercator() ) { }
    
};