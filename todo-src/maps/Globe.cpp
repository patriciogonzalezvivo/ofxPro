//
//  Globe.cpp
//  DV
//
//  Created by Patricio Gonzalez Vivo on 8/11/13.
//
//

#include "Globe.h"
#define STRINGIFY(A) #A

Globe::Globe(){
    ofSpherePrimitive::set(300, 60);
    
    wireScale = 0.0;
    solidScale = 0.98;
    haloScale = 0.99;
    
    wireColor.set(1.0);
    solidColor.set(1.0);
    haloColor.set(1.0);
    
    coastColor.set(1.0);
    riversColor.set(0.0,0.5490,0.7843);
    
    string vertexBacklightShader = STRINGIFY(varying vec3 eyeVec;
                                             varying vec3 vNorm;
                                             
                                             uniform float minAngle;
                                             uniform float exponent;
                                             
                                             varying float angle;
                                             
                                             
                                             void main(void){
                                                 
                                                 vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
                                                 eyeVec = normalize(-vVertex);
                                                 vNorm = normalize( gl_NormalMatrix * gl_Normal);
                                                 
                                                 angle = dot(vNorm, eyeVec);
                                                 
                                                 gl_Position = ftransform();
                                                 gl_FrontColor = gl_Color;
                                             });
    
    string fragmentBacklightShader = STRINGIFY(varying vec3 eyeVec;
                                               varying float angle;
                                               varying vec3 vNorm;
                                               
                                               void main() {
                                                   if(angle > 0.0){
                                                       gl_FragColor = gl_Color * vec4( vec3( pow(1.-angle, 4.0) / 1.57079632679), 1.0);
                                                   }
                                                   else {
                                                       gl_FragColor = vec4(0.0);
                                                   }
                                               });
    
    haloShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexBacklightShader);
    haloShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentBacklightShader);
    haloShader.linkProgram();
};

void Globe::update(){
    
    coastVbo.clear();
    coastVbo.setMode(OF_PRIMITIVE_LINES);
    riversVbo.clear();
    riversVbo.setMode(OF_PRIMITIVE_LINES);
    
//    loadLatLonToVbo(coastVbo, "Globe/africa-cil.txt");
//    loadLatLonToVbo(coastVbo, "Globe/asia-cil.txt");
//    loadLatLonToVbo(coastVbo, "Globe/europe-cil.txt");
//    loadLatLonToVbo(coastVbo, "Globe/namer-cil.txt");
//    loadLatLonToVbo(coastVbo, "Globe/samer-cil.txt");
    loadPositionsToVbo(coastVbo, "Globe/simple-coast.txt");
    
//    loadLatLonToVbo( riversVbo, "Globe/africa-riv.txt" );
//    loadLatLonToVbo( riversVbo, "Globe/asia-riv.txt" );
//    loadLatLonToVbo( riversVbo, "Globe/europe-riv.txt" );
//    loadLatLonToVbo( riversVbo, "Globe/namer-riv.txt" );
//    loadLatLonToVbo( riversVbo, "Globe/samer-riv.txt" );
    loadPositionsToVbo(riversVbo, "Globe/simple-rivers.txt");
    
}

void Globe::loadPositionsToVbo(ofVboMesh &_vbo, string _file){
    
    ifstream fileIn;
	
    string filePath = _file;
    ofBuffer buffer = ofBufferFromFile(filePath);
    
    ofPoint prevPoint;
    
    bool bFirst = true;
    while(!buffer.isLastLine()) {
        string temp = buffer.getNextLine();
        
        if(temp.length() != 0) {
            
            vector<string> values = ofSplitString(temp, " ");
            
            
            if ( values[0] == "segment"){
                bFirst = true;
                
            } else {
                ofPoint newPoint = ofPoint(ofToFloat(values[0]),
                                           ofToFloat(values[1]),
                                           ofToFloat(values[2]));
                if (!bFirst){
                    _vbo.addVertex(prevPoint);
                    _vbo.addVertex(newPoint);
                } else {
                    bFirst = false;
                }
                
                prevPoint = newPoint;
            }
        }
    }
}

void Globe::loadLatLonToVbo(ofVboMesh &_vbo, string _file){
    ifstream fileIn;
	
    string filePath = _file;
    ofBuffer buffer = ofBufferFromFile(filePath);
    
    ofPoint prevPoint;
    
    bool bFirst = true;
    while(!buffer.isLastLine()) {
        string temp = buffer.getNextLine();
        
        if(temp.length() != 0) {
            
            vector<string> values = ofSplitString(temp, " ");
            
            
            if ( values[0] == "segment"){
                bFirst = true;
                
            } else {
                
                ofVec3f displacementFromCenter = ofVec3f(0,0,-radius);
                
                ofQuaternion latRot;
                latRot.makeRotate( ofToFloat(values[0]), 1, 0, 0);
                
                ofQuaternion longRot;
                longRot.makeRotate( ofToFloat(values[1]), 0, 1, 0);
                
                ofPoint newPoint = latRot * longRot * displacementFromCenter;
                
                if (!bFirst){
                    _vbo.addVertex(prevPoint);
                    _vbo.addVertex(newPoint);
                } else {
                    bFirst = false;
                }
                
                prevPoint = newPoint;
            }
        }
    }
}


void Globe::draw(){
    
    Sphere::draw();
    
    if(haloScale != 0 || haloColor.a != 0){
        ofPushMatrix();
        ofPushStyle();
        
        ofScale(haloScale, haloScale, haloScale);
        
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        haloShader.begin();
        ofSetColor(haloColor);
        ofSpherePrimitive::draw();
        haloShader.end();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        
        ofPopStyle();
        ofPopMatrix();
    }
    
    
    
    ofPushMatrix();
    ofPushStyle();
    ofScale(0.3,0.3,0.3);
    ofRotateY(ofGetFrameNum()*0.05);

    if (coastColor.a){
        ofSetLineWidth(0.5);
        ofSetColor(coastColor);
        coastVbo.drawWireframe();
    }
    
    if (riversColor.a){
        ofSetLineWidth(0.2);
        ofSetColor(riversColor);
        riversVbo.drawWireframe();
    }
    
    ofPopStyle();
    ofPopMatrix();
}