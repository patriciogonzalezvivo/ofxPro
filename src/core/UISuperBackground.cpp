//
//  UISuperBackground.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/20/13.
//
//

#include "UISuperBackground.h"

#define STRINGIFY(A) #A

UISuperBackground::UISuperBackground(){
    UIBackground();
    
    string fragShader = STRINGIFY(
                                  uniform sampler2DRect tex0;
                                  uniform vec2 resolution;
                                  
                                  uniform vec3 bgColor1;
                                  uniform vec3 bgColor2;
                                  
                                  uniform float time;
                                  uniform float value;
                                  
                                  float rand(vec2 co){
                                      return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
                                  }
                                  
                                  float rand(float c){
                                      return rand(vec2(c,1.0));
                                  }
                                  
                                  void main(){
                                      vec2 st = gl_FragCoord.xy/resolution.xy;
                                      st.y = 1.0 - st.y;
                                      
                                      vec3 color = texture2DRect(tex0, st*resolution).rgb;
                                      
                                      float t = float(time * 0.0001);
                                      float pct = (1.0+( rand(st+t*.00001) -0.2) * value);
                                      
                                      color = mix(bgColor1*0.8,color,pct);
                                      
                                      gl_FragColor = vec4(color,1.0);
                                  }
                                  );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
    fbo.allocate(ofGetScreenWidth(),ofGetScreenHeight());
    
    ofAddListener(ofEvents().update, this, &UISuperBackground::update);
}

void UISuperBackground::setupUI(){
    gui->addSlider("value", 0.0, 1.0, &value);
    UIBackground::setupUI();
}

void UISuperBackground::update(ofEventArgs & args){
    if(bEnable){
        
        if( fbo.getWidth() != ofGetWidth() || fbo.getHeight() != ofGetHeight() ){
            fbo.allocate(ofGetWidth(), ofGetHeight());
        }
        
        color.update();
        
        if (bGradient){
            color2.update();
            
            float w = ofGetWidth();
            float h = ofGetHeight();
            mesh.clear();
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            // this could be optimized by building a single mesh once, then copying
            // it and just adding the colors whenever the function is called.
            ofVec2f center(ofGetWidth()*0.5,ofGetHeight()*0.5);
            mesh.addTexCoord(center);
            mesh.addVertex(center);
            mesh.addColor( color );
            int n = 32; // circular gradient resolution
            float angleBisector = TWO_PI / (n * 2);
            float smallRadius = ofDist(0, 0, w / 2, h / 2);
            float bigRadius = smallRadius / cos(angleBisector);
            for(int i = 0; i <= n; i++) {
                float theta = i * TWO_PI / n;
                mesh.addTexCoord( center + ofVec2f(sin(theta), cos(theta)) * bigRadius );
                mesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
                mesh.addColor( color2 );
            }
            
            fbo.begin();
            ofClear( color2 );
            ofSetColor(255);
            mesh.draw();
            fbo.end();
        } else {
            fbo.begin();
            ofClear( color );
            ofSetColor(255);
            fbo.end();
        }
    }
}

void UISuperBackground::draw(){
    if(bEnable){
        ofPushStyle();
        
        if (bGradient){
            ofBackground(color2);
        } else {
            ofBackground(color);
        }
        
        shader.begin();
        shader.setUniform3f("bgColor1", color.r, color.g, color.b);
        shader.setUniform3f("bgColor2", color2.r, color2.g, color2.b);
        shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform1f("value", value);
        ofSetColor(255);
        fbo.draw(0, 0);
        shader.end();
        
        ofPopStyle();
    }
}