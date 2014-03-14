//
//  StreetView.cpp
//
//  Created by Patricio Gonzalez Vivo on 3/10/14.
//
//

#include "StreetView.h"

#include "base64.h"
#include <zlib.h>

StreetView::StreetView(){
    ofRegisterURLNotification(this);
    loc.lat = 0;
    loc.lon = 0;
    pano_id = "";
    depth_map_base64 = "";
    
    bDataLoaded = false;
    bPanoLoaded = false;
    bAutoDownload = false;
}

StreetView::StreetView(string _pano_id){
    ofRegisterURLNotification(this);
    bAutoDownload = false;
    setPanoId(_pano_id);
}

StreetView::StreetView(double _lat, double _lon){
    ofRegisterURLNotification(this);
    bAutoDownload = false;
    setLatLon(_lat,_lon);
}

StreetView::StreetView(Location _location){
    ofRegisterURLNotification(this);
    bAutoDownload = false;
    setLocation(_location);
}

void StreetView::setAutoDownload(bool _enable){
    bAutoDownload = _enable;
}

void StreetView::setLocation(Location _loc){
    setLatLon(_loc.lat,_loc.lon);
}

void StreetView::setLatLon(double _lat, double _lon){
    clear();
    string data_url = "http://cbk0.google.com/cbk?output=xml&ll="+ofToString(_lat)+","+ofToString(_lon)+"&dm=1&pm=1";
    ofLoadURLAsync(data_url);
}

void StreetView::setPanoId(string _pano_id){
    clear();
    string data_url = "http://cbk0.google.com/cbk?output=xml&panoid="+_pano_id+"&dm=1&pm=1";
    ofLoadURLAsync(data_url);
}

StreetView::~StreetView(){
    ofUnregisterURLNotification(this);
    clear();
}

void StreetView::clear(){
    loc.lat = 0;
    loc.lon = 0;
    bDataLoaded = false;
    bPanoLoaded = false;
    pano_id.clear();
    depth_map_base64.clear();
    images.clear();
    links.clear();
    if(fbo.isAllocated()){
        fbo.begin();
        ofClear(0,0);
        fbo.end();
    }
}

void StreetView::downloadPanorama(){
    if(!bPanoLoaded){
        if(pano_id != ""){
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 7; j++){
                    ofLoadURLAsync("http://cbk0.google.com/cbk?output=tile&panoid="+pano_id+"&zoom=3&x="+ofToString(j)+"&y="+ofToString(i));
                }
            }
        }
        
        bool bDepth = false;
        bool bPano = false;
        
        //Decode the depth map
        //The depth map is encoded as a series of pixels in a 512x256 image. Each pixels refers
        //to a depthMapPlane which are also encoded in the data. Each depthMapPlane has three elements:
        //The x,y,z normals and the closest distance the plane has to the origin. This uniquely
        //identifies the plane in 3d space.
        //
        if(depth_map_base64 != ""){
            //Decode base64
            vector<unsigned char> depth_map_compressed(depth_map_base64.length());
            int compressed_length = decode_base64(&depth_map_compressed[0], &depth_map_base64[0]);
            
            //Uncompress data with zlib
            //TODO: decompress in a loop so we can accept any size
            unsigned long length = 512 * 256 + 5000;
            vector<unsigned char> depth_map(length);
            int zlib_return = uncompress(&depth_map[0], &length, &depth_map_compressed[0], compressed_length);
            if (zlib_return != Z_OK)
                throw "zlib decompression of the depth map failed";
            
            //Load standard data
            const int headersize = depth_map[0];
            const int numPanos = depth_map[1] | (depth_map[2] << 8);
            mapWidth = depth_map[3] | (depth_map[4] << 8);
            mapHeight = depth_map[5] | (depth_map[6] << 8);
            const int panoIndicesOffset = depth_map[7];
            
            if (headersize != 8 || panoIndicesOffset != 8)
                throw "Unexpected depth map header";
            
            //Load depthMapIndices
            depthmapIndices = vector<unsigned char>(mapHeight * mapWidth);
            memcpy(&depthmapIndices[0], &depth_map[panoIndicesOffset], mapHeight * mapWidth);
            
            //Load depthMapPlanes
            depthmapPlanes = vector<DepthMapPlane> (numPanos);
            memcpy(&depthmapPlanes[0], &depth_map[panoIndicesOffset + mapHeight * mapWidth], numPanos * sizeof (struct DepthMapPlane));
            
            bDepth = true;
        }
        
        //Decode the pano map
        if(pano_map_base64!=""){
            
            //Decode base64
            vector<unsigned char> pano_map_compressed(pano_map_base64.length());
            int compressed_length = decode_base64(&pano_map_compressed[0], pano_map_base64.c_str());
            
            //Uncompress data with zlib
            unsigned long length = mapWidth * mapHeight + 5000;
            vector<unsigned char> pano_map(length);
            int zlib_return = uncompress(&pano_map[0], &length, &pano_map_compressed[0], compressed_length);
            if (zlib_return != Z_OK)
                throw "zlib decompression of the pano map failed";
            
            //Load standard data
            const int headersize = pano_map[0];
            const int numPanos = pano_map[1] | (pano_map[2] << 8);
            const int panomapWidth = pano_map[3] | (pano_map[4] << 8);
            const int panomapHeight = pano_map[5] | (pano_map[6] << 8);
            const int panoIndicesOffset = pano_map[7];
            
            if (headersize != 8 || panoIndicesOffset != 8
                || panomapWidth != mapWidth || panomapHeight != mapHeight)
                throw "Unexpected pano map header";
            
            //Load panomap indices
            panomapIndices = vector<unsigned char>(mapWidth * mapHeight);
            memcpy(&panomapIndices[0], &pano_map[panoIndicesOffset], mapWidth * mapHeight);
            
            //Load pano-ids
            //Remember: the panoids array is 0-indexed,
            //while the pano id's in panomapIndices are 1-indexed (0 being reserved for sky)
            //Don't forget to subtract 1 from the index when accessing panoids!
            const int panoid_offset = panoIndicesOffset + mapWidth*mapHeight;
            memset(&panoids, '\0', (PANOID_LENGTH + 1) * numPanos); //Makes sure all strings are properly \0 terminated
            
            for (int i = 0; i < numPanos; i++) {
                memcpy(&panoids[i], &pano_map[panoid_offset + PANOID_LENGTH * i], PANOID_LENGTH);
                
                //All panomap indices have some pixels that refer to themselves,
                //Usually the index is 1 or 2 (0 is reserved for sky and other infinite depth spots)
                if (memcmp(&panoids[i], &pano_id, PANOID_LENGTH + 1) == 0)
                    ownPanomapIndex = i + 1;
            }
            
            bPano = true;
        }
        
        if(bDepth && bPano){
            updateModel();
        }
    }
}

void StreetView::urlResponse(ofHttpResponse & response){
    if(response.status==200 && response.request.url.find("http://cbk0.google.com/cbk?output=xml") == 0 && !bDataLoaded){
        images.clear();
        
        ofxXmlSettings  XML;
        XML.loadFromBuffer(response.data);
        
        pano_id = XML.getAttribute("panorama:data_properties", "pano_id", "");
        loc.lat = XML.getAttribute("panorama:data_properties", "original_lat", 0.0);
        loc.lon = XML.getAttribute("panorama:data_properties", "original_lng", 0.0);
        num_zoom_levels = XML.getAttribute("panorama:data_properties", "num_zoom_levels", 0);
        
        pano_yaw_deg = XML.getAttribute("panorama:projection_properties", "pano_yaw_deg", 0.0);
        tilt_yaw_deg = XML.getAttribute("panorama:projection_properties", "tilt_yaw_deg", 0.0);
        tilt_pitch_deg = XML.getAttribute("panorama:projection_properties", "tilt_pitch_deg", 0.0);
        
        //Get the base64 encoded data
        depth_map_base64 = XML.getValue("panorama:model:depth_map", "");
        pano_map_base64 = XML.getValue("panorama:model:pano_map", "");

        XML.pushTag("panorama");
        XML.pushTag("annotation_properties");
        int nLinks = XML.getNumTags("link");
        for (int i = 0; i < nLinks; i++) {
            Link l;
            l.pano_id = XML.getAttribute("link", "pano_id", "");
            l.yaw_deg = XML.getAttribute("link", "yaw_deg", 0.0f);
            links.push_back(l);
            XML.removeTag("link",0);
        }
        XML.popTag();
        XML.popTag();
        
        bDataLoaded = true;
        
        if(bAutoDownload){
            downloadPanorama();
        }
        
    } else if(response.status==200 && response.request.url.find("http://cbk0.google.com/cbk?output=tile") == 0){
        ofImage img;
        img.loadImage(response.data);
        images.push_back(img);
    }
}

bool StreetView::isVisible(int x, int y) {
    
    //Make sure the pixel is on the map
    if (x < 0) x += mapWidth;
    if (y < 0) y += mapHeight;
    if (x >= mapWidth) x -= mapWidth;
    if (y >= mapHeight) y -= mapHeight;
    
    int panoIndex = panomapIndices[y * mapWidth + x];
    int depthMapIndex = depthmapIndices[y * mapWidth + x];
    
    return panoIndex == ownPanomapIndex && depthMapIndex != 0;
}

bool StreetView::isTransparant(int x, int y, int horizontal_step) {
    if (y > 0 && y < mapHeight - 1) {
        for (int _x = -horizontal_step; _x <= horizontal_step; _x += horizontal_step) {
            for (int _y = -1; _y <= 1; _y++) {
                if (!isVisible(x + _x, y + _y))
                    return true;
                
            }
        }
    }
    return false;
}

Location StreetView::getLocation(){
    return loc;
}

string StreetView::getCloseLinkTo(float _deg){
    
}

float StreetView::getGroundHeight() {
    int groundIndex = depthmapIndices[mapHeight * mapWidth - 1];
    return depthmapPlanes[groundIndex].d;
}

float StreetView::getWidth(){
    if(fbo.isAllocated()){
        return fbo.getWidth();
    }
}

float StreetView::getHeight(){
    if(fbo.isAllocated()){
        return fbo.getHeight();
    }
}

ofTexture& StreetView::getTextureReference(){
    if(fbo.isAllocated()){
        return fbo.getTextureReference();
    }
}

ofTexture StreetView::getLookAt(float _deg, float _amp){
    return getLookTo(pano_yaw_deg-360-_deg+90,_amp);
}

ofTexture StreetView::getLookTo(float _deg, float _amp){
    ofTexture rta;
    
    if(fbo.isAllocated()){
        float widthDeg = fbo.getWidth()/360.0;
        
        float offsetX = widthDeg*_deg;
        float amplitud = widthDeg*_amp;
        
        ofFbo roi;
        roi.allocate(widthDeg*_amp, fbo.getHeight());
        rta.allocate(widthDeg*_amp, fbo.getHeight(),GL_RGB);
        
        roi.begin();
        ofClear(0,0);
        fbo.draw(-offsetX+amplitud*0.5-fbo.getWidth(),0);
        fbo.draw(-offsetX+amplitud*0.5,0);
        if(offsetX+amplitud>fbo.getWidth()){
            fbo.draw(-offsetX+amplitud*0.5+fbo.getWidth(),0);
        }
        roi.end();
        roi.draw(0, 0);
        
        rta = roi.getTextureReference();
    }
    return rta;
}

void StreetView::update(){
    if(bDataLoaded && !bPanoLoaded && images.size() >= 3*7){
    
        fbo.allocate(3335,1778);
//        fbo.allocate(images[0].width*7, images[0].height*3);
        
        fbo.begin();
        ofClear(0,0);
        int x = 0;
        int y = 0;
        for(int i= 0; i < images.size(); i++){
            images.at(i).draw(x*images.at(i).width, y*images.at(i).height);
            if(x < 6){
                x++;
            }else{
                x=0;
                y++;
            }
        }
        fbo.end();
        
        bPanoLoaded = true;
    }
}

void StreetView::updateModel(){
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    for (int x = 0; x < mapWidth - 1; x += 1) {
        const int next_x = x + 1;
        int map_next_x = next_x;
        if (map_next_x >= mapWidth)
            map_next_x -= mapWidth;
        
        const unsigned int endHeight = mapHeight - 1;
        
        for (unsigned int y = 0; y < endHeight; y += 1) {
            const int next_y = y + 1;
            
            addVertexAtAzimuthElevation(next_x, y);
            addVertexAtAzimuthElevation(x, y);
            addVertexAtAzimuthElevation(x, next_y);
            
            addVertexAtAzimuthElevation(x, next_y);
            addVertexAtAzimuthElevation(next_x, next_y);
            addVertexAtAzimuthElevation(next_x, y);
            
        }
    }
}

void StreetView::addVertexAtAzimuthElevation(int x, int y){
    float rad_azimuth = x / (float) (mapWidth - 1.0f) * TWO_PI;
    float rad_elevation = y / (float) (mapHeight - 1.0f) * PI;
    
    //Calculate the cartesian position of this vertex (if it was at unit distance)
    ofPoint xyz;
    xyz.x = sin(rad_elevation) * sin(rad_azimuth);
    xyz.y = sin(rad_elevation) * cos(rad_azimuth);
    xyz.z = cos(rad_elevation);
    float distance = 1;
    
    ofPoint normal;
    
    //Value that is safe to use to retrieve stuff from the index arrays
    const int map_x = x % mapWidth;
    
    //Calculate distance of point according to the depth map data.
    int depthMapIndex = depthmapIndices[y * mapWidth + map_x];
    if (depthMapIndex == 0) {
        //Distance of sky
        distance = 100;
        
        normal = ofPoint(0,0,0)-xyz;
        normal.normalize();
        
    } else {
        DepthMapPlane plane = depthmapPlanes[depthMapIndex];
        distance = -plane.d / (plane.x * xyz.x + plane.y * xyz.y + -plane.z * xyz.z);
        normal.set(plane.x,plane.y,plane.z);
    }
    
    ofVec2f texCoord = ofVec2f((x/(float)mapWidth)*3335.0,(y/(float)mapHeight)*1778.0);
    xyz *= distance;
    
//    mesh.addColor(ofFloatColor(1,0.5));//x/(float)mapWidth,y/(float)mapHeight,0.0));

    mesh.addNormal(normal);
    mesh.addTexCoord(texCoord);
    mesh.addVertex(xyz);
}

void StreetView::draw(){
    if(bPanoLoaded&&fbo.isAllocated()){
        ofSetColor(255);
        fbo.getTextureReference().bind();
        mesh.draw();
        fbo.getTextureReference().unbind();
    }
}

