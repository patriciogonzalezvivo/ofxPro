#include "AbstractMapProvider.h"
#include "TileLoader.h"
#include "UIMap.h"

void TileLoader::start(Coordinate _coord, MapProviderRef _provider, UIMap *_map) {
	coord = _coord;
	provider = _provider;
	theMap = _map;
	
	sws.threadedStreamReady += Delegate<TileLoader, StreamEventArgs>(this, &TileLoader::onThreadedStreamReceived);
	
	vector<string> urls = provider->getTileUrls(coord);
    
	if (urls.size() > 0) {
		// TODO: more than one URL?
		sws.getStreamThreaded(urls[0]);
	}
	else {
		_map->tileDone(coord, NULL);
	}
}

void TileLoader::onThreadedStreamReceived(const void* pSender, StreamEventArgs & args_) {
	
	ofMemoryImage* threadedMemImage = new ofMemoryImage();
	threadedMemImage->setUseTexture(false); // we can't use a texture with this one
	threadedMemImage->loadFromData(args_.buff,args_.bytesToRead);
	// cleanup:
	delete args_.buff;
	
	theMap->tileDone(coord, threadedMemImage);
}