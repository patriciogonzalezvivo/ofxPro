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
    backgroundSet(new UIMapBackground() );
    
    lightAdd("DIR LIGHT ", OF_LIGHT_DIRECTIONAL );
    lightAdd("SPOT LIGHT", OF_LIGHT_SPOT);
    
    guiAdd(grid);
}

void example3D::selfGuiEvent(ofxUIEventArgs &e){

}

void example3D::selfSetupSystemGui(){
    
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
    glEnable(GL_DEPTH_TEST);
    {
        ofPushMatrix();
        ofPushStyle();
        materials["MATERIAL 1"]->begin();
        
        ofSetColor(255);
        grid.draw();
        
        ofSetColor(255);
        ofDrawSphere(0, 0, 300);
        
        materials["MATERIAL 1"]->end();
        ofPushStyle();
        ofPopMatrix();
    }
    glDisable(GL_DEPTH_TEST);
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