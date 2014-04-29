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
    clear();
    maxDistance = 200;
    bRegister = false;
    bTexture = true;
    mapWidth = 512;
    mapHeight = 256;
    num_zoom_levels = 3;
    zoom = 1;
}

StreetView::StreetView(string _pano_id){
    StreetView();
    setPanoId(_pano_id);
}

StreetView::StreetView(double _lat, double _lon){
    StreetView();
    setLatLon(_lat,_lon);
}

StreetView::StreetView(Location _location){
    StreetView();
    setLocation(_location);
}

StreetView::~StreetView(){
    if(bRegister){
        ofUnregisterURLNotification(this);
        bRegister = false;
    }
}

void StreetView::clear(){
    loc.lat = 0;
    loc.lon = 0;
    bDataLoaded = false;
    bPanoLoaded = false;
    
    pano_id.clear();
    depth_map_base64.clear();
    panoImages.clear();
    links.clear();
    if(panoFbo.isAllocated()){
        panoFbo.begin();
        ofClear(0,0);
        panoFbo.end();
    }
}

void StreetView::setUseTexture(bool _bUseTex){
    if(_bUseTex&&bDataLoaded&&!bPanoLoaded){
        downloadPanorama();
    }
    bTexture = _bUseTex;
}

void StreetView::setLocation(Location _loc){
    setLatLon(_loc.lat,_loc.lon);
}

void StreetView::setLatLon(double _lat, double _lon){
    if(!bRegister){
        ofRegisterURLNotification(this);
        bRegister = true;
    }
    
    clear();
    data_url = "http://cbk0.google.com/cbk?output=xml&ll="+ofToString(_lat)+","+ofToString(_lon)+"&dm=1";
    ofLoadURLAsync(data_url);
}

void StreetView::setPanoId(string _pano_id){
    if(!bRegister){
        ofRegisterURLNotification(this);
        bRegister = true;
    }
    
    if(_pano_id!=pano_id){
        clear();
        pano_id = _pano_id;
        data_url = "http://cbk0.google.com/cbk?output=xml&panoid="+pano_id+"&dm=1";
        ofLoadURLAsync(data_url);
    }
}

void StreetView::setZoom(int _zoom){
    zoom = _zoom;
    panoFbo.allocate(getWidth(),getHeight());
    
    if(zoom>num_zoom_levels){
        zoom = num_zoom_levels;
    }
    
    if (bPanoLoaded) {
        bPanoLoaded = false;
        downloadPanorama();
    }
}

void StreetView::urlResponse(ofHttpResponse & response){
//    cout << response.request.url << endl;
//    cout << "Status " << response.status << endl;
    
    if((response.status==200) && (response.request.url == data_url )&& (!bDataLoaded)){
        panoImages.clear();
        
        ofxXmlSettings  XML;
        XML.loadFromBuffer(response.data);
        
        pano_id = XML.getAttribute("panorama:data_properties", "pano_id", "");
        text = XML.getValue("panorama:data_properties:text", "");
        street_range = XML.getValue("panorama:data_properties:street_range",  "");
        region = XML.getValue("panorama:data_properties:region", "");
        country = XML.getValue("panorama:data_properties:country", "");
        
//        loc.lat = XML.getAttribute("panorama:data_properties", "original_lat", 0.0);
//        loc.lon = XML.getAttribute("panorama:data_properties", "original_lng", 0.0);
        loc.lat = XML.getAttribute("panorama:data_properties", "lat", 0.0);
        loc.lon = XML.getAttribute("panorama:data_properties", "lng", 0.0);
        elevation = XML.getAttribute("panorama:data_properties", "elevation_wgs84_m", 0.0);
        num_zoom_levels = XML.getAttribute("panorama:data_properties", "num_zoom_levels", 0);
        if(zoom>num_zoom_levels){
            zoom = num_zoom_levels;
        }
        pano_yaw_deg = XML.getAttribute("panorama:projection_properties", "pano_yaw_deg", 0.0);
        tilt_yaw_deg = XML.getAttribute("panorama:projection_properties", "tilt_yaw_deg", 0.0);
        tilt_pitch_deg = XML.getAttribute("panorama:projection_properties", "tilt_pitch_deg", 0.0);
        
        //Get the base64 encoded data
        depth_map_base64 = XML.getValue("panorama:model:depth_map", "");

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
        }
        
        bDataLoaded = true;
        
        if(bTexture){
            downloadPanorama();
        }
    
    } else if(response.request.url.find("http://cbk0.google.com/cbk?output=tile&panoid="+pano_id) == 0){// && response.status==200 ){
        ofImage img;
        img.loadImage(response.data);
        panoImages.push_back(img);
    }
}

void StreetView::downloadPanorama(){
    if(!bPanoLoaded){
        if(pano_id != ""){
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 7; j++){
                    ofLoadURLAsync("http://cbk0.google.com/cbk?output=tile&panoid="+pano_id+"&zoom="+ofToString(zoom)+"&x="+ofToString(j)+"&y="+ofToString(i));
                }
            }
        }
        
        if(depth_map_base64 != ""){
            makeDepthMesh();
        }
    }
}

void StreetView::makeDepthMesh(){
    meshDepth.clear();
    meshDepth.setMode(OF_PRIMITIVE_TRIANGLES);    
    for (int x = 0; x < mapWidth - 1; x ++) {
        const int next_x = x + 1;
        int map_next_x = next_x;
        if (map_next_x >= mapWidth)
            map_next_x -= mapWidth;
        
        const unsigned int endHeight = mapHeight - 1;
        for (unsigned int y = 0; y < endHeight; y ++) {
            const int next_y = y + 1;
            
            addDepthVertexAtAzimuthElevation(next_x, y);
            addDepthVertexAtAzimuthElevation(x, y);
            addDepthVertexAtAzimuthElevation(x, next_y);
            
            addDepthVertexAtAzimuthElevation(x, next_y);
            addDepthVertexAtAzimuthElevation(next_x, next_y);
            addDepthVertexAtAzimuthElevation(next_x, y);
            
        }
    }
}

void StreetView::addDepthVertexAtAzimuthElevation(int x, int y){
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
        distance = maxDistance;
        normal = xyz;
        normal.normalize();
    } else {
        DepthMapPlane plane = depthmapPlanes[depthMapIndex];
        distance = -plane.d/(plane.x * xyz.x + plane.y * xyz.y + -plane.z * xyz.z);
        normal.set(plane.x,plane.y,plane.z);
    }
    
    ofVec2f texCoord = ofVec2f((x/(float)mapWidth)*getWidth(),(y/(float)mapHeight)*getHeight());
    xyz *= distance;
    
    meshDepth.addNormal(normal);
    meshDepth.addTexCoord(texCoord);
    meshDepth.addVertex(xyz);
}

Location StreetView::getLocation(){
    return loc;
}

string StreetView::getCloseLinkTo(float _deg){
    ofPoint direction = ofPoint(cos(_deg*DEG_TO_RAD),sin(_deg*DEG_TO_RAD));
    
    int closer = -1;
    float minDiff = 0;
    for (int i = 0; i < links.size(); i++) {
        
        float angle = links[i].yaw_deg*DEG_TO_RAD;
        ofPoint l = ofPoint(cos(angle),sin(angle));
        
        float diff = l.dot(direction);
        if( diff >= 0 && diff > minDiff){
            minDiff = diff;
            closer = i;
        }
    }
    
    if(closer != -1){
        return links[closer].pano_id;
    } else {
        return "";
    }
}

double StreetView::getGroundHeight(){
    int groundIndex = depthmapIndices[mapHeight * mapWidth - 1];
    return depthmapPlanes[groundIndex].d;
}

float StreetView::getWidth(){
    return mapWidth*(1.63*powf(2.0, zoom-1));
}

float StreetView::getHeight(){
    return mapHeight*(1.63*powf(2.0, zoom-1));
}

void StreetView::update(){
    if(bDataLoaded && !bPanoLoaded){
        panoFbo.begin();
        ofClear(0,0);
        int x = 0;
        int y = 0;
        for(int i= 0; i < panoImages.size(); i++){
            panoImages.at(i).draw(x*panoImages.at(i).width, y*panoImages.at(i).height);
            if(x < 6){
                x++;
            }else{
                x=0;
                y++;
            }
        }
        panoFbo.end();
        
        if(panoImages.size() >= 3*7){
            
            panoImages.clear();
            bPanoLoaded = true;
        }
    }
}

ofTexture& StreetView::getTextureReference(){
    if(!panoFbo.isAllocated()){
        panoFbo.allocate(getWidth(),getHeight());
    }
    return panoFbo.getTextureReference();
}

ofTexture StreetView::getTextureAt(float _deg, float _amp){
    float widthDeg = getWidth()/360.0;
    
    float offsetX = widthDeg*(pano_yaw_deg-360-_deg+90);
    float amplitud = widthDeg*_amp;
    
    ofFbo roi;
    roi.allocate(widthDeg*_amp, getHeight());
    
    roi.begin();
    ofClear(0,0);
    
    getTextureReference().draw(-offsetX+amplitud*0.5-getWidth()*2.0,0);
    getTextureReference().draw(-offsetX+amplitud*0.5-getWidth(),0);
    getTextureReference().draw(-offsetX+amplitud*0.5,0);
    if(offsetX+amplitud>getWidth()){
        getTextureReference().draw(-offsetX+amplitud*0.5+getWidth(),0);
    }
    
    roi.end();
    
    return roi.getTextureReference();
}