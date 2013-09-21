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
                                  uniform float time;
                                  
                                  uniform float freq;
                                  uniform float pct;
                                  
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
                                      
                                      float t = float(time * freq);
                                      color *= (1.0+( rand(st+t*.00001) -0.2) * pct);
                                      
                                      gl_FragColor = vec4(color,1.0);
                                  }
                                  );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
    fbo.allocate(ofGetScreenWidth(),ofGetScreenHeight());
    
    ofAddListener(ofEvents().update, this, &UISuperBackground::update);
}

void UISuperBackground::setupUI(){
    
    gui->addSlider("grain_pct", 0.0, 1.0, &grainPct);
    gui->addSlider("HUE", 0.0, 1.0, &HSBTarget.x );
    gui->addSlider("SAT", 0.0, 1.0, &HSBTarget.y );
    gui->addSlider("BRI", 0.0, 1.0, &HSBTarget.z );
    
    ofxUIToggle *toggle = gui->addToggle("GRADIENT", false);
    
    hueSlider = gui->addSlider("HUE2", 0.0, 1.0, &HSBTarget2.x );
    satSlider = gui->addSlider("SAT2", 0.0, 1.0, &HSBTarget2.y );
    briSlider = gui->addSlider("BRI2", 0.0, 1.0, &HSBTarget2.z );
    
    gui->autoSizeToFitWidgets();
}

void UISuperBackground::update(ofEventArgs & args){
    if(bEnable){
        
        if( fbo.getWidth() != ofGetWidth() || fbo.getHeight() != ofGetHeight() ){
            fbo.allocate(ofGetWidth(), ofGetHeight());
            bChange = true;
        }
        
        if(bChange){
            
            if (HSBTarget.distance( HSB ) >= 0.00001  ){
                HSB.addForceTo( HSBTarget, true );
                HSB.update(0.1);
                color.setHsb(HSB.x, HSB.y, HSB.z);
                color.a = 1.0;
                bChange = true;
            } else {
                bChange = false;
            }
            
            if (HSBTarget2.distance( HSB2 ) >= 0.00001 ){
                HSB2.addForceTo( HSBTarget2, true );
                HSB2.update(0.1);
                color2.setHsb(HSB2.x, HSB2.y, HSB2.z);
                color2.a = 1.0;
                bChange = true;
            } else {
                bChange = false;
            }
            
            if ( gradientMode == OF_GRADIENT_CIRCULAR ){
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
                mesh.draw();
                fbo.end();
            }
        }
    }
}

void UISuperBackground::draw(){
    if(bEnable){
        ofPushStyle();
        
        if ( gradientMode == OF_GRADIENT_CIRCULAR ){
            
            ofBackground( color2 );
            
            shader.begin();
            shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
            shader.setUniform1f("time", ofGetElapsedTimef());
            shader.setUniform1f("pct", grainPct);
            shader.setUniform1f("freq", 0.001);
            fbo.draw(0, 0);
            shader.end();

            
        } else {
            ofSetSmoothLighting(false);
            ofBackground(color);
        }
        
        ofPopStyle();
    }
}