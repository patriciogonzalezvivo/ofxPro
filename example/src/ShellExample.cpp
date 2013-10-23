//
//  ShellExample.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/6/13.
//
//

#include "ShellExample.h"


void ShellExample::selfSetup(){
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    float width = ofGetScreenWidth();
    float height = ofGetScreenHeight();
    
    grid.setFromCenter(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
    grid.setResolutionToInches();
    
    //  Shell
    //
    bufferSize = 512;
    shellBuffer = 100;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    shader.loadFrag(getDataPath()+"shaders/shader.frag");
}

void ShellExample::selfSetupGui(){
    
    guiAdd(grid);
    guiAdd(fog);
    guiAdd(shader);
    
    lightAdd("DIR LIGHT ", OF_LIGHT_DIRECTIONAL );
    lightAdd("SPOT LIGHT", OF_LIGHT_SPOT);
}

void ShellExample::selfGuiEvent(ofxUIEventArgs &e){
    
}

void ShellExample::selfSetupSystemGui(){
    sysGui->addLabel("Shell");
    sysGui->addSlider("Buffer", 10, 1000, &shellBuffer);
    sysGui->addSlider("Noise", 0.00, 5.0, &shellNoise);
    sysGui->addSlider("Radio", 0.0, 100, &shellRadio);
    sysGui->addSlider("Rotation", 1.0, 100.0, &shellRot);
    sysGui->addSlider("Translation", 0.0, 1.0, &shellTrans);
}

void ShellExample::guiSystemEvent(ofxUIEventArgs &e){
    
}

void ShellExample::selfSetupRenderGui(){

}

void ShellExample::guiRenderEvent(ofxUIEventArgs &e){
    
}

//void ShellExample::setupBackground(){
//    background = new UISuperBackground();
//    background->linkUIs( &guis );
//    background->linkCamera( &cam );
//    guiAdd( *background );
//}

void ShellExample::selfPresetLoaded(string presetPath){
	
}

void ShellExample::selfBegin(){
    fog.linkColor( background );
}

//--------------------------------------------------------------

ofPolyline ShellExample::freqArc( vector<float> &_vector, const ofPoint &_center, float _angleBegin, float _angleEnd, float _minRad,  bool _bSmooth){
    
    ofPolyline line;
    
    float angle = ofWrapRadians(_angleBegin);
    float step = (ofWrapRadians(_angleEnd) - angle)/((float)_vector.size());
    float scale = 1.0f;
    
    ofPoint start = ofPoint(_center.x + _minRad * cos(angle),
                            _center.y + _minRad * sin(angle));
    ofPoint end = ofPoint(_center.x + _minRad * cos(_angleEnd),
                          _center.y + _minRad * sin(_angleEnd));
    
    line.addVertex( start );
    for (int i = 0; i < _vector.size(); i++){
        
        float value = ofMap(_vector[i]*scale, 0.0, 60.0f, _minRad, _minRad*2);
        ofPoint p = _center;
        p.x += value * cos(angle);
        p.y += value * sin(angle);
        angle += step;
        
        if (_bSmooth){
            line.curveTo( p );
        } else {
            line.addVertex(p);
        }
    }
    line.addVertex( end );
    
    return line;
}

//--------------------------------------------------------------

void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	ofVec3f normal = ((b - a).cross(c - a)).normalize();
	mesh.addNormal(normal);
	mesh.addVertex(a);
	mesh.addNormal(normal);
	mesh.addVertex(b);
	mesh.addNormal(normal);
	mesh.addVertex(c);
}

void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

//--------------------------------------------------------------
void ShellExample::createSkin(int _width){
    
    int width = _width;
    int height = points.size() / width;
    
    mesh.clear();
    for(int y = 0; y < height - 1; y ++) {
		for(int x = 0; x < width - 1; x ++) {
            
			ofVec3f nw = points[ x + y * width];
			ofVec3f ne = points[ (x+1) + y * width];
			ofVec3f sw = points[ x + (y+1) * width];
			ofVec3f se = points[ (x+1) + (y+1) * width];
			
			addFace(mesh, nw, ne, se, sw);
		}
	}
}

void ShellExample::selfSceneTransformation(){
	
}

void ShellExample::selfUpdate(){

    vector<float> newAverage;
    for (int i = 0; i < bufferSize; i++) {
        newAverage.push_back( ofNoise(i*0.01,ofGetElapsedTimef())*shellNoise );
    }
    averages.push_back( newAverage );
    
    while(averages.size()> shellBuffer){
        averages.erase(averages.begin());
    }
    
    //  Shell
    //
    float phi   = (1.0+sqrtf(5.0))/2.0;
    float grow  = (1.0+phi);
    float stepGrow = grow*(shellRot/shellBuffer)*0.1;
    
    float radio = shellRadio;
    offSet = ofPoint(0,0);
    
    int lineWidth;
    points.clear();
    
    ofMatrix4x4 matrix;
    for (int i = 0; i < averages.size(); i++){
        radio += radio*stepGrow;
        float size = radio*grow;
        offSet = ofPoint(size,size*-shellTrans);
        
        ofPolyline line = freqArc( averages[i], offSet, PI-0.65, 0.0f+0.65, size );
        //line = line.getSmoothed(2);
        lineWidth = line.size();
        
        matrix.rotate( shellRot , 1.0 , 0.0, 0.0);
        for (int j = 0; j < lineWidth; j++){
            points.push_back( matrix*line[j] );
        }
    }
    
    createSkin(lineWidth);
    
}

void ShellExample::selfDraw(){
    glEnable(GL_DEPTH_TEST);
    
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(255);
    grid.draw();
    
    ofSetColor(0);
    
    {
        ofPushMatrix();
        ofPushStyle();
        fog.begin();
        materials["MATERIAL 1"]->begin();
        
        ofTranslate(0,offSet.x*0.7);
        ofRotate(-90, 0, 0, 1);
        ofSetColor(255,250,240);
        mesh.drawFaces();
        
//        ofSetColor(255);
//        ofDrawSphere(200);
        
        materials["MATERIAL 1"]->end();
        fog.end();
        ofPushStyle();
        ofPopMatrix();
    }
    
	ofPopStyle();
    ofPopMatrix();
    
    glDisable(GL_DEPTH_TEST);
}

void ShellExample::selfDrawDebug(){
	lightsDraw();
}

void ShellExample::selfDrawBackground(){

}

void ShellExample::selfPostDraw(){
//    shader.begin();
//    shader.getShader().setUniformTexture("tex1", UIProject::getRenderTarget().getDepthTexture(), 1);
    UIProject::selfPostDraw();
//    shader.end();
}

void ShellExample::selfEnd(){
}

void ShellExample::selfExit(){
}

void ShellExample::selfKeyPressed(ofKeyEventArgs & args){
	
}
void ShellExample::selfKeyReleased(ofKeyEventArgs & args){
	
}

void ShellExample::selfMouseDragged(ofMouseEventArgs& data){
	
}

void ShellExample::selfMouseMoved(ofMouseEventArgs& data){
	
}

void ShellExample::selfMousePressed(ofMouseEventArgs& data){
	
}

void ShellExample::selfMouseReleased(ofMouseEventArgs& data){
	
}