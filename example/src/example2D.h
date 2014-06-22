//
//  example2D.h
//
//  Created by Patricio Gonzalez Vivo on 6/21/14.
//
//

#pragma once

#include "ofxPro.h"

class example2D : public ofx2DPro {
public:
    
    string getSystemName(){
		return "example2D";
	}
    
    void selfSetupGuis();
    void selfGuiEvent(ofxUIEventArgs &e);
    
    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);

    void selfSetupRenderGui();
    void guiRenderEvent(ofxUIEventArgs &e);
    
    void selfSetup();
    
    void selfBegin();
    
    void selfUpdate();
    
    void selfDraw();
    
	void selfEnd();
    void selfExit();
    
    void selfKeyPressed(ofKeyEventArgs & args);
    void selfKeyReleased(ofKeyEventArgs & args);
    
    void selfMouseDragged(ofMouseEventArgs& data);
    void selfMouseMoved(ofMouseEventArgs& data);
    void selfMousePressed(ofMouseEventArgs& data);
    void selfMouseReleased(ofMouseEventArgs& data);
    
    
    float radius;
};