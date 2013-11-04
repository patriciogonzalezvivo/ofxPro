//
//  UILight.cpp
//  example
//
//  Created by Patricio Gonzalez Vivo on 9/21/13.
//
//

#include "UILight.h"

UILight::UILight(){
    
    light.setup();
    orientation.set(0.0f,0.0f,0.0f);
    ambient = ofFloatColor(.5f,.5f,.5f,1.f);
    diffuse = ofFloatColor(.5f,.5f,.5f,1.f);
    specular = ofFloatColor(.5f,.5f,.5f,1.f);
    spotCutOff = 45.0f;
    exponent = 0.0f;
}

UILight::UILight( string _name, ofLightType _type ){
    UILight();
    setName(_name);
    setType(_type);
}

UILight::~UILight(){
}

void UILight::setName(string _name){
    if (gui == NULL){
        name = _name;
    }
}

void UILight::setType( ofLightType _type ){
    if (gui == NULL){
        switch(_type){
            case OF_LIGHT_POINT:{
                light.setPointLight();
            }
                break;
            case OF_LIGHT_SPOT:{
                light.setSpotlight();
            }
                break;
                
            case OF_LIGHT_DIRECTIONAL:{
                light.setDirectional();
            }
                break;
        }
    }
}

void UILight::setupUI(){
    float length = (gui->getGlobalCanvasWidth()-gui->getWidgetSpacing()*5)/3.;
    float dim = gui->getGlobalSliderHeight();
    
    switch( light.getType() ){
        case OF_LIGHT_POINT:{
            gui->addLabel("POSITION", OFX_UI_FONT_SMALL);
            gui->addMinimalSlider("X", -1000.0, 1000.0, &x, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            gui->addMinimalSlider("Y", -1000.0, 1000.0, &y, length, dim)->setShowValue(false);
            gui->addMinimalSlider("Z", -1000.0, 1000.0, &z, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            gui->addSpacer();
        }
            break;
            
        case OF_LIGHT_SPOT:{
            gui->addSlider("CUT OFF", 0, 90, &spotCutOff);
            gui->addSlider("EXPONENT", 0.0, 128.0, &exponent);
            gui->addSpacer();
            
            gui->addLabel("POSITION", OFX_UI_FONT_SMALL);
            gui->addMinimalSlider("X", -1000.0, 1000.0, &x, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            gui->addMinimalSlider("Y", -1000.0, 1000.0, &y, length, dim)->setShowValue(false);
            gui->addMinimalSlider("Z", -1000.0, 1000.0, &z, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            gui->addSpacer();
            
            gui->addLabel("ORIENTATION", OFX_UI_FONT_SMALL);
            gui->addMinimalSlider("OX", -90.0, 90.0, &orientation.x, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            gui->addMinimalSlider("OY", -90.0, 90.0, &orientation.y, length, dim)->setShowValue(false);
            gui->addMinimalSlider("OZ", -90.0, 90.0, &orientation.z, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            gui->addSpacer();
        }
            break;
            
        case OF_LIGHT_DIRECTIONAL:{
            gui->addLabel("ORIENTATION", OFX_UI_FONT_SMALL);
            gui->addMinimalSlider("OX", -90.0, 90.0, &orientation.x, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            gui->addMinimalSlider("OY", -90.0, 90.0, &orientation.y, length, dim)->setShowValue(false);
            gui->addMinimalSlider("OZ", -90.0, 90.0, &orientation.z, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            gui->addSpacer();
        }
            break;
    }
    
    gui->addLabel("AMBIENT", OFX_UI_FONT_SMALL);
    gui->addMinimalSlider("AR", 0.0, 1.0, &ambient.r, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider("AG", 0.0, 1.0, &ambient.g, length, dim)->setShowValue(false);
    gui->addMinimalSlider("AB", 0.0, 1.0, &ambient.b, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSpacer();
    gui->addLabel("DIFFUSE", OFX_UI_FONT_SMALL);
    gui->addMinimalSlider("DR", 0.0, 1.0, &diffuse.r, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider("DG", 0.0, 1.0, &diffuse.g, length, dim)->setShowValue(false);
    gui->addMinimalSlider("DB", 0.0, 1.0, &diffuse.b, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSpacer();
    gui->addLabel("SPECULAR", OFX_UI_FONT_SMALL);
    gui->addMinimalSlider("SR", 0.0, 1.0, &specular.r, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider("SG", 0.0, 1.0, &specular.g, length, dim)->setShowValue(false);
    gui->addMinimalSlider("SB", 0.0, 1.0, &specular.b, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
}

void UILight::guiEvent(ofxUIEventArgs &e){
    
}

void UILight::enable(){
    if(bEnable){
        switch(light.getType()){
            case OF_LIGHT_POINT:{
                light.setPosition(*this);
            }
                break;
            case OF_LIGHT_SPOT:{
                light.setPosition(*this);
                light.setOrientation(orientation);
                light.setSpotlightCutOff(spotCutOff);
                light.setSpotConcentration(exponent);
            }
                break;
                
            case OF_LIGHT_DIRECTIONAL:{
                light.setOrientation(orientation);
            }
                break;
        }
        
        light.setAmbientColor(ambient);
        light.setDiffuseColor(diffuse);
        light.setSpecularColor(specular);
        light.enable();
    } else {
        light.disable();
    }
}

void UILight::disable(){
    light.disable();
}

void UILight::play(){
    light.setup();
}

void UILight::stop(){
    light.destroy();
}

void UILight::draw(){
    if (bEnable){
        ofSetColor( diffuse );
        ofPushMatrix();
        light.draw();
        ofTranslate( *this );
        
        ofColor textColor = getColor();
        textColor.setBrightness(gui->getColorBack().getBrightness());
        ofSetColor( textColor );
        ofDrawBitmapString(name, 20,20,20);
        
        if(light.getType() == OF_LIGHT_DIRECTIONAL){
            ofSetColor( diffuse );
            ofMultMatrix( light.getGlobalTransformMatrix() );
            ofDrawArrow(ofPoint(0,0,0), ofPoint(0,0,70),20);
            ofDrawSphere(0, 0, 10);
        }
        ofPopMatrix();
    }
}