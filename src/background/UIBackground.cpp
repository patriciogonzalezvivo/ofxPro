//
//  UIBackground.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/14/13.
//
//

#include "UIBackground.h"

UIBackground::UIBackground(){
    guis = NULL;
    cam = NULL;
    
    hueSlider = NULL;
    satSlider = NULL;
    briSlider = NULL;
    
    bChange = true;
    
    color.set(0);
    HSB.set(0,0,0);
    HSBTarget.set(0,0,0);
    
    color2.set(0);
    HSB2.set(0,0,0);
    HSBTarget2.set(0,0,0);
}

UIBackground::~UIBackground(){
    if (gui != NULL){
        gui->disable();
        ofRemoveListener(gui->newGUIEvent, this, &UIBackground::guiEvent);
        delete gui;
    }
    
    if ( hueSlider != NULL ){
        delete hueSlider;
    }
    
    if ( satSlider != NULL ){
        delete hueSlider;
    }
    
    if ( briSlider != NULL ){
        delete hueSlider;
    }
}

void UIBackground::setupUI(){
    
    gui->addSlider("HUE", 0.0, 1.0, &HSBTarget.x );
    gui->addSlider("SAT", 0.0, 1.0, &HSBTarget.y );
    gui->addSlider("BRI", 0.0, 1.0, &HSBTarget.z );
    
    ofxUIToggle *toggle = gui->addToggle("GRADIENT", false);
    
    hueSlider = gui->addSlider("HUE2", 0.0, 1.0, &HSBTarget2.x );
    satSlider = gui->addSlider("SAT2", 0.0, 1.0, &HSBTarget2.y );
    briSlider = gui->addSlider("BRI2", 0.0, 1.0, &HSBTarget2.z );
    
    gui->autoSizeToFitWidgets();
}

void UIBackground::linkUIs( vector<ofxUISuperCanvas *> *_guis ){
    guis = _guis;
}

void UIBackground::linkCamera( ofEasyCam *_cam ){
    cam = _cam;
}

void UIBackground::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name == "GRADIENT"){
        ofxUIToggle *t = (ofxUIToggle *) e.widget;
        if(t->getValue()){
            gradientMode = OF_GRADIENT_CIRCULAR;
            hueSlider->setVisible(true);
            satSlider->setVisible(true);
            briSlider->setVisible(true);
            gui->autoSizeToFitWidgets();
            if(gui->isMinified()){
                gui->setMinified(true);
            }
        } else {
            gradientMode = -1;
            hueSlider->setVisible(false);
            satSlider->setVisible(false);
            briSlider->setVisible(false);
            gui->autoSizeToFitWidgets();
        }
    }
    
    if(name == "BRI" || name == "BRI2" || name == "GRADIENT"){
        if (guis != NULL){
            float guiBgBrigtness = 0;
            
            if(gradientMode == OF_GRADIENT_CIRCULAR){
                guiBgBrigtness = HSBTarget2.z;
            }else {
                guiBgBrigtness = HSBTarget.z;
            }
            
            for(int i = 0; i < guis->size(); i++){
                (*guis)[i]->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(guiBgBrigtness*255,OFX_UI_COLOR_BACK_ALPHA*4.0));
                (*guis)[i]->setColorBack( ofColor((1.0-guiBgBrigtness)*255, OFX_UI_COLOR_BACK_ALPHA*4.0));
            }
            
        }
    }
    
    bChange = true;
}

void UIBackground::draw(){
    
    if(bEnable){
        
        if(bChange){
            
            if (HSBTarget.distance( HSB ) >= 0.00001  ){
                HSB.addForceTo( HSBTarget, true );
                HSB.update(0.1);
                color.setHsb(HSB.x, HSB.y, HSB.z);
                bChange = true;
            } else {
                bChange = false;
            }
            
            if (HSBTarget2.distance( HSB2 ) >= 0.00001 ){
                HSB2.addForceTo( HSBTarget2, true );
                HSB2.update(0.1);
                color2.setHsb(HSB2.x, HSB2.y, HSB2.z);
                bChange = true;
            } else {
                bChange = false;
            }
        }
        
        ofPushStyle();
        ofClear(0,0,0);
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        
        if(gradientMode == OF_GRADIENT_CIRCULAR){
            ofSetSmoothLighting(true);
            ofBackgroundGradient(color, color2, OF_GRADIENT_CIRCULAR);
            
        } else {
            
            ofSetSmoothLighting(false);
            ofBackground(color);
        }
        
        ofPopStyle();
    }
}