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
    position.set(0.0f,0.0f,0.0f);
    oriTarget.set(0.0f,0.0f,0.0f);
    orientation.set(0.0f,0.0f,0.0f);
    ambient.set(ofFloatColor(.5f,.5f,.5f,1.f));
    diffuse.set(ofFloatColor(.5f,.5f,.5f,1.f));
    specular.set(ofFloatColor(.5f,.5f,.5f,1.f));
    spotCutOff = 45.0f;
    exponent = 0.0f;
}

UILight::UILight( string _name, ofLightType _type ){
    UILight();
    setName(_name);
    setType(_type);
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

void  UILight::move( ofPoint _pos ){
    position+=_pos;
    this->set(position);
}

void UILight::setupUI(){
    float length = (gui->getGlobalCanvasWidth()-gui->getWidgetSpacing()*5);
    float dim = gui->getGlobalSliderHeight();
    
    switch( light.getType() ){
        case OF_LIGHT_POINT:{
            addUIPos("POS", *this);
        }
            break;
            
        case OF_LIGHT_SPOT:{
            gui->addSlider("CUT OFF", 0, 90, &spotCutOff);
            gui->addSlider("EXPONENT", 0.0, 128.0, &exponent);
            gui->addSpacer();
            
            addUIPos("POS", *this);
            addUIOri("ORI", oriTarget);
        }
            break;
            
        case OF_LIGHT_DIRECTIONAL:{
            addUIOri("ORI", oriTarget);
        }
            break;
    }
    
    addUIColor("AMBIENT", ambient);
    addUIColor("DIFFUSE", diffuse);
    addUIColor("SPECULAR", specular);
}

void UILight::guiEvent(ofxUIEventArgs &e){
    
}

void UILight::enable(){
    if(bEnable){
        
        position.goTo(*this,0.1);
        orientation.goTo(oriTarget,0.1);
        
        ambient.update();
        diffuse.update();
        specular.update();
        
        switch(light.getType()){
            case OF_LIGHT_POINT:{
                light.setPosition(position);
            }
                break;
            case OF_LIGHT_SPOT:{
                light.setPosition(position);
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
        
        if(light.getType() != OF_LIGHT_DIRECTIONAL){
            light.draw();
        }
        ofTranslate( position );
        ofColor textColor = getColor();
        textColor.setBrightness(gui->getColorBack().getBrightness());
        ofSetColor( textColor );
        //ofDrawBitmapString(name, -40,60,60);
        if(light.getType() == OF_LIGHT_DIRECTIONAL){
            ofSetColor( diffuse );
            light.draw();
            ofMultMatrix( light.getGlobalTransformMatrix() );
            ofDrawArrow(ofPoint(0,0,0), ofPoint(0,0,70),20);
        }
        ofPopMatrix();
    }
}