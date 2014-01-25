//
//  UIBackground.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/14/13.
//
//

#include "UIBackground.h"

UIBackground::UIBackground(){
    guis = NULL;
    
//    hueSlider = NULL;
//    satSlider = NULL;
//    briSlider = NULL;
}

void UIBackground::linkUIs( vector<UIReference> *_guis ){
    guis = _guis;
}

void UIBackground::setupUI(){
    addUIColor("A COLOR", color);
    addUIColor("B COLOR", color2);
    gui->addToggle("GRADIENT", &bGradient);
}

void UIBackground::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();

    if(name == "BRI" || name == "BRI2" || name == "GRADIENT"){
        if (guis != NULL){
            for(int i = 0; i < guis->size(); i++){
                (*guis)[i]->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(getUIBrightness()*255,OFX_UI_COLOR_BACK_ALPHA*4.0));
                (*guis)[i]->setColorBack( ofColor(getUIBrightness()*255, OFX_UI_COLOR_BACK_ALPHA*4.0));
            }
        }
    }
}

float UIBackground::getUIBrightness(){
    if(bGradient){
        return 1.0-color2.bri;
    } else {
        return 1.0-color.bri;
    }
}

void UIBackground::draw(){
    if(bEnable){
        color.update();
        color2.update();
        
        ofPushStyle();
        ofClear(0,0,0);
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        
        if(bGradient){
            ofSetSmoothLighting(true);
            ofBackgroundGradient(color, color2, OF_GRADIENT_CIRCULAR);
        } else {
            ofSetSmoothLighting(false);
            ofBackground(color);
        }
        
        ofPopStyle();
    }
}