//
//  example2D.cpp
//
//  Created by Patricio Gonzalez Vivo on 6/21/14.
//
//

#include "example2D.h"

void example2D::selfSetup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
}

void example2D::selfSetupGuis(){
}

void example2D::selfGuiEvent(ofxUIEventArgs &e){
    
}

void example2D::selfSetupSystemGui(){
    sysGui->addSlider("Radius", 0, 300, &radius);
}

void example2D::guiSystemEvent(ofxUIEventArgs &e){
    
}

void example2D::selfSetupRenderGui(){
    
}

void example2D::guiRenderEvent(ofxUIEventArgs &e){
    
}

void example2D::selfBegin(){
    
}

void example2D::selfUpdate(){
    
}

void example2D::selfDraw(){
    
    ofSetColor(255);
    ofCircle(ofGetWidth()*0.5, ofGetHeight()*0.5, radius);
}

void example2D::selfEnd(){
    
}

void example2D::selfExit(){
    
}

void example2D::selfKeyPressed(ofKeyEventArgs & args){
	
}

void example2D::selfKeyReleased(ofKeyEventArgs & args){
	
}

void example2D::selfMouseDragged(ofMouseEventArgs& data){
	
}

void example2D::selfMouseMoved(ofMouseEventArgs& data){
	
}

void example2D::selfMousePressed(ofMouseEventArgs& data){
	
}

void example2D::selfMouseReleased(ofMouseEventArgs& data){
	
}