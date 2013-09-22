//
//  UIMaterial.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/22/13.
//
//

#include "UIMaterial.h"

UIMaterial::UIMaterial(){
    diffuse.set(0.8f, 0.8f, 0.8f, 1.0f);
    specular.set(0.0f, 0.0f, 0.0f, 1.0f);
    ambient.set(0.2f, 0.2f, 0.2f, 1.0f);
    emissive.set(0.0f, 0.0f, 0.0f, 1.0f);
    shininess = 0.2;
    
    name = "MATERIAL";
};

UIMaterial::UIMaterial(string _name){
    UIMaterial::UIMaterial();
    setName(_name);
};

UIMaterial::~UIMaterial(){
};

// set colors
void UIMaterial::setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor oEmissive){
    diffuse = oDiffuse;
    ambient = oAmbient;
    specular = oSpecular;
    emissive = oEmissive;
};

void UIMaterial::setName(string _name){
    if (gui == NULL){
        name = _name;
    }
}

void UIMaterial::setupUI(){
    float length = ( gui->getGlobalCanvasWidth()- gui->getWidgetSpacing()*5)/3.;
    float dim = gui->getGlobalSliderHeight();
    
    gui->addLabel("AMBIENT", OFX_UI_FONT_SMALL);
    gui->addMinimalSlider("AR", 0.0, 1.0, &ambient.r, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider("AG", 0.0, 1.0, &ambient.g, length, dim)->setShowValue(false);
    gui->addMinimalSlider("AB", 0.0, 1.0, &ambient.b, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
    
    gui->addLabel("DIFFUSE", OFX_UI_FONT_SMALL);
    gui->addMinimalSlider("AR", 0.0, 1.0, &diffuse.r, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider("AG", 0.0, 1.0, &diffuse.g, length, dim)->setShowValue(false);
    gui->addMinimalSlider("AB", 0.0, 1.0, &diffuse.b, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
    
    gui->addLabel("EMISSIVE", OFX_UI_FONT_SMALL);
    gui->addMinimalSlider("ER", 0.0, 1.0, &emissive.r, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider("EG", 0.0, 1.0, &emissive.g, length, dim)->setShowValue(false);
    gui->addMinimalSlider("EB", 0.0, 1.0, &emissive.b, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
    
    gui->addLabel("SPECULAR", OFX_UI_FONT_SMALL);
    gui->addMinimalSlider("SR", 0.0, 1.0, &specular.r, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider("SG", 0.0, 1.0, &specular.g, length, dim)->setShowValue(false);
    gui->addMinimalSlider("SB", 0.0, 1.0, &specular.b, length, dim)->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
    
    gui->addMinimalSlider("SHINY", 0.0, 1.0, &shininess)->setShowValue(false);
}

string UIMaterial::getClassName(){
    return name;
}

void UIMaterial::guiEvent(ofxUIEventArgs &e){
    
}

// apply the Material
void UIMaterial::begin(){
    if (bEnable){
        // save previous values, opengl es cannot use push/pop attrib
        glGetMaterialfv(GL_FRONT,GL_DIFFUSE,&prev_diffuse.r);
        glGetMaterialfv(GL_FRONT,GL_SPECULAR,&prev_specular.r);
        glGetMaterialfv(GL_FRONT,GL_AMBIENT,&prev_ambient.r);
        glGetMaterialfv(GL_FRONT,GL_EMISSION,&prev_emissive.r);
        glGetMaterialfv(GL_FRONT, GL_SHININESS, &prev_shininess);
        
        glGetMaterialfv(GL_BACK,GL_DIFFUSE,&prev_diffuse_back.r);
        glGetMaterialfv(GL_BACK,GL_SPECULAR,&prev_specular_back.r);
        glGetMaterialfv(GL_BACK,GL_AMBIENT,&prev_ambient_back.r);
        glGetMaterialfv(GL_BACK,GL_EMISSION,&prev_emissive.r);
        glGetMaterialfv(GL_BACK, GL_SHININESS, &prev_shininess_back);
        
        // Material colors and properties
        glMaterialfv(GL_FRONT, GL_DIFFUSE, &diffuse.r);
        glMaterialfv(GL_FRONT, GL_SPECULAR, &specular.r);
        glMaterialfv(GL_FRONT, GL_AMBIENT, &ambient.r);
        glMaterialfv(GL_FRONT, GL_EMISSION, &emissive.r);
        glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
        
        glMaterialfv(GL_BACK, GL_DIFFUSE, &diffuse.r);
        glMaterialfv(GL_BACK, GL_SPECULAR, &specular.r);
        glMaterialfv(GL_BACK, GL_AMBIENT, &ambient.r);
        glMaterialfv(GL_BACK, GL_EMISSION, &emissive.r);
        glMaterialfv(GL_BACK, GL_SHININESS, &shininess);
    }
};

void UIMaterial::end(){
    if (bEnable){
        // Set previous material colors and properties
        glMaterialfv(GL_FRONT, GL_DIFFUSE, &prev_diffuse.r);
        glMaterialfv(GL_FRONT, GL_SPECULAR, &prev_specular.r);
        glMaterialfv(GL_FRONT, GL_AMBIENT, &prev_ambient.r);
        glMaterialfv(GL_FRONT, GL_EMISSION, &prev_emissive.r);
        glMaterialfv(GL_FRONT, GL_SHININESS, &prev_shininess);
        
        glMaterialfv(GL_BACK, GL_DIFFUSE, &prev_diffuse_back.r);
        glMaterialfv(GL_BACK, GL_SPECULAR, &prev_specular_back.r);
        glMaterialfv(GL_BACK, GL_AMBIENT, &prev_ambient_back.r);
        glMaterialfv(GL_BACK, GL_EMISSION, &prev_emissive_back.r);
        glMaterialfv(GL_BACK, GL_SHININESS, &prev_shininess_back);
    }
};