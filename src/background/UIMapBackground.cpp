//
//  UIMapBackground.cpp
//
//  Created by Patricio Gonzalez Vivo on 1/21/14.
//
//

#include "UIMapBackground.h"
#define STRINGIFY(A) #A

UIMapBackground::UIMapBackground(){
    UIBackground();

    string vertShader = STRINGIFY(
                                  varying vec2 oTexCoord;
                                  void main(){
                                      oTexCoord = gl_MultiTexCoord0.xy;
                                      gl_Position = ftransform();
                                  }
                                  );
    
    shader.setupShaderFromSource(GL_VERTEX_SHADER, vertShader);
    string fragShader = STRINGIFY(
                                  uniform sampler2DRect image;
                                  uniform vec3 colorOne;
                                  uniform vec3 colorTwo;
                                  uniform float gradientExponent;
                                  varying vec2 oTexCoord;
                                  
                                  void main(){
                                      gl_FragColor = vec4( mix(colorTwo,colorOne, pow(texture2DRect(image,oTexCoord).r, gradientExponent)), 1.0);
                                  }
                                  );
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
    backgrounds = NULL;
    bMeshReady = false;
    
    ofAddListener(ofEvents().update, this, &UIMapBackground::update);
    ofAddListener(ofEvents().windowResized , this, &UIMapBackground::windowResized);
}

void UIMapBackground::setupUI(){

    vector<string> backgroundsList;
    string path = "GUI/backgrounds";
    ofDirectory backgroundsDir(path);
    if(backgroundsDir.exists()){
		backgroundsDir.listDir();
        int total = backgroundsDir.getFiles().size();
        for (int i = 0; i < total; i++) {
            backgroundsList.push_back( backgroundsDir.getName(i) );
        }
        
        if(backgrounds == NULL){
            backgrounds = gui->addRadio("Background Maps", backgroundsList);
        }
    }
    
    gui->addSlider("Exp", 0.0, 1.0, &exp);
    
    gui->addSpacer();
    UIBackground::setupUI();
}

void UIMapBackground::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();

    if(name == "aBRI" || name == "bBRI" || name == "GRADIENT"){
        if (guis != NULL){
            for(int i = 0; i < guis->size(); i++){
                (*guis)[i]->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(getUIBrightness()*255,OFX_UI_COLOR_BACK_ALPHA*4.0));
                (*guis)[i]->setColorBack( ofColor(getUIBrightness()*255, OFX_UI_COLOR_BACK_ALPHA*4.0));
            }
        }
    } else if (name.find('.') != string::npos){
        ofxUIToggle *t = (ofxUIToggle*)e.widget;
        if (t != NULL){
            if (t->getValue()){
                ofLoadImage(texture, "GUI/backgrounds/"+name);
            }
        }
    }
    
    
}

void UIMapBackground::update(ofEventArgs & args){
    if(bEnable){
        if(!bMeshReady){
            getBackgroundMesh(mesh, texture, ofGetWidth(), ofGetHeight());
            bMeshReady = true;
        }
        
        color.update();
        if (bGradient){
            color2.update();
        }
    }
}

void UIMapBackground::windowResized(ofResizeEventArgs & args){
    bMeshReady = false;
}

void UIMapBackground::getBackgroundMesh(ofMesh& _mesh, ofTexture& _image, float _width, float _height){
	ofRectangle screenRect(0,0,_width,_height);
	ofRectangle imageRect(0,0,_image.getWidth(),_image.getHeight());
	
	imageRect.scaleTo(screenRect, OF_ASPECT_RATIO_KEEP_BY_EXPANDING);
    
    _mesh.clear();
    
	_mesh.addVertex(ofVec3f(imageRect.getMinX(),imageRect.getMinY()));
	_mesh.addTexCoord(ofVec2f(0,0));
    
	_mesh.addVertex(ofVec3f(imageRect.getMinX(),imageRect.getMaxY()));
	_mesh.addTexCoord(ofVec2f(0,_image.getHeight()));
    
	_mesh.addVertex(ofVec3f(imageRect.getMaxX(),imageRect.getMinY()));
	_mesh.addTexCoord(ofVec2f(_image.getWidth(),0));
    
	_mesh.addVertex(ofVec3f(imageRect.getMaxX(),imageRect.getMaxY()));
	_mesh.addTexCoord(ofVec2f(_image.getWidth(),_image.getHeight()));
	
	_mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
}

void UIMapBackground::draw(){
    if(bEnable){
        ofBackground(color);
        
        if (bGradient && texture.isAllocated()){
            
            ofPushStyle();
            ofSetColor(255);
            
            shader.begin();
            shader.setUniformTexture("image", texture, 1);
            shader.setUniform3f("colorOne", color.r, color.g, color.b);
            shader.setUniform3f("colorTwo", color2.r, color2.g, color2.b);
            shader.setUniform1f("gradientExponent", exp);
            mesh.draw();
            shader.end();
            
            ofPopStyle();
        }
        
        
    }
}