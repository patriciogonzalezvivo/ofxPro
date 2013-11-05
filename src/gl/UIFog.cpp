//
//  UIFog.cpp
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/15/13.
//
//

#include "UIFog.h"

UIFog::UIFog(){
    bgColor = NULL;
}

void UIFog::setupUI(){
    
    gui->addSlider("Density", 0.0, 1.0, &density);
    gui->addSlider("Density_EXP", -3.0, 3.0, &density_exponent);
    
    gui->addSpacer();
    gui->addToggle("Mach_Background", &bMachBackground);
    
    red = gui->addSlider("Red", 0.0, 1.0, &color.r);
    green = gui->addSlider("Green", 0.0, 1.0, &color.g);
    blue = gui->addSlider("Blue", 0.0, 1.0, &color.b);
}

void UIFog::setColor( ofColor _color ){
    if(bMachBackground){
        bMachBackground = false;
    } else {
        color = _color;
    }
}

void UIFog::linkColor( UIBackground *_background ){
    bgColor = &_background->color;
}

void UIFog::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name == "Mach_Background"){
        if(bMachBackground){
            red->setVisible(false);
            green->setVisible(false);
            blue->setVisible(false);
            gui->autoSizeToFitWidgets();
        } else {
            red->setVisible(true);
            green->setVisible(true);
            blue->setVisible(true);
            gui->autoSizeToFitWidgets();
            if(gui->isMinified()){
                gui->setMinified(true);
            }
        }
    }
}

void UIFog::begin(){
    if (bEnable){
        float FogCol[3];
        if (bMachBackground && bgColor != NULL){
            FogCol[0] = bgColor->r;
            FogCol[1] = bgColor->g;
            FogCol[2] = bgColor->b;
        } else {
            FogCol[0] = color.r;
            FogCol[1] = color.g;
            FogCol[2] = color.b;
        }

#ifdef TARGET_RASPBERRY_PI
        
#else
        GLuint fogfilter= 1;
        GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };
        
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, fogMode[fogfilter]);                                // Fog Mode
        glFogfv(GL_FOG_COLOR , FogCol);                                         // Set the fog color
        glFogf(GL_FOG_DENSITY, powf(10.0, (int)density_exponent) * density);    // Thin the fog out a little
        glHint(GL_FOG_HINT, GL_DONT_CARE);                                      // Fog Hint Value
#endif
    }
}

void UIFog::end(){
    if (bEnable){
#ifdef TARGET_RASPBERRY_PI
        
#else
        glDisable(GL_FOG);
#endif
    }
    
}
