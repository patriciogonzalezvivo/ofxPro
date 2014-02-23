//
//  UIPage.cpp
//
//  Created by Patricio Gonzalez Vivo on 2/23/14.
//
//

#include "UIPage.h"

void UIPage::setupUI(){
    gui->add2DPad("Hand_Position", ofPoint(0,1), ofPoint(0,1), &hand);
    gui->addSlider("Mesh_definition", 1, 100, &meshDefinition);
    gui->addToggle("Flip_vertical", &bFlipVertical);
    gui->addToggle("Debug", &bDebug);
}

void UIPage::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name == "Hand_Position"){
        bChange = true;
    }
}

void UIPage::draw(){
    if(bEnable){
        Page::draw();
    }
}