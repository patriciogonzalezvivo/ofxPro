//
//  BingMapsProvider.h
//  PrintMaking
//
//  Created by Patricio Gonzalez Vivo on 9/29/13.
//
//

#pragma once

#include "AbstractMapProvider.h"
#include "MercatorProjection.h"

class BingMapsProvider : public AbstractMapProvider {
public:
    static MapProviderRef create() {
        return MapProviderRef(new BingMapsProvider());
    }
    
    vector<string> getTileUrls(const Coordinate &rawCoordinate) {
		std::vector<std::string> urls;
		if (rawCoordinate.zoom >= 1 && rawCoordinate.zoom <= 19
			&& rawCoordinate.row >= 0 && rawCoordinate.row < pow(2, rawCoordinate.zoom)) {
			Coordinate coordinate = sourceCoordinate(rawCoordinate);
			std::stringstream url;
			std::string subdomain = subdomains[(int)ofRandom(0, subdomains.size())];
			std::string quadkey = getQuadKey((int)coordinate.column, (int)coordinate.row, (int)coordinate.zoom);
			url << "http://" << subdomain << ".tiles.virtualearth.net/tiles/a" << quadkey << ".jpeg?g=543"; //&mkt={culture}&token={token}";
			urls.push_back(url.str());
		}
		return urls;
	}
    
//private:
    std::string getQuadKey(const int column, const int row, const int zoom) {
		std::stringstream key;
		for (int i = 1; i <= zoom; i++) {
			int digit = (((row >> (zoom - i)) & 1) << 1) | ((column >> (zoom - i)) & 1);
			key << digit;
		}
		return key.str();
	}
    
	vector<string> subdomains;
	
	BingMapsProvider(): AbstractMapProvider( MercatorProjection::createWebMercator() ) {
		subdomains.push_back("t0");
		subdomains.push_back("t1");
		subdomains.push_back("t2");
		subdomains.push_back("t3");
	}
};