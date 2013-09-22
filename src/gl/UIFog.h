//
//  UIFog.h
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/15/13.
//
//

#pragma once

#include "UIClass.h"
#include "UIBackground.h"

class UIFog : public UIClass{
public:
    
    UIFog();
    
    void    setupUI();
    void    setColor( ofColor *_color );
    void    linkColor( UIBackground *_background );
    
    string  getClassName(){return "FOG"; }
    
    void    begin();
    void    end();
    
    float   density_exponent;
    float   density;
    
protected:
    void    guiEvent(ofxUIEventArgs &e);
    ofFloatColor *color;

};
