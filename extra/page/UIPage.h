//
//  UIPage.h
//
//  Created by Patricio Gonzalez Vivo on 2/23/14.
//
//

#pragma once

#include "Page.h"
#include "UIClass.h"

class UIPage : public Page, public UIClass {
public:
    
    UIPage(){};
    virtual ~UIPage(){};
    
    string  getClassName(){return "PAGE";};
    
    void    draw();
    
protected:
    void    setupUI( );
    void    guiEvent(ofxUIEventArgs &e);
    
};
