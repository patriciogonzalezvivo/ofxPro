//
//  UIMap.cpp
//  PrintMaking
//
//  Created by Patricio Gonzalez Vivo on 9/25/13.
//
//

#include "UIMap.h"
#include "QueueSorter.h"

UIMap::UIMap(){
    name = "MAP";
    
    width = 0;
    height = 0;
    
    centerCoordinate.row = 0.5;
    centerCoordinate.column = 0.5;
    centerCoordinate.zoom = 0;
    
    bLoaded = false;
}

UIMap::~UIMap(){
    
};

void UIMap::setup(MapProviderRef _provider ) {
	setMapProvider( _provider );
    
    centerCoordinate.row = 0.5;
    centerCoordinate.column = 0.5;
    centerCoordinate.zoom = 0;
}

void UIMap::setupUI(){
    
    gui->addLabel("Provider");
    vector<string> maps;
    maps.push_back("Bing");
    maps.push_back("Stamen-toner");
    maps.push_back("Stamen-toner-lite");
    maps.push_back("Stamen-toner-background");
    maps.push_back("MapBox");
    maps.push_back("Mgcdn");
    maps.push_back("GeoIp");
    maps.push_back("Modest-BlueMarble");
    maps.push_back("BayArea");
    providers = gui->addRadio("MAPS", maps);
    
    gui->addLabel("Parameters");
    gui->addSlider("Zoom", 2.0, 19.0, &zoom);
    gui->addSlider("Latitud", -90.0, 90.0, &lat);
    gui->addSlider("Longitud", -180.0, 180.0, &lon);
    
    bLoaded = true;
}

void UIMap::loadMap(string _predef){
    if ( _predef == "OpenStreet" ){
        setMapProvider( OpenStreetMapProvider::create() );
        providerName = _predef;
    } else if ( _predef == "Stamen-toner" ){
        setMapProvider( TemplatedMapProvider::create("http://spaceclaw.stamen.com/toner/{Z}/{X}/{Y}.png") );
        providerName = _predef;
    } else if ( _predef == "Stamen-toner-lite" ){
        setMapProvider( TemplatedMapProvider::create("http://spaceclaw.stamen.com/toner-lite/{Z}/{X}/{Y}.png") );
        providerName = _predef;
    } else if ( _predef == "Stamen-toner-background" ){
        setMapProvider( TemplatedMapProvider::create("http://spaceclaw.stamen.com/toner-background/{Z}/{X}/{Y}.png") );
        providerName = _predef;
    } else if ( _predef == "MapBox" ){
        setMapProvider( TemplatedMapProvider::create("http://c.tiles.mapbox.com/v3/examples.map-szwdot65/{Z}/{X}/{Y}.png") );
        providerName = _predef;
    } else if ( _predef == "Mgcdn" ){
        setMapProvider( TemplatedMapProvider::create("http://otile1.mqcdn.com/tiles/1.0.0/osm/{Z}/{X}/{Y}.png") );
        providerName = _predef;
    } else if ( _predef == "GeoIp" ){
        setMapProvider( TemplatedMapProvider::create("http://acetate.geoiq.com/tiles/acetate/{Z}/{X}/{Y}.png") );
        providerName = _predef;
    } else if ( _predef == "Modest-BlueMarble" ){
        setMapProvider( TemplatedMapProvider::create("http://s3.amazonaws.com/com.modestmaps.bluemarble/{Z}-r{Y}-c{X}.jpg") );
        providerName = _predef;
    } else if ( _predef == "BayArea" ){
        setMapProvider( TemplatedMapProvider::create("http://osm-bayarea.s3.amazonaws.com/{Z}-r{Y}-c{X}.jpg") );
        providerName = _predef;
    } else if ( _predef == "Bing" ){
        setMapProvider( BingMapsProvider::create() );
        providerName = _predef;
    }
}

void UIMap::loadCustomValues(ofxXmlSettings &_XML){
    loadMap( _XML.getValue("provider", "Bing") );
    setCenter(Location(_XML.getValue("lat", 0),
                       _XML.getValue("lon", 0) ) );
    setZoom( _XML.getValue("zoom", zoom) );
}

void UIMap::saveCustomValues(ofxXmlSettings &_XML){
    _XML.setValue("zoom", centerCoordinate.zoom );
    _XML.setValue("lat", getCenter().lat );
    _XML.setValue("lon", getCenter().lon );
    _XML.setValue("provider", providerName);
}

void UIMap::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (bLoaded){
        if( name == "Zoom"){
            setZoom( zoom );
        } else if ( name == "lat" || name == "lon"){
            setCenter( Location(lat, lon));
        } else {
            loadMap( name );
        }
    }
}

void UIMap::setUseTexture(bool bUseTex){
    // ...
}

ofTexture& UIMap::getTextureReference(){
    return fbo.getTextureReference();
}

void UIMap::update(){
    if (bEnable){
        
        // if we're in between zoom levels, we need to choose the nearest:
        int baseZoom = ofClamp((int)round(centerCoordinate.zoom), provider->getMinZoom(), provider->getMaxZoom());
        
        // these are the top left and bottom right tile coordinates
        // we'll be loading everything in between:
        Coordinate tl = pointCoordinate(ofPoint(0,0)).zoomTo(baseZoom);
        Coordinate tr = pointCoordinate(ofPoint(width,0)).zoomTo(baseZoom);
        Coordinate bl = pointCoordinate(ofPoint(0,height)).zoomTo(baseZoom);
        Coordinate br = pointCoordinate(ofPoint(width,height)).zoomTo(baseZoom);
        
        // find start and end columns
        int minCol = floor(std::min(std::min(tl.column,tr.column),std::min(bl.column,br.column)));
        int maxCol = floor(std::max(std::max(tl.column,tr.column),std::max(bl.column,br.column)));
        int minRow = floor(std::min(std::min(tl.row,tr.row),std::min(bl.row,br.row)));
        int maxRow = floor(std::max(std::max(tl.row,tr.row),std::max(bl.row,br.row)));
        
        // pad a bit, for luck (well, because we might be zooming out between zoom levels)
        minCol -= GRID_PADDING;
        minRow -= GRID_PADDING;
        maxCol += GRID_PADDING;
        maxRow += GRID_PADDING;
        
        visibleKeys.clear();
        // grab coords for visible tiles
        //
        for (int col = minCol; col <= maxCol; col++) {
            for (int row = minRow; row <= maxRow; row++) {
                
                // source coordinate wraps around the world:
                //Coordinate coord = provider->sourceCoordinate(Coordinate(row,col,baseZoom));
                Coordinate coord = Coordinate(row,col,baseZoom);
                
                // keep this for later:
                visibleKeys.insert(coord);
                
                if (images.count(coord) == 0) {
                    
                    // fetch it if we don't have it
                    grabTile(coord);
                    
                    // see if we have  a parent coord for this tile?
                    bool gotParent = false;
                    for (int i = (int)coord.zoom; i > 0; i--) {
                        Coordinate zoomed = coord.zoomTo(i).container();
                        if (images.count(zoomed) > 0) {
                            visibleKeys.insert(zoomed);
                            gotParent = true;
                            break;
                        }
                    }
                    
                    // or if we have any of the children
                    if (!gotParent) {
                        Coordinate zoomed = coord.zoomBy(1).container();
                        vector<Coordinate> kids;
                        kids.push_back(zoomed);
                        kids.push_back(zoomed.right());
                        kids.push_back(zoomed.down());
                        kids.push_back(zoomed.right().down());
                        for (int i = 0; i < kids.size(); i++) {
                            if (images.count(kids[i]) > 0) {
                                visibleKeys.insert(kids[i]);
                            }
                        }
                    }
                    
                }
                
            } // rows
        } // columns
        
        // TODO: sort by zoom so we draw small zoom levels (big tiles) first:
        // can this be done with a different comparison function on the visibleKeys set?
        //Collections.sort(visibleKeys, zoomComparator);
        
        if (fbo.getWidth() != width || fbo.getHeight() != height ){
            fbo.allocate(width, height);
        }
        
        int numDrawnImages = 0;
        fbo.begin();
        ofPushMatrix();
        ofClear(0,0);
        {
            ofRotate(ofRadToDeg(rotation));
            int numDrawnImages = 0;
            std::set<Coordinate>::iterator citer;
            for (citer = visibleKeys.begin(); citer != visibleKeys.end(); citer++) {
                Coordinate coord = *citer;
                
                float scale = pow(2.0, centerCoordinate.zoom - coord.zoom);
                ofPoint tileSize = provider->getTileSize() * scale;
                ofPoint center = ofPoint(width*0.5,height*0.5);
                Coordinate theCoord = centerCoordinate.zoomTo(coord.zoom);
                
                double tx = center.x + (coord.column - theCoord.column) * tileSize.x;
                double ty = center.y + (coord.row - theCoord.row) * tileSize.y;
                
                if (images.count(coord) > 0) {
                    ofImage *tile = images[coord];
                    // we want this image to be at the end of recentImages, if it's already there we'll remove it and then add it again
                    //			recentImages.erase(remove(recentImages.begin(), recentImages.end(), tile), recentImages.end());
                    std::vector<ofImage*>::iterator result = find(recentImages.begin(), recentImages.end(), tile);
                    if (result != recentImages.end()) {
                        recentImages.erase(result);
                    } else {
                        // if it's not in recent images it must be brand new?
                        tile->setUseTexture(true);
                        //					tile->initTex();
                        tile->update();
                    }

                    tile->draw( tx, ty, tileSize.x, tileSize.y );
                    numDrawnImages++;
                    recentImages.push_back(tile);
                }
            }
        }
        ofPopMatrix();
        fbo.end();
        
        // stop fetching things we can't see:
        // (visibleKeys also has the parents and children, if needed, but that shouldn't matter)
        //queue.retainAll(visibleKeys);
        vector<Coordinate>::iterator iter = queue.begin();
        while (iter != queue.end()) {
            Coordinate key = *iter;
            if (visibleKeys.count(key) == 0){
                iter = queue.erase(iter);
            }
            else {
                ++iter;
            }
        }
        
        // TODO sort what's left by distance from center:
        //
        //queueSorter.setCenter(new Coordinate( (minRow + maxRow) / 2.0f, (minCol + maxCol) / 2.0f, zoom));
        //Collections.sort(queue, queueSorter);
        
        // load up to 4 more things:
        processQueue();
        
        // clear some images away if we have too many...
        int numToKeep = max(numDrawnImages,MAX_IMAGES_TO_KEEP);
        if (recentImages.size() > numToKeep) {
            
            // first clear the pointers from recentImages
            recentImages.erase(recentImages.begin(), recentImages.end()-numToKeep);
            //images.values().retainAll(recentImages);
            
            // TODO: re-think the stl collections used so that a simpler retainAll equivalent is available
            // now look in the images map and if the value is no longer in recent images then get rid of it
            //
            map<Coordinate,ofImage*>::iterator iter = images.begin();
            map<Coordinate,ofImage*>::iterator endIter = images.end();
            for (; iter != endIter;) {
                ofImage* tile = iter->second;
                vector<ofImage*>::iterator result = find(recentImages.begin(), recentImages.end(), tile);
                if (result == recentImages.end()) {
                    images.erase(iter++);
                    delete tile;
                }
                else {
                    ++iter;
                }
            }
        }
	}
}

void UIMap::draw() {
    if (bEnable){
        ofPushStyle();
        ofSetColor(255);
        fbo.draw(x,y);
        ofPopStyle();
    }
}

void UIMap::drawDebug(){
    DraggableRectangle::draw();
}

//---------------------------------------------------------- SET & GET
//
void UIMap::setSize(ofPoint _size) {
    width = _size.x;
    height = _size.y;
}

ofPoint UIMap::getSize() const {
	return ofPoint(width,height);
}

void UIMap::setExtent( const MapExtent &extent, bool forceIntZoom ){
    Coordinate TL = provider->locationCoordinate( extent.getNorthWest() ).zoomTo( getZoom() );
    Coordinate BR = provider->locationCoordinate( extent.getSouthEast() ).zoomTo( getZoom() );
    
    const ofPoint tileSize = provider->getTileSize();
    
    // multiplication factor between horizontal span and map width
    const double hFactor = (BR.column - TL.column) / (width / tileSize.x);
    
    // multiplication factor expressed as base-2 logarithm, for zoom difference
    const double hZoomDiff = log2(hFactor);
    
    // possible horizontal zoom to fit geographical extent in map width
    const double hPossibleZoom = TL.zoom - (forceIntZoom ? ceil(hZoomDiff) : hZoomDiff);
    
    // multiplication factor between vertical span and map height
    const double vFactor = (BR.row - TL.row) / (height / tileSize.y);
    
    // multiplication factor expressed as base-2 logarithm, for zoom difference
    const double vZoomDiff = log2(vFactor);
    
    // possible vertical zoom to fit geographical extent in map height
    const double vPossibleZoom = TL.zoom - (forceIntZoom ? ceil(vZoomDiff) : vZoomDiff);
    
    // initial zoom to fit extent vertically and horizontally
    double initZoom = std::min(hPossibleZoom, vPossibleZoom);
    
    // additionally, make sure it's not outside the boundaries set by provider limits
    initZoom = std::min(initZoom, (double)provider->getMaxZoom());
    initZoom = std::max(initZoom, (double)provider->getMinZoom());
    
    // coordinate of extent center
    const double centerRow = (TL.row + BR.row) / 2.0;
    const double centerColumn = (TL.column + BR.column) / 2.0;
    const double centerZoom = (TL.zoom + BR.zoom) / 2.0;
    setCenter( Coordinate(centerRow, centerColumn, centerZoom).zoomTo(initZoom) );
}

MapExtent UIMap::getExtent() const{
    return MapExtent( pointLocation( ofPoint(0,0) ), pointLocation( getSize() ) );
}

void UIMap::setMapProvider( MapProviderRef _mapProvider ){
//    tileLoader->setMapProvider( _mapProvider );
    images.clear();
    queue.clear();
    recentImages.clear();
    visibleKeys.clear();
    provider = _mapProvider;
}

/** @return zoom level of currently visible tile layer */
double UIMap::getZoom() const {
    return centerCoordinate.zoom;
}

Location UIMap::getCenter() const {
	return provider->coordinateLocation( centerCoordinate );
}

Coordinate UIMap::getCenterCoordinate() const {
    return centerCoordinate;
}

void UIMap::setCenter(const Coordinate &center) {
     centerCoordinate = center;
}

void UIMap::setCenter(const Location &location) {
	setCenter( provider->locationCoordinate(location).zoomTo(getZoom()) );
}

void UIMap::setCenterZoom(const Location &location, const double &_zoom) {
	setCenter( provider->locationCoordinate(location).zoomTo(_zoom) );
}

/** sets scale according to given zoom level, should leave you with pixel perfect tiles */
void UIMap::setZoom(const double &_zoom) {
    centerCoordinate = centerCoordinate.zoomTo(_zoom);
}

//----------------------------------------------------------------- CONVERSION

ofPoint UIMap::coordinatePoint(const Coordinate &target) const {
    
	/* Return an x, y point on the map image for a given coordinate. */
	Coordinate coord = target;
	
	if(coord.zoom != centerCoordinate.zoom) {
		coord = coord.zoomTo(centerCoordinate.zoom);
	}
    
	// distance from the center of the map
    const ofPoint tileSize = provider->getTileSize();
	ofPoint point = ofPoint(width,height) * 0.5;
	point.x += tileSize.x * (coord.column - centerCoordinate.column);
	point.y += tileSize.y * (coord.row - centerCoordinate.row);
    
	ofPoint rotated(point);
    rotated.rotate(rotation, ofPoint(0,0,1));
	
	return rotated;
}

Coordinate UIMap::pointCoordinate(const ofPoint &point) const {
	/* Return a coordinate on the map image for a given x, y point. */
	// new point coordinate reflecting distance from map center, in tile widths
	ofPoint rotated(point);
    const ofPoint tileSize = provider->getTileSize();
	rotated.rotate(-rotation, ofPoint(0,0,1));
	Coordinate coord(centerCoordinate);
	coord.column += (rotated.x - width * 0.5) / tileSize.x;
	coord.row += (rotated.y - height * 0.5) / tileSize.y;
	return coord;
}

ofPoint UIMap::locationPoint(const Location &location) const {
	return coordinatePoint( provider->locationCoordinate(location));
}

Location UIMap::pointLocation(const ofPoint &point) const {
	return provider->coordinateLocation(pointCoordinate(point));
}


//----------------------------------------------------------------- MOVE ARROUND

void UIMap::zoomBy(const double &dir) {
	setZoom( getZoom()+dir );
}

void UIMap::panBy(const ofPoint &delta) { panBy(delta.x, delta.y); }

void UIMap::panBy(const double &dx, const double &dy) {
    const double sinr = sin(rotation);
    const double cosr = cos(rotation);
	const double dxr = dx*cosr + dy*sinr;
	const double dyr = dy*cosr - dx*sinr;
    const ofPoint tileSize = provider->getTileSize();
	centerCoordinate.column -= dxr / tileSize.x;
	centerCoordinate.row -= dyr / tileSize.y;
}
void UIMap::scaleBy(const double &s) {
	scaleBy(s, ofPoint(width,height) * 0.5);
}
void UIMap::scaleBy(const double &s, const ofPoint &c) {
	scaleBy(s, c.x, c.y);
}
void UIMap::scaleBy(const double &s, const double &cx, const double &cy) {
    const double prevRotation = rotation;
	rotateBy(-prevRotation,cx,cy);
    ofPoint center = ofPoint(width,height) * 0.5;
	panBy(-cx+center.x, -cy+center.y);
	centerCoordinate = centerCoordinate.zoomBy(log2(s));
	panBy(cx-center.x, cy-center.y);
	rotateBy(prevRotation,cx,cy);
}

void UIMap::rotateBy(const double &r, const ofPoint &c){
    rotateBy(r,c.x,c.y);
}

void UIMap::rotateBy(const double &r, const double &cx, const double &cy) {
	panBy(-cx, -cy);
	rotation += r;
	panBy(cx, cy);
}

void UIMap::zoomIn() {
    zoomBy(+1);
}

void UIMap::zoomOut() {
    zoomBy(-1);
}

void UIMap::panUp() {
	setCenter(getCenterCoordinate().up());
}
void UIMap::panDown() {
	setCenter(getCenterCoordinate().down());
}
void UIMap::panLeft() {
	setCenter(getCenterCoordinate().left());
}
void UIMap::panRight() {
	setCenter(getCenterCoordinate().right());
}

void UIMap::panAndZoomIn(const Location &location) {
	setCenterZoom(location, getZoom() + 1);
}

void UIMap::panTo(const Location &location) {
	setCenter(location);
}

//------------------------------------------------------------------- MANAGING TILES

void UIMap::grabTile(Coordinate coord) {
	bool isPending = pending.count(coord) > 0;
	bool isQueued = find(queue.begin(), queue.end(), coord) != queue.end();
	bool isAlreadyLoaded = images.count(coord) > 0;
	if (!isPending && !isQueued && !isAlreadyLoaded) {
		queue.push_back(Coordinate(coord));
	}
}

void UIMap::processQueue() {
	if (queue.size() > MAX_PENDING-pending.size()) {
		sort(queue.begin(), queue.end(), QueueSorter(getCenterCoordinate().zoomTo(getZoom())));
	}
    
	while (pending.size() < MAX_PENDING && queue.size() > 0) {
		Coordinate coord = *(queue.begin());
		Coordinate key = Coordinate(coord);
        
		pending[key] = TileLoader();
		pending[key].start(key, provider, this);
        
		queue.erase(queue.begin());
	}  
}

//void UIMap::urlResponse(ofHttpResponse &_response){
//    
//}

// TODO: there could be issues when this is called from within a thread
// probably needs synchronizing on images / pending / queue
void UIMap::tileDone(Coordinate coord, ofImage *img) {
	// check if we're still waiting for this (new provider clears pending)
	// also check if we got something
	if (pending.count(coord) > 0 && img != NULL) {
		images[Coordinate(coord)] = img;
		pending.erase(coord);
	} else {
		pending.erase(coord);
		// try again?
	}
}

// ---------------------------------------------------------- EVENTS
//
void UIMap::keyPressed(int key) {
    if (bEnable){
        if (key == '<' || key == ',' || key == '=' || key == '+' ) {
            if (getZoom() < 19) {
                zoomIn();
            }
        }
        else if (key == '>' || key == '.'|| key == '-' || key == '_') {
            if (getZoom() > 0) {
                zoomOut();
            }
        }
    }
	// TODO: keyboard movement
}

void UIMap::keyReleased(int key) {
	// TODO: keyboard movement
}

void UIMap::mouseDragged(int _x, int _y, int _button) {
    if (bEnable){
        ofPoint mouse = ofPoint(_x,_y);
        ofPoint pMouse = ofPoint(ofGetPreviousMouseX(),ofGetPreviousMouseY());
        
        if (inside(mouse)){
            
            float scale = pow(2.0, centerCoordinate.zoom);
            ofPoint dMouse = (mouse - pMouse);// / scale;

            if (_button == 0) {
                panBy(dMouse);
            } else if (_button == 2) {
                ofPoint mMouse = mouse - (ofPoint(x,y));//+ofPoint(width,height));
                
                if (ofGetKeyPressed( OF_KEY_SHIFT )) {
                    rotateBy(dMouse.x < 0 ? M_PI/72.0 : -M_PI/72.0, mMouse);
                } else {
                    scaleBy(dMouse.y < 0 ? 1.05 : 1.0/1.05, mMouse);
                }
            }
            
            lat = getCenter().lat;
            lon = getCenter().lon;
        }
    }
}

void UIMap::mouseDraggedDebug(ofPoint _mouse){
    if (bEnable){
        ofPoint A = ofPoint(x,y);
        ofPoint B = ofPoint(x+width,y+height);
        
        if ( A.distance( _mouse) < 20 ){
            x += _mouse.x - x;
            y += _mouse.y - y;
            this->set(x,y, width, height);
            
        } else if ( B.distance( _mouse) < 20 ){
            width += _mouse.x - x - width;
            height += _mouse.y - y - height;
            this->set(x,y, width, height);
        }
    }
}

void UIMap::mousePressed(int _x, int _y, int _button) {

}
void UIMap::mouseReleased(int x, int y, int button) {

}