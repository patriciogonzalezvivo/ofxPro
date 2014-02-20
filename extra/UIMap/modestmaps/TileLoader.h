#ifndef TILE_LOADER
#define TILE_LOADER

#include "ofMain.h"
#include "ofThread.h"
#include "Coordinate.h"
#include "SimpleWebScraper.h"

class AbstractMapProvider;
class UIMap;

class TileLoader {
	
public:
	
    Coordinate coord;
	MapProviderRef provider;
	UIMap *theMap;
	
	void start(Coordinate _coord, MapProviderRef _provider, UIMap *_map);

	SimpleWebScraper sws;	
		
	// callback events ----------------------------------------------------------
	void onTextReceived(const void* pSender, string& response);
	void onUnthreadedStreamReceived(const void* pSender, StreamEventArgs & args_);	
	void onThreadedStreamReceived(const void* pSender, StreamEventArgs & args_);
		
};

	
#endif