//
//  UIPage.cpp
//
//  Created by Patricio Gonzalez Vivo on 2/23/14.
//
//

#include "UIPage.h"

void UIPage::setupUI(){
    gui->addIntSlider("Mesh_definition", 1, 200, &meshDefinition);
    gui->add2DPad("Hand_Position", ofPoint(0,1), ofPoint(0,1), &hand);
    gui->addSlider("Hand_Lerp_Pct", 0.0,1.0, &handLerpPct);

    gui->addSlider("Axis", 0.0, 100.0, &coneAxisDist);
    
    gui->addToggle("Left_to_Right", &bLeftRight);
    gui->addToggle("Flip_Horizontal", &bFlipHorizontal);
    gui->addToggle("Flip_Vertical", &bFlipVertical);
    gui->addToggle("Debug", &bDebug);
}

void UIPage::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    bChange = true;
}

void UIPage::draw(){
    if(bEnable){
        Page::draw();
    }
}