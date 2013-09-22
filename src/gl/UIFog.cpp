//
//  UIFog.cpp
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/15/13.
//
//

#include "UIFog.h"

UIFog::UIFog(){
    color = NULL;
}

void UIFog::setupUI(){
    
//    if (color == NULL){
//        color = new ofFloatColor(0.8);
//    
//        gui->addSlider("Red", 0.0, 1.0, &color->r);
//        gui->addSlider("Green", 0.0, 1.0, &color->g);
//        gui->addSlider("Blue", 0.0, 1.0, &color->b);
//    }
    
    gui->addSlider("Density", 0.0, 1.0, &density);
    gui->addSlider("Density_EXP", -3.0, 3.0, &density_exponent);
}

void UIFog::guiEvent(ofxUIEventArgs &e){

}

void UIFog::begin(){
    
    if (bEnable){
        if (color == NULL){
            color = new ofFloatColor(0.8);
        }
        
        float FogCol[3]={ color->r, color->g, color->b};
        
        GLuint fogfilter= 1;
        GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };
        
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, fogMode[fogfilter]);    // Fog Mode
        glFogfv(GL_FOG_COLOR, FogCol);              // Set the fog color
        glFogf(GL_FOG_DENSITY, powf(10.0, (int)density_exponent) * density);             // Thin the fog out a little
        glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
    }
}

void UIFog::end(){
    if (bEnable){
        glDisable(GL_FOG);
    }
    
}
