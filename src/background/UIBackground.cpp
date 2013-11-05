//
//  UIBackground.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/14/13.
//
//

#include "UIBackground.h"

UIBackground::UIBackground(){
    guis = NULL;
    
    hueSlider = NULL;
    satSlider = NULL;
    briSlider = NULL;
}

void UIBackground::linkUIs( vector<UIReference> *_guis ){
    guis = _guis;
}

void UIBackground::setupUI(){
//    gui->addSlider("change_Speed", 0.0, 0.5, &speed);
    
    gui->addLabel("Colors");
    gui->addSlider("HUE", 0.0, 1.0, &color.hue);
    gui->addSlider("SAT", 0.0, 1.0, &color.sat);
    gui->addSlider("BRI", 0.0, 1.0, &color.bri);
    
    gui->addToggle("GRADIENT", &bGradient);
    hueSlider = gui->addSlider("HUE2", 0.0, 1.0, &color2.hue);
    satSlider = gui->addSlider("SAT2", 0.0, 1.0, &color2.sat);
    briSlider = gui->addSlider("BRI2", 0.0, 1.0, &color2.bri);
    
    gui->autoSizeToFitWidgets();
}

void UIBackground::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name == "GRADIENT"){
        if(bGradient){
            hueSlider->setVisible(true);
            satSlider->setVisible(true);
            briSlider->setVisible(true);
            gui->autoSizeToFitWidgets();
            if(gui->isMinified()){
                gui->setMinified(true);
            }
        } else {
            hueSlider->setVisible(false);
            satSlider->setVisible(false);
            briSlider->setVisible(false);
            gui->autoSizeToFitWidgets();
        }
    }
    
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