//
//  example3D.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/6/13.
//
//

#include "example3D.h"

void example3D::selfSetup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
}

void example3D::selfSetupGuis(){
    lightAdd("DIR LIGHT ", OF_LIGHT_DIRECTIONAL );
    lightAdd("SPOT LIGHT", OF_LIGHT_SPOT);
    materialAdd("MATERIAL 2");
}

void example3D::selfGuiEvent(ofxUIEventArgs &e){

}

void example3D::selfSetupSystemGui(){
    sysGui->addSlider("Radius_1", 0, 300, &radius1);
    sysGui->addSlider("Radius_2", 0, 50, &radius2);
}

void example3D::guiSystemEvent(ofxUIEventArgs &e){
    
}

void example3D::selfSetupRenderGui(){

}

void example3D::guiRenderEvent(ofxUIEventArgs &e){
    
}

void example3D::selfPresetLoaded(string presetPath){
	
}

void example3D::selfBegin(){
}

void example3D::selfSceneTransformation(){
	
}

void example3D::selfUpdate(){
    
}

void example3D::selfDraw(){
    materials["MATERIAL 1"]->begin();
    ofSetColor(255);
    ofDrawSphere(0, 0, radius1);
    materials["MATERIAL 1"]->end();
    
    ofPushMatrix();
    materials["MATERIAL 2"]->begin();
    ofTranslate((radius1*1.5+radius2)*cos(ofGetElapsedTimef()*0.5),0,(radius1*1.5+radius2)*sin(ofGetElapsedTimef()*0.5));
    ofDrawSphere(0, 0, radius2);
    materials["MATERIAL 2"]->end();
    ofPopMatrix();
}

void example3D::selfDrawOverlay(){
}

void example3D::selfEnd(){
}

void example3D::selfExit(){
}

void example3D::selfKeyPressed(ofKeyEventArgs & args){
	
}
void example3D::selfKeyReleased(ofKeyEventArgs & args){
	
}

void example3D::selfMouseDragged(ofMouseEventArgs& data){
	
}

void example3D::selfMouseMoved(ofMouseEventArgs& data){
	
}

void example3D::selfMousePressed(ofMouseEventArgs& data){
	
}

void example3D::selfMouseReleased(ofMouseEventArgs& data){
	
}