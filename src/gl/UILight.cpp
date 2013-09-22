//
//  UILight.cpp
//  example
//
//  Created by Patricio Gonzalez Vivo on 9/21/13.
//
//

#include "UILight.h"

UILight::UILight(){
    gui = NULL;
    light.setup();
    lightPos.set(0.0f,0.0f,0.0f);
    lightOrientation.set(0.0f,0.0f,0.0f);
    lightAmbient = ofFloatColor(.5f,.5f,.5f,1.f);
    lightDiffuse = ofFloatColor(.5f,.5f,.5f,1.f);
    lightSpecular = ofFloatColor(.5f,.5f,.5f,1.f);
    lightSpotCutOff = 45.0f;
    lightExponent = 0.0f;
}

UILight::UILight( string _name, ofLightType _type ){
    UILight();
    setName(_name);
    setType(_type);
}

UILight::~UILight(){
    light.destroy();
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
            gui->addMinimalSlider("X", -1000.0, 1000.0, &lightPos.x, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            gui->addMinimalSlider("Y", -1000.0, 1000.0, &lightPos.y, length, dim)->setShowValue(false);
            gui->addMinimalSlider("Z", -1000.0, 1000.0, &lightPos.z, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            gui->addSpacer();
        }
            break;
            
        case OF_LIGHT_SPOT:{
            gui->addSlider("CUT OFF", 0, 90, &lightSpotCutOff);
            gui->addSlider("EXPONENT", 0.0, 128.0, &lightExponent);
            gui->addSpacer();
            
            gui->addLabel("POSITION", OFX_UI_FONT_SMALL);
            gui->addMinimalSlider("X", -1000.0, 1000.0, &lightPos.x, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            gui->addMinimalSlider("Y", -1000.0, 1000.0, &lightPos.y, length, dim)->setShowValue(false);
            gui->addMinimalSlider("Z", -1000.0, 1000.0, &lightPos.z, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            gui->addSpacer();
            
            gui->addLabel("ORIENTATION", OFX_UI_FONT_SMALL);
            gui->addMinimalSlider("OX", -90.0, 90.0, &lightOrientation.x, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            gui->addMinimalSlider("OY", -90.0, 90.0, &lightOrientation.y, length, dim)->setShowValue(false);
            gui->addMinimalSlider("OZ", -90.0, 90.0, &lightOrientation.z, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            gui->addSpacer();
        }
            break;
            
        case OF_LIGHT_DIRECTIONAL:{
            gui->addLabel("ORIENTATION", OFX_UI_FONT_SMALL);
            gui->addMinimalSlider("OX", -90.0, 90.0, &lightOrientation.x, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
            gui->addMinimalSlider("OY", -90.0, 90.0, &lightOrientation.y, length, dim)->setShowValue(false);
            gui->addMinimalSlider("OZ", -90.0, 90.0, &lightOrientation.z, length, dim)->setShowValue(false);
            gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
            gui->addSpacer();
        }
            break;
    }
    
    gui->addLabel("AMBIENT", OFX_UI_FONT_SMALL);
    gui->addMinimalSlider("AR", 0.0, 1.0, &lightAmbient.r, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider("AG", 0.0, 1.0, &lightAmbient.g, length, dim)->setShowValue(false);
    gui->addMinimalSlider("AB", 0.0, 1.0, &lightAmbient.b, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSpacer();
    gui->addLabel("DIFFUSE", OFX_UI_FONT_SMALL);
    gui->addMinimalSlider("DR", 0.0, 1.0, &lightDiffuse.r, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider("DG", 0.0, 1.0, &lightDiffuse.g, length, dim)->setShowValue(false);
    gui->addMinimalSlider("DB", 0.0, 1.0, &lightDiffuse.b, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addSpacer();
    gui->addLabel("SPECULAR", OFX_UI_FONT_SMALL);
    gui->addMinimalSlider("SR", 0.0, 1.0, &lightSpecular.r, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider("SG", 0.0, 1.0, &lightSpecular.g, length, dim)->setShowValue(false);
    gui->addMinimalSlider("SB", 0.0, 1.0, &lightSpecular.b, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
}

void UILight::guiEvent(ofxUIEventArgs &e){
    
}

void UILight::enable(){
    if(bEnable){
        switch(light.getType()){
            case OF_LIGHT_POINT:{
                light.setPosition(lightPos);
            }
                break;
            case OF_LIGHT_SPOT:{
                light.setPosition(lightPos);
                light.setOrientation(lightOrientation);
                light.setSpotlightCutOff(lightSpotCutOff);
                light.setSpotConcentration(lightExponent);
            }
                break;
                
            case OF_LIGHT_DIRECTIONAL:{
                light.setOrientation(lightOrientation);
            }
                break;
        }
        
        light.setAmbientColor(lightAmbient);
        light.setDiffuseColor(lightDiffuse);
        light.setSpecularColor(lightSpecular);
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
    
    
    ofPushMatrix();
    ofScale(1,-1,1);
    light.draw();
    ofTranslate(lightPos);
    ofDrawBitmapString(name, ofPoint(10,10,10));
    ofPopMatrix();
}