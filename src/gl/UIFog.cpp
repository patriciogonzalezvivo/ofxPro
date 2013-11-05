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
    
    gui->addSlider("Density", 0.0, 1.0, &density);
    gui->addSlider("Density_EXP", -3.0, 3.0, &density_exponent);
    
    if (color == NULL){
        color = new ofFloatColor(0.8);
        
        red = gui->addSlider("Red", 0.0, 1.0, &color->r);
        green = gui->addSlider("Green", 0.0, 1.0, &color->g);
        blue = gui->addSlider("Blue", 0.0, 1.0, &color->b);
    }
}

void UIFog::setColor( ofColor *_color ){
    if (color == NULL){
        color = new ofFloatColor( *_color );
    }
}

void UIFog::linkColor( UIBackground *_background ){
    if (color != NULL){
        //delete color;
        
        red->setVisible(false);
        green->setVisible(false);
        blue->setVisible(false);
        
        gui->autoSizeToFitWidgets();
    }
    
    color = &_background->getColor();
}

void UIFog::guiEvent(ofxUIEventArgs &e){

}

void UIFog::begin(){
    
    if (bEnable){
        if (color == NULL){
            color = new ofFloatColor(0.8);
        }
        
        float FogCol[3]={ color->r, color->g, color->b};
        
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
