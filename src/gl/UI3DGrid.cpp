//
//  UI3DGrid.cpp
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#include "UI3DGrid.h"
#define STRINGIFY(A) #A

UI3DGrid::UI3DGrid(){
    colorSampleImage.loadImage("GUI/defaultColorPalette.png");
    
    gridScale = 25.;
	gridMajorScale = 10;
	fogFalloffDistance =  2000.;
	fogFalloffExpo = 2.;
	fogFalloffScale = 1.2;
	gridAlphaScale = 1.;
	gridLineWidth = 1.;
	gridDim = 2000;
	majorGridLineWidth = 1.5;
	gridColor.set(255);
	gridMajorColor.set(255);
	gridAlpha = gridMajorAlpha = .5;
	gridBrightness = gridMajorBrightness = 1.;
    
    string vertexShader = STRINGIFY(varying vec3 ePos;
                                    varying float dist;
                                    varying vec2 endVal;
                                    
                                    void main(){
                                        endVal = gl_Vertex.xz;
                                        vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
                                        ePos = normalize(ecPosition.xyz/ecPosition.w);
                                        gl_Position = gl_ProjectionMatrix * ecPosition;
                                        
                                        dist = gl_Position.z;
                                        
                                        gl_FrontColor = gl_Color;
                                    });
    
    string fragShader = STRINGIFY(uniform float halfGridDim;
                                  uniform float falloffDist;
                                  uniform float falloffExpo;
                                  uniform float falloffScl;
                                  uniform float alphaScale;
                                  
                                  varying vec3 ePos;
                                  varying float dist;
                                  varying vec2 endVal;
                                  
                                  float nearClip = 1.;
                                  
                                  float linearizeDepth( in float d ) {
                                      return (2.0 * nearClip) / (falloffDist + nearClip - d * (falloffDist - nearClip));
                                  }
                                  
                                  void main(void){
                                      float alpha = pow((1. - linearizeDepth( gl_FragCoord.z )) * falloffScl, falloffExpo);
                                      gl_FragColor = vec4( gl_Color.xyz, gl_Color.w * alpha * alphaScale);
                                  });
    
    shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
    float halfGridDim = gridDim / 2;
	vector<ofVec3f> gridVertices(gridDim * 4);
	for (int i=0; i<gridDim; i++)
	{
		gridVertices[i*4+0].set(i - halfGridDim, 0,-halfGridDim);
		gridVertices[i*4+1].set(i - halfGridDim, 0, halfGridDim);
		gridVertices[i*4+2].set(-halfGridDim, 0, i - halfGridDim);
		gridVertices[i*4+3].set( halfGridDim, 0, i - halfGridDim);
	}
	grid.setVertexData( &gridVertices[0], gridVertices.size(), GL_STATIC_DRAW );
	numGridVertices = gridVertices.size();
	gridVertices.clear();
	
	for (int i=0; i<gridDim; i += 5)
	{
		gridVertices.push_back( ofVec3f(i - halfGridDim, 0,-halfGridDim) );
		gridVertices.push_back( ofVec3f(i - halfGridDim, 0, halfGridDim) );
		gridVertices.push_back( ofVec3f(-halfGridDim, 0, i - halfGridDim) );
		gridVertices.push_back( ofVec3f( halfGridDim, 0, i - halfGridDim) );
	}
	gridMajor.setVertexData( &gridVertices[0], gridVertices.size(), GL_STATIC_DRAW );
	numGridMajorVertices = gridVertices.size();
	gridVertices.clear();
}

void UI3DGrid::setupUI(){
	gui->addSlider("gridLineWidth", 0.5, 10, &gridLineWidth);
	gui->addSlider("majorGridLineWidth", 0.5, 10, &majorGridLineWidth);
	gui->addSlider("gridScale", 1., 100., &gridScale);
	gui->addSlider("gridMajorScale", 1, 25, &gridMajorScale );
	
	gui->addSpacer();
	
	gui->addImageSampler("gridColor", &colorSampleImage,
                         (float)colorSampleImage.getWidth()/2,
                         (float)colorSampleImage.getHeight()/2 );
    
	gui->addSlider("gridAlpha", 0, 255, &gridAlpha );
	gui->addSlider("gridBrightness", 0, 1, &gridBrightness );
	gui->addImageSampler("gridMajorColor", &colorSampleImage,
                         (float)colorSampleImage.getWidth()/2,
                         (float)colorSampleImage.getHeight()/2 );
    
	gui->addSlider("gridMajorAlpha", 0, 255, &gridMajorAlpha );
	gui->addSlider("gridMajorBrightness", 0, 1, &gridMajorBrightness );
	gui->addSlider("gridAlphaScale", .5, 2., &gridAlphaScale );
    
    gui->addSpacer();
	
	gui->addSlider("fogFalloffDistance", 100, 5000, &fogFalloffDistance );
	gui->addSlider("fogFalloffExpo", .6, 10, &fogFalloffExpo );
	gui->addSlider("fogFalloffScale", .5, 2., &fogFalloffScale );
}

void UI3DGrid::guiEvent(ofxUIEventArgs &e){
    if( e.widget->getName() == "gridColor"){
		ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
		gridColor = sampler->getColor();
	}
	
	else if( e.widget->getName() == "gridMajorColor"){
		ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
		gridMajorColor = sampler->getColor();
	}
}

void UI3DGrid::draw(){
    if(bEnable){
        ofPushStyle();
		//	ofVec3f camPos;
		//	camPos = cam->getPosition();
		//	camPos += cam->getUpDir().cross(cam->getSideDir()).normalize() * gridDim * gridScale * .5;
		
		ofSetColor(255,255, 255, 150 );// make this an adjustable color in th GUI
		
		shader.begin();
		shader.setUniform1f("halfGridDim", gridDim * .5 );
		shader.setUniform1f("falloffDist", fogFalloffDistance );
		shader.setUniform1f("falloffExpo", fogFalloffExpo );
		shader.setUniform1f("falloffScl", fogFalloffScale );
		shader.setUniform1f("alphaScale", gridAlphaScale );
		
		ofPushMatrix();
		int gms = gridMajorScale;
		//	ofTranslate( floor(camPos.x/(gridScale*gms))*gms*gridScale, 0, floor(camPos.z/(gridScale*gms))*gms*gridScale);
		
		ofScale( gridScale * gms,gridScale * gms, gridScale * gms );
		
		glLineWidth( majorGridLineWidth );
		ofSetColor( gridMajorColor.r*gridMajorBrightness, gridMajorColor.g*gridMajorBrightness, gridMajorColor.b*gridMajorBrightness, gridMajorAlpha );
		grid.draw(GL_LINES, 0, numGridVertices );
		
		ofPopMatrix();
		
		ofPushMatrix();
		//	ofTranslate( floor(camPos.x/gridScale) * gridScale, 0, floor(camPos.z/gridScale) * gridScale );
		ofScale( gridScale, gridScale, gridScale );
		
		glLineWidth( gridLineWidth );
		ofSetColor( gridColor.r*gridBrightness, gridColor.g*gridBrightness, gridColor.b*gridBrightness, gridAlpha );
		grid.draw(GL_LINES, 0, numGridVertices );
		
		ofPopMatrix();
		
		shader.end();
        ofPopStyle();
	}
}