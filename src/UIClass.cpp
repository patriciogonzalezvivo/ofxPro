//
//  UIClass.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/9/13.
//
//

#include "UIClass.h"

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

    ofAddListener(gui->newGUIEvent, this, &UIClass::guiEvent);

    return gui;
}
