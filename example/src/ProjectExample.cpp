//
//  ProjectExample.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/6/13.
//
//

#include "ProjectExample.h"


void ProjectExample::selfSetup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
}

void ProjectExample::selfSetupGui(){
    
    lightAdd("DIR LIGHT ", OF_LIGHT_DIRECTIONAL );
    lightAdd("SPOT LIGHT", OF_LIGHT_SPOT);
}

void ProjectExample::selfGuiEvent(ofxUIEventArgs &e){
    
}

void ProjectExample::selfSetupSystemGui(){
    
}

void ProjectExample::guiSystemEvent(ofxUIEventArgs &e){
    
}

void ProjectExample::selfSetupRenderGui(){

}

void ProjectExample::guiRenderEvent(ofxUIEventArgs &e){
    
}

void ProjectExample::selfPresetLoaded(string presetPath){
	
}

void ProjectExample::selfBegin(){
}

void ProjectExample::selfSceneTransformation(){
	
}

void ProjectExample::selfUpdate(){
    
}

void ProjectExample::selfDraw(){
    glEnable(GL_DEPTH_TEST);
    {
        ofPushMatrix();
        ofPushStyle();
        materials["MATERIAL 1"]->begin();
        
        ofSetColor(255);
        ofDrawSphere(0, 0, 300);
        
        materials["MATERIAL 1"]->end();
        ofPushStyle();
        ofPopMatrix();
    }
    glDisable(GL_DEPTH_TEST);
}

void ProjectExample::selfDrawDebug(){
	lightsDraw();
}

void ProjectExample::selfEnd(){
}

void ProjectExample::selfExit(){
}

void ProjectExample::selfKeyPressed(ofKeyEventArgs & args){
	
}
void ProjectExample::selfKeyReleased(ofKeyEventArgs & args){
	
}

void ProjectExample::selfMouseDragged(ofMouseEventArgs& data){
	
}

void ProjectExample::selfMouseMoved(ofMouseEventArgs& data){
	
}

void ProjectExample::selfMousePressed(ofMouseEventArgs& data){
	
}

void ProjectExample::selfMouseReleased(ofMouseEventArgs& data){
	
}