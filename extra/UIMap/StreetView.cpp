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
    
    compileList = -1;
    horizontal_accuracy = 2;
    vertical_accuracy = 5;
    zoom_level = 2;
    wireframe = false;
    mipmapping = false;
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
    compileList = -1;
    string data_url = "http://cbk0.google.com/cbk?output=xml&ll="+ofToString(_lat)+","+ofToString(_lon)+"&dm=1&pm=1";
    ofLoadURLAsync(data_url);
}

void StreetView::setPanoId(string _pano_id){
    clear();
    compileList = -1;
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
    compileList = -1;
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
                for(int j = 0; j < 6; j++){
                    ofLoadURLAsync("http://cbk0.google.com/cbk?output=tile&panoid="+pano_id+"&zoom=3&x="+ofToString(j)+"&y="+ofToString(i));
                }
            }
        }
        
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
        }
    }
}

void StreetView::urlResponse(ofHttpResponse & response){
    if(response.status==200 && response.request.url.find("http://cbk0.google.com/cbk?output=xml") == 0 && !bDataLoaded){
        images.clear();
        
        ofxXmlSettings  XML;
        XML.loadFromBuffer(response.data);
        
//        <data_properties image_width="13312" image_height="6656" tile_width="512" tile_height="512" image_date="2011-07" pano_id="h8MTbKFUL84LsKzZjpbbSA" imagery_type="1" num_zoom_levels="3" lat="40.737268" lng="-73.992553" original_lat="40.737279" original_lng="-73.992562" elevation_wgs84_m="-17.887235" best_view_direction_deg="108.72954">
        pano_id = XML.getAttribute("panorama:data_properties", "pano_id", "");
        loc.lat = XML.getAttribute("panorama:data_properties", "original_lat", 0.0);
        loc.lon = XML.getAttribute("panorama:data_properties", "original_lng", 0.0);
        num_zoom_levels = XML.getAttribute("panorama:data_properties", "num_zoom_levels", 0);
        
//        <projection_properties projection_type="spherical" pano_yaw_deg="122.63" tilt_yaw_deg="-46.44" tilt_pitch_deg="1.1899999"/>
        pano_yaw_deg = XML.getAttribute("panorama:projection_properties", "pano_yaw_deg", 0.0);
        tilt_yaw_deg = XML.getAttribute("panorama:projection_properties", "tilt_yaw_deg", 0.0);
        tilt_pitch_deg = XML.getAttribute("panorama:projection_properties", "tilt_pitch_deg", 0.0);
        
        //Get the base64 encoded data
        depth_map_base64 = XML.getValue("panorama:model:depth_map", "");
        pano_map_base64 = XML.getValue("panorama:model:pano_map", "");

//        <annotation_properties>
//        <link yaw_deg="119.05" pano_id="8FegTnSng4M45fC-y-da4Q" road_argb="0x80ffffff" scene="0">
//        <link_text>East 16th Street</link_text>
//        </link>
//        <link yaw_deg="299.05" pano_id="SnpKtJ8do5DNP7pXj14pkg" road_argb="0x80ffffff" scene="0">
//        <link_text>5th Avenue / East 16th Street / West 16th Street</link_text>
//        </link>
//        </annotation_properties>
        
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
        
        utmLoc = loc.getUTM();
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

void StreetView::update(){
    if(bDataLoaded && !bPanoLoaded && images.size() >= 3*6){
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
        
        bPanoLoaded = true;
    }
}

void StreetView::draw(){
    if(bPanoLoaded&&fbo.isAllocated()){
        draw(utmLoc, true);
    }
}

void StreetView::draw(UtmPosition referencePoint, bool drawAll) {
    /**
     * Transparant borders are the fuzzy edges of the panorama. Due to the use of display
     * lists it is impossible to both have a continually increasing opacity AND transparant borders.
     *
     * We solve this in a two step process: during the fade-in sequence we set transparancy globally,
     * no glColor calls are allowed in the display list that could corrupt the global transparancy.
     * Once faded in we enable the transparant borders and allow glColor calls in the display list
     */
    bool settingsChanged = false;
    bool firstTime = false;
    
    if (compileList == -1) {
        compileList = glGenLists(1);
        threeSixtyCompileList = glGenLists(1);
        firstTime = true;
    }
    
    //If any setting changed from the last time we compiled the list we need to recompile it with the new settings
    if (compiledRenderSettings.horizontalDetail != horizontal_accuracy
        || compiledRenderSettings.verticalAccuracy != vertical_accuracy) {
        settingsChanged = true;
    }
    
    //Recreate the display lists
    if (firstTime || settingsChanged) {
        RenderSettings renderSettings;
        renderSettings.horizontalDetail = 2;
        renderSettings.verticalAccuracy = 5;
        
        //Recreate display list for 'only own data' panorama
        renderSettings.transparancy = true;
        glNewList(compileList, GL_COMPILE);
        drawActual(referencePoint, false, renderSettings);
        glEndList();
        
        //Recreate display list for 'all data' panorama
        renderSettings.transparancy = false;
        glNewList(threeSixtyCompileList, GL_COMPILE);
        drawActual(referencePoint, true, renderSettings);
        glEndList();
        
        compiledRenderSettings = renderSettings;
    }
    
    //Draw the cached version
    if (drawAll) {
        glCallList(threeSixtyCompileList);
    } else {
        glCallList(compileList);
    }
}

void StreetView::drawActual(UtmPosition referencePoint, bool drawAll, RenderSettings settings) {
    glPushMatrix();
    glTranslated(utmLoc.easting - referencePoint.easting, utmLoc.northing - referencePoint.northing, 0);
    glRotated(180 - pano_yaw_deg, 0, 0, 1);
    
//    glBindTexture(GL_TEXTURE_2D, texture_id);
    fbo.bind();
    
    //The environment is basically just a sphere with each vertex at a different distance creating
    //The illusion that everything is organized into planes
    
    /**
     * Draw the panorama in long vertical strips
     */
    for (int x = 0; x < mapWidth - 1; x += settings.horizontalDetail) {
        //printf("%d ", x);
        bool drawing = false;
        int currentDepthMap = 0;
        int currentDepthMap2 = 0;
        
        const int next_x = x + settings.horizontalDetail;
        int map_next_x = next_x;
        if (map_next_x >= mapWidth)
            map_next_x -= mapWidth;
        
        //Start drawing immediately at the zenith in the sky
        if (drawAll) {
            drawing = true;
            
            glBegin(GL_QUAD_STRIP);
            drawVertexAtAzimuthElevation(next_x, 0, settings);
            drawVertexAtAzimuthElevation(x, 0, settings);
        }
        const unsigned int endHeight = mapHeight - 1;
        
        for (unsigned int y = 0; y < endHeight; y += 1) {
            
            const int next_y = y + 1;
            
            //Stop drawing
            if (!drawAll && drawing && (!isVisible(x, y) || !isVisible(x + 1, y))) {
                
                drawing = false;
                
                drawVertexAtAzimuthElevation(next_x, y, settings);
                drawVertexAtAzimuthElevation(x, y, settings);
                glEnd();
            }
            
            /**
             * If the depth map changes ensure that extra vertices are placed to create a nice sharp
             * edge between planes.
             */
            const int depthMapIndex = depthmapIndices[(y + 1) * mapWidth + x];
            const int depthMapIndex2 = depthmapIndices[(y + 1) * mapWidth + map_next_x];
            
            if (drawing && (depthMapIndex != currentDepthMap || depthMapIndex2 != currentDepthMap2)) {
                currentDepthMap = depthMapIndex;
                currentDepthMap2 = depthMapIndex2;
                
                drawVertexAtAzimuthElevation(next_x, y, settings);
                drawVertexAtAzimuthElevation(x, y, settings);
                
                drawVertexAtAzimuthElevation(next_x, next_y, settings);
                drawVertexAtAzimuthElevation(x, next_y, settings);
                
                //Draw extra perspective corrective lines
            } else if (drawing && (y % settings.verticalAccuracy) == 0) {
                
                drawVertexAtAzimuthElevation(next_x, y, settings);
                drawVertexAtAzimuthElevation(x, y, settings);
                
                //Draw extra lines for transparancy to flow nicely
            } else if (!drawAll && drawing && (isTransparant(x, y - 1, settings.horizontalDetail) || isTransparant(next_x, y - 1, settings.horizontalDetail))) {
                drawVertexAtAzimuthElevation(next_x, y, settings);
                drawVertexAtAzimuthElevation(x, y, settings);
            }//Start drawing
            else if (!drawing && isVisible(x, y) && isVisible(x + 1, y)) {
                drawing = true;
                
                glBegin(GL_QUAD_STRIP);
                
                currentDepthMap = depthMapIndex;
                currentDepthMap2 = depthMapIndex2;
                
                drawVertexAtAzimuthElevation(next_x, y, settings);
                drawVertexAtAzimuthElevation(x, y, settings);
            }
        }
        //Draw last vertex at the nadir / very bottom of panorama
        if (drawing) {
            drawVertexAtAzimuthElevation(next_x, endHeight, settings);
            drawVertexAtAzimuthElevation(x, endHeight, settings);
            
            glEnd();
        }
        
    }
    fbo.unbind();
    glPopMatrix();
}

void StreetView::drawVertexAtAzimuthElevation(int x, int y, RenderSettings settings) {

    if (settings.transparancy)
        glColor4d(1, 1, 1, isTransparant(x, y, settings.horizontalDetail) ? 0 : 1);
    
    float rad_azimuth = x / (float) (mapWidth - 1.0f) * TWO_PI;
    float rad_elevation = y / (float) (mapHeight - 1.0f) * PI;
    
    //Calculate the cartesian position of this vertex (if it was at unit distance)
    ofPoint xyz;
    xyz.x = sin(rad_elevation) * sin(rad_azimuth);
    xyz.y = sin(rad_elevation) * cos(rad_azimuth);
    xyz.z = cos(rad_elevation);
    float distance = 1;
    
    //Value that is safe to use to retrieve stuff from the index arrays
    const int map_x = x % mapWidth;
    
    //Calculate distance of point according to the depth map data.
    int depthMapIndex = depthmapIndices[y * mapWidth + map_x];
    if (depthMapIndex == 0) {
        //Distance of sky
        distance = 100;
    } else {
        DepthMapPlane plane = depthmapPlanes[depthMapIndex];
        distance = -plane.d / (plane.x * xyz.x + plane.y * xyz.y + -plane.z * xyz.z);
    }
    
    //Draw point!
//    glTexCoord2d(x / (float) mapWidth, y / (float) mapHeight);
    glTexCoord2d((x/(float)mapWidth)*fbo.getWidth(), (y/(float)mapHeight)*fbo.getHeight());
    glVertex3f(xyz.x * distance, xyz.y*distance, xyz.z * distance);
}
