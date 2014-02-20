#pragma once

#include "AbstractMapProvider.h"
#include "MercatorProjection.h"

class OpenStreetMapProvider : public AbstractMapProvider {
public:
	static MapProviderRef create(){
        return MapProviderRef( new OpenStreetMapProvider() );
    }
    
    vector<string> getTileUrls(const Coordinate &rawCoordinate) {
		vector<string> urls;
		if (rawCoordinate.row >= 0 && rawCoordinate.row < pow(2.0, rawCoordinate.zoom)) {
			Coordinate coordinate = sourceCoordinate(rawCoordinate);
			stringstream url;
			string subdomain = subdomains[(int)ofRandom(0, subdomains.size())];
			url << "http://" << subdomain << "tile.openstreetmap.org/" << (int)coordinate.zoom << "/" << (int)coordinate.column << "/" << (int)coordinate.row << ".png";
			cout << rawCoordinate << " --> " << url.str() << endl;
			urls.push_back(url.str());
		}
		return urls;
	}
    
private:
	
	vector<string> subdomains;
	
	OpenStreetMapProvider() : AbstractMapProvider( MercatorProjection::createWebMercator() ) {
		subdomains.push_back("");
		subdomains.push_back("a.");
		subdomains.push_back("b.");
		subdomains.push_back("c.");
	}
	
};