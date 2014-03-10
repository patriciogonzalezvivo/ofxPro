//
//  UIStreetView.cpp
//
//  Created by Patricio Gonzalez Vivo on 3/10/14.
//
//

#include "UIStreetView.h"

#include "base64.h"
#include <zlib.h>

UIStreetView::UIStreetView(){
    ofRegisterURLNotification(this);
    fbo.allocate(13312, 6656);
    lat = 0;
    lng = 0;
    bLoaded = false;
}

UIStreetView::UIStreetView(double _lat, double _lon){
    UIStreetView();
    setLatLon(_lat,_lon);
}

UIStreetView::UIStreetView(const Location &location){
    UIStreetView();
    setLocation(location);
}

void UIStreetView::setLocation(const Location &_loc){
    setLatLon(_loc.lat,_loc.lon);
}

void UIStreetView::setLatLon(double _lat, double _lon){
    bLoaded = false;
    string data_url = "http://cbk0.google.com/cbk?output=xml&ll="+ofToString(_lat)+","+ofToString(_lon)+"&dm=1&pm=1";
    ofLoadURLAsync(data_url);
}

UIStreetView::~UIStreetView(){
    ofUnregisterURLNotification(this);
}

void UIStreetView::setupUI(){
    
}

void UIStreetView::guiEvent(ofxUIEventArgs &e){
    
}

void UIStreetView::urlResponse(ofHttpResponse & response){
    if(response.status==200 && response.request.url.find("http://cbk0.google.com/cbk?output=xml") == 0){
        images.clear();
        
        ofxXmlSettings  XML;
        XML.loadFromBuffer(response.data);
        
//        <data_properties image_width="13312" image_height="6656" tile_width="512" tile_height="512" image_date="2011-07" pano_id="h8MTbKFUL84LsKzZjpbbSA" imagery_type="1" num_zoom_levels="3" lat="40.737268" lng="-73.992553" original_lat="40.737279" original_lng="-73.992562" elevation_wgs84_m="-17.887235" best_view_direction_deg="108.72954">
        pano_id = XML.getAttribute("panorama:data_properties", "pano_id", "BLANK");
        lat = XML.getAttribute("panorama:data_properties", "lat", 0.0);
        lng = XML.getAttribute("panorama:data_properties", "lng", 0.0);
        original_lat = XML.getAttribute("panorama:data_properties", "original_lat", 0.0);
        original_lng = XML.getAttribute("panorama:data_properties", "original_lng", 0.0);
        num_zoom_levels = XML.getAttribute("panorama:data_properties", "num_zoom_levels", 0);
        
//        <projection_properties projection_type="spherical" pano_yaw_deg="122.63" tilt_yaw_deg="-46.44" tilt_pitch_deg="1.1899999"/>
        pano_yaw_deg = XML.getAttribute("panorama:projection_properties", "pano_yaw_deg", 0.0);
        tilt_yaw_deg = XML.getAttribute("panorama:projection_properties", "tilt_yaw_deg", 0.0);
        tilt_pitch_deg = XML.getAttribute("panorama:projection_properties", "tilt_pitch_deg", 0.0);

//        <annotation_properties>
//        <link yaw_deg="119.05" pano_id="8FegTnSng4M45fC-y-da4Q" road_argb="0x80ffffff" scene="0">
//        <link_text>East 16th Street</link_text>
//        </link>
//        <link yaw_deg="299.05" pano_id="SnpKtJ8do5DNP7pXj14pkg" road_argb="0x80ffffff" scene="0">
//        <link_text>5th Avenue / East 16th Street / West 16th Street</link_text>
//        </link>
//        </annotation_properties>
        
        //Decode the depth map
        //The depth map is encoded as a series of pixels in a 512x256 image. Each pixels refers
        //to a depthMapPlane which are also encoded in the data. Each depthMapPlane has three elements:
        //The x,y,z normals and the closest distance the plane has to the origin. This uniquely
        //identifies the plane in 3d space.
        {
//            //Get the base64 encoded data
//            string depth_map_base64 = XML.getValue("panorama:model:depth_map", "NONE");
//            
//            //Decode base64
//            vector<unsigned char> depth_map_compressed(depth_map_base64.length());
//            int compressed_length = decode_base64(&depth_map_compressed[0], &depth_map_base64[0]);
//            
//            //Uncompress data with zlib
//            //TODO: decompress in a loop so we can accept any size
//            unsigned long length = 512 * 256 + 5000;
//            vector<unsigned char> depth_map(length);
//            int zlib_return = uncompress(&depth_map[0], &length, &depth_map_compressed[0], compressed_length);
//            if (zlib_return != Z_OK)
//                throw "zlib decompression of the depth map failed";
//            
//            //Load standard data
//            const int headersize = depth_map[0];
//            const int numPanos = depth_map[1] | (depth_map[2] << 8);
//            mapWidth = depth_map[3] | (depth_map[4] << 8);
//            mapHeight = depth_map[5] | (depth_map[6] << 8);
//            const int panoIndicesOffset = depth_map[7];
//            
//            if (headersize != 8 || panoIndicesOffset != 8)
//                throw "Unexpected depth map header";
//            
//            //Load depthMapIndices
//            depthmapIndices = vector<unsigned char>(mapHeight * mapWidth);
//            memcpy(&depthmapIndices[0], &depth_map[panoIndicesOffset], mapHeight * mapWidth);
//            
//            //Load depthMapPlanes
//            depthmapPlanes = vector<struct DepthMapPlane> (numPanos);
//            memcpy(&depthmapPlanes[0], &depth_map[panoIndicesOffset + mapHeight * mapWidth], numPanos * sizeof (struct DepthMapPlane));
        }
        
        
        if(pano_id != "BLANK"){
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 6; j++){
                    ofLoadURLAsync("http://cbk0.google.com/cbk?output=tile&panoid="+pano_id+"&zoom=3&x="+ofToString(j)+"&y="+ofToString(i));
                }
            }
        }
    
    } else if(response.status==200){
        ofImage img;
        img.loadImage(response.data);
        images.push_back(img);
    }
}

float UIStreetView::getWidth(){
    if(fbo.isAllocated()){
        return fbo.getWidth();
    }
}

float UIStreetView::getHeight(){
    if(fbo.isAllocated()){
        return fbo.getHeight();
    }
}

ofTexture& UIStreetView::getTextureReference(){
    if(fbo.isAllocated()){
        return fbo.getTextureReference();
    }
}

void UIStreetView::update(){
    if(images.size() >= 3*6 && !bLoaded){
        fbo.allocate(images[0].width*6, images[0].height*3);
        
        fbo.begin();
        ofClear(0,0);
        int x = 0;
        int y = 0;
        for(int i= 0; i < images.size(); i++){
            images.at(i).draw(x*images.at(i).width, y*images.at(i).height);
            if(x < 5){
                x++;
            }else{
                x=0;
                y++;
            }
        }
        fbo.end();
        bLoaded = true;
    }
}

void UIStreetView::draw(float _x, float _y,float _width, float _height) {
    if(bLoaded){
        if(_width==-1||_height==-1){
            _width = fbo.getWidth();
            _height = fbo.getHeight();
        }
        
        ofPushStyle();
        ofSetColor(255);
        fbo.draw(_x,_y,_width,_height);
        ofPopStyle();
    }
}
