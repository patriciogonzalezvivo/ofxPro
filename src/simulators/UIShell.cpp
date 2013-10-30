//
//  UIShell.cpp
//  PrintMaking
//
//  Created by Patricio Gonzalez Vivo on 10/27/13.
//
//

#include "UIShell.h"

UIShell::UIShell(){
    buffer = 100;
    bufferSize = 512;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
}

void UIShell::setupUI(){
    gui->addSlider("Buffer", 10, 1000, &buffer);
    gui->addSlider("Noise", 0.00, 5.0, &noise);
    gui->addSlider("Radio", 0.0, 100, &radio);
    gui->addSlider("Rotation", 1.0, 100.0, &rotation);
    gui->addSlider("Translation", 0.0, 1.0, &translation);
}

void UIShell::update(){
    if(bEnable){
        vector<float> newAverage;
        for (int i = 0; i < bufferSize; i++) {
            newAverage.push_back( ofNoise(i*0.01,ofGetElapsedTimef())*noise );
        }
        averages.push_back( newAverage );
        
        while(averages.size()> buffer){
            averages.erase(averages.begin());
        }
        
        //  Shell
        //
        float phi   = (1.0+sqrtf(5.0))/2.0;
        float grow  = (1.0+phi);
        float stepGrow = grow*(rotation/buffer)*0.1;
        
        float _radio = radio;
        offSet = ofPoint(0,0);
        
        int lineWidth;
        points.clear();
        
        ofMatrix4x4 matrix;
        for (int i = 0; i < averages.size(); i++){
            _radio += _radio*stepGrow;
            float size = _radio*grow;
            offSet = ofPoint(size,size*-translation);
            
            ofPolyline line = freqArc( averages[i], offSet, PI-0.65, 0.0f+0.65, size );
            lineWidth = line.size();
            
            matrix.rotate( rotation , 1.0 , 0.0, 0.0);
            for (int j = 0; j < lineWidth; j++){
                points.push_back( matrix*line[j] );
            }
        }
        
        createSkin(lineWidth);
    }
}

void UIShell::draw(){
    if(bEnable){
        ofPushMatrix();
        ofTranslate(0,offSet.x*0.7);
        ofRotate(-90, 0, 0, 1);
        ofSetColor(255,250,240);
        mesh.drawFaces();
        ofPopMatrix();
    }
}


void UIShell::guiEvent(ofxUIEventArgs &e){
    
}

//------------------------------------------------ HELPERS
ofPolyline UIShell::freqArc( vector<float> &_vector, const ofPoint &_center, float _angleBegin, float _angleEnd, float _minRad,  bool _bSmooth){
    
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

void UIShell::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	ofVec3f normal = ((b - a).cross(c - a)).normalize();
	mesh.addNormal(normal);
	mesh.addVertex(a);
	mesh.addNormal(normal);
	mesh.addVertex(b);
	mesh.addNormal(normal);
	mesh.addVertex(c);
}

void UIShell::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

void UIShell::createSkin(int _width){
    
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
