/**********************************************************************************
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/
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
            addUIPos("POS", this);
        }
            break;
            
        case OF_LIGHT_SPOT:{
            gui->addSlider("CUT OFF", 0, 90, &spotCutOff);
            gui->addSlider("EXPONENT", 0.0, 128.0, &exponent);
            gui->addSpacer();
            
            addUIPos("POS", this);
            addUIOri("ORI", &oriTarget);
        }
            break;
            
        case OF_LIGHT_DIRECTIONAL:{
            addUIOri("ORI", &oriTarget);
        }
            break;
    }
    
    addUIColor("AMBIENT", &ambient);
    addUIColor("DIFFUSE", &diffuse);
    addUIColor("SPECULAR", &specular);
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
//    light.destroy();
    light.disable();
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