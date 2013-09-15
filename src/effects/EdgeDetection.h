//
//  EdgeDetection.h
//  LeapMotion
//
//  Created by Patricio Gonzalez Vivo on 9/10/13.
//
//

#pragma once

#include "UIClass.h"

class EdgeDetection : public UIClass {
public:
    EdgeDetection();
    
    string getClassName(){return "EDGE_DETECTION"; }
    void setupGui();
    
    void begin();
    void end();
    
protected:
    void    guiEvent(ofxUIEventArgs &e);
    ofShader shader;
};
