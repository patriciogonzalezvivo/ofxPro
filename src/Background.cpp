//
//  Background.cpp
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/14/13.
//
//

#include "Background.h"

Background::Background(){
    guis = NULL;
}

Background::~Background(){
    
}

void Background::setupUI(){
    
    gui->addSlider("HUE", 0.0, 1.0, &HSB.x );
    gui->addSlider("SAT", 0.0, 1.0, &HSB.y );
    gui->addSlider("BRI", 0.0, 1.0, &HSB.z );
    
    ofxUIToggle *toggle = gui->addToggle("GRADIENT", false);
    
    hueSlider = gui->addSlider("HUE2", 0.0, 1.0, &HSB2.x );
    satSlider = gui->addSlider("SAT2", 0.0, 1.0, &HSB2.y );
    briSlider = gui->addSlider("BRI2", 0.0, 1.0, &HSB2.z );
    
    gui->autoSizeToFitWidgets();
}

void Background::setLinkedUIs( vector<ofxUISuperCanvas *> *_guis ){
    guis = _guis;
}

void Background::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name == "BRI"){
        if (guis != NULL){
            for(int i = 0; i < guis->size(); i++){
                (*guis)[i]->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(HSB.z*255,OFX_UI_COLOR_BACK_ALPHA*4.0));
                (*guis)[i]->setColorBack( ofColor((1.0-HSB.z)*255, OFX_UI_COLOR_BACK_ALPHA*4.0));
            }
        }
    } else if(name == "GRADIENT"){
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
    
    color.setHsb(HSB.x, HSB.y, HSB.z);
    color2.setHsb(HSB2.x, HSB2.y, HSB2.z);
}

void Background::draw(){
    
    if(bEnable){
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