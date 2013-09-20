//
//  UIFog.h
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/15/13.
//
//

#pragma once

#include "UIClass.h"

class UIFog : public UIClass{
public:
    
    UIFog();
    
    void    setupUI();
    
    string  getClassName(){return "FOG"; }
    
    void    begin();
    void    end();
    
    ofFloatColor *color;
    
    float   density_exponent;
    float   density;
    
protected:
    void    guiEvent(ofxUIEventArgs &e);

};
