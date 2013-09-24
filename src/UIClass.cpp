//
//  UIClass.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/9/13.
//
//

#include "UIClass.h"

UIClass::UIClass(){
    gui = NULL;
}

UIClass::~UIClass(){
    if (gui != NULL){
        gui->disable();
        ofRemoveListener(gui->newGUIEvent, this, &UIClass::guiEvent);
        delete gui;
    }
}

ofxUISuperCanvas* UIClass::getUI( ofxUICanvas *_parent ){

    if (gui == NULL){
        gui = new ofxUISuperCanvas( getClassName() , _parent);
        gui->copyCanvasStyle(_parent);
        gui->copyCanvasProperties(_parent);
        gui->setName( getClassName() );
        gui->setWidgetFontSize(OFX_UI_FONT_SMALL);

        ofxUIToggle *toggle = gui->addToggle("ENABLE", &bEnable);
        toggle->setLabelPosition(OFX_UI_WIDGET_POSITION_LEFT);
        gui->resetPlacer();
        gui->addWidgetDown(toggle, OFX_UI_ALIGN_RIGHT, true);
        gui->addWidgetToHeader(toggle);
        gui->addSpacer();

        setupUI();

        ofAddListener(gui->newGUIEvent, this, &UIClass::guiEvent);
    }

    return gui;
}
