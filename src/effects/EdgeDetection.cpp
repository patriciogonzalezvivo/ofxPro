//
//  EdgeDetection.cpp
//  LeapMotion
//
//  Created by Patricio Gonzalez Vivo on 9/10/13.
//
//

#include "EdgeDetection.h"

#define STRINGIFY(A) #A

EdgeDetection::EdgeDetection(){
    
    string fragShader = STRINGIFY(uniform sampler2DRect tex0;
                                  uniform vec2 resolution;
                                  
                                  float getIntensity(vec2 u){
                                      vec3 a = texture2DRect(tex0,u).xyz;
                                      return (a.x+a.y+a.z)/3.0;
                                  }
                                  
                                  void main(void){
                                      vec2 uv = gl_FragCoord.xy;
                                      uv.y = resolution.y - uv.y;
                                      vec2 p = vec2(1.0);
                                      
                                      float avg = 0.0;
                                      avg += getIntensity(uv+vec2(p.x,0.0));
                                      avg += getIntensity(uv+vec2(-p.x,0.0));
                                      avg += getIntensity(uv+vec2(0.0,p.y));
                                      avg += getIntensity(uv+vec2(0.0,-p.y));
                                      avg += getIntensity(uv+vec2(p.x,p.y));
                                      avg += getIntensity(uv+vec2(-p.x,-p.y));
                                      avg += getIntensity(uv+vec2(p.x,-p.y));
                                      avg += getIntensity(uv+vec2(-p.x,p.y));
                                      avg /= 8.0;
                                      
                                      float result = (1.0-getIntensity(uv)) + avg;
                                      result = (1.0 - result) * 10.0;
                                      
                                      gl_FragColor = vec4(vec3(result),1.0);
                                  }
                                  );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
    bEnable = true;
}

void EdgeDetection::setupGui(){

}

void EdgeDetection::guiEvent(ofxUIEventArgs &e){
    
    if (gui != NULL){
        string name = e.widget->getName();
        
        if (name == "ENABLE"){
            ofxUIToggle *t = (ofxUIToggle *) e.widget;
            bEnable = t->getValue();
        }
        
    }
}

void EdgeDetection::begin(){
    if (bEnable){
        shader.begin();
        shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
    }
}

void EdgeDetection::end(){
    if (bEnable){
        shader.end();
    }
}