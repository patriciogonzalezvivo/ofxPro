//
//  UIClass.cpp
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#include "UIClass.h"

UIClass::~UIClass(){
    if (gui != NULL){
        gui->disable();
        ofRemoveListener(gui->newGUIEvent, this, &UIClass::guiEvent);
    }
}

UIReference UIClass::getUIReference( ofxUICanvas *_parent ){

    UIReference tmp( new ofxUISuperCanvas(getClassName() , _parent) );
    gui = tmp;
    
    gui->copyCanvasStyle( _parent );
    gui->copyCanvasProperties( _parent );
    gui->setName( getClassName() );
    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);

    ofxUIToggle *toggle = gui->addToggle("ENABLE", &bEnable);
    toggle->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
    gui->resetPlacer();
    gui->addWidgetDown(toggle, OFX_UI_ALIGN_RIGHT, true);
    gui->addWidgetToHeader(toggle);
    gui->addSpacer();
    
    setupUI();

    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &UIClass::guiEvent);

    return gui;
}

void UIClass::addUIPos(string _NAME, aPoint *_pos, bool _justXY){
    float length = (gui->getGlobalCanvasWidth()-gui->getWidgetSpacing()*5);
    float dim = gui->getGlobalSliderHeight();
    gui->addLabel(_NAME, OFX_UI_FONT_SMALL);
    
    if(_justXY){
        gui->addMinimalSlider(_NAME+"X", 0, ofGetScreenWidth(), &(_pos->x), length/2., dim)->setShowValue(false);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        gui->addMinimalSlider(_NAME+"Y", 0, ofGetScreenHeight(), &(_pos->y), length/2., dim)->setShowValue(false);
    } else {
        gui->addMinimalSlider(_NAME+"X", -1000.0, 1000.0, &(_pos->x), length/3., dim)->setShowValue(false);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        gui->addMinimalSlider(_NAME+"Y", -1000.0, 1000.0, &(_pos->y), length/3., dim)->setShowValue(false);
        gui->addMinimalSlider(_NAME+"Z", -1000.0, 1000.0, &(_pos->z), length/3., dim)->setShowValue(false);
    }
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
}

void UIClass::addUIOri(string _NAME, aPoint *_pos, bool _justXY){
    float length = (gui->getGlobalCanvasWidth()-gui->getWidgetSpacing()*5);
    float dim = gui->getGlobalSliderHeight();
    gui->addLabel(_NAME, OFX_UI_FONT_SMALL);
    
    if(_justXY){
        
        gui->addMinimalSlider(_NAME+"X", 0, 360, &(_pos->x), length/2., dim)->setShowValue(false);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        gui->addMinimalSlider(_NAME+"Y", 0, 360, &(_pos->y), length/2., dim)->setShowValue(false);
    } else {
        gui->addMinimalSlider(_NAME+"X", -180, 180, &(_pos->x), length/3., dim)->setShowValue(false);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        gui->addMinimalSlider(_NAME+"Y", -180, 180, &(_pos->y), length/3., dim)->setShowValue(false);
        gui->addMinimalSlider(_NAME+"Z", -180, 180, &(_pos->z), length/3., dim)->setShowValue(false);
    }
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
}

void UIClass::addUIOri(string _NAME, ofPoint *_pos, bool _justXY){
    float length = (gui->getGlobalCanvasWidth()-gui->getWidgetSpacing()*5);
    float dim = gui->getGlobalSliderHeight();
    gui->addLabel(_NAME, OFX_UI_FONT_SMALL);
    
    if(_justXY){
        
        gui->addMinimalSlider(_NAME+"X", 0, 360, &(_pos->x), length/2., dim)->setShowValue(false);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        gui->addMinimalSlider(_NAME+"Y", 0, 360, &(_pos->y), length/2., dim)->setShowValue(false);
    } else {
        gui->addMinimalSlider(_NAME+"X", -180, 180, &(_pos->x), length/3., dim)->setShowValue(false);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        gui->addMinimalSlider(_NAME+"Y", -180, 180, &(_pos->y), length/3., dim)->setShowValue(false);
        gui->addMinimalSlider(_NAME+"Z", -180, 180, &(_pos->z), length/3., dim)->setShowValue(false);
    }
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
}

void UIClass::addUIPos(string _NAME, ofPoint *_pos, bool _justXY){
    float length = (gui->getGlobalCanvasWidth()-gui->getWidgetSpacing()*5);
    float dim = gui->getGlobalSliderHeight();
    gui->addLabel(_NAME, OFX_UI_FONT_SMALL);
    
    if(_justXY){
        gui->addMinimalSlider(_NAME+"X", 0, ofGetScreenWidth(), &(_pos->x), length/2., dim)->setShowValue(false);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        gui->addMinimalSlider(_NAME+"Y", 0, ofGetScreenHeight(), &(_pos->y), length/2., dim)->setShowValue(false);
        
    } else {
        gui->addMinimalSlider(_NAME+"X", -1000.0, 1000.0, &(_pos->x), length/3., dim)->setShowValue(false);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        gui->addMinimalSlider(_NAME+"Y", -1000.0, 1000.0, &(_pos->y), length/3., dim)->setShowValue(false);
        gui->addMinimalSlider(_NAME+"Z", -1000.0, 1000.0, &(_pos->z), length/3., dim)->setShowValue(false);
    }
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
}

void UIClass::addUIColor(string _NAME, aColor *_color){
    float length = (gui->getGlobalCanvasWidth()-gui->getWidgetSpacing()*5);
    float dim = gui->getGlobalSliderHeight();
    string pre = ofToLower(ofToString(_NAME[0]));
    
    gui->addLabel(_NAME, OFX_UI_FONT_SMALL);
    gui->addMinimalSlider(pre+"HUE", 0.0, 1.0, &_color->hue, length, dim);//->setShowValue(false);
    gui->addMinimalSlider(pre+"SAT", 0.0, 1.0, &_color->sat,length/2.0, dim);//->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addMinimalSlider(pre+"BRI", 0.0, 1.0, &_color->bri, length/2.0, dim);//->setShowValue(false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
}