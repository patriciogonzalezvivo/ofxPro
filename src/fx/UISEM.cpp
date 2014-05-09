//
//  UISEM.cpp
//
//  Created by Patricio Gonzalez Vivo on 5/7/14.
//
//

#include "UISEM.h"
#define STRINGIFY(A) #A

UISEM::UISEM(){
    {
        string  vertexShader = "#version 120\n";
        vertexShader += STRINGIFY(
                                  varying vec3 e;
                                  varying vec3 n;
                                  
                                  void main() {
                                      e = normalize( vec3( gl_ModelViewMatrix * vec4( gl_Vertex.xyz, 1.0 ) ) );
                                      n = normalize( gl_NormalMatrix * gl_Normal );
                                      gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
                                  }
                                  );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        
        string fragmentShader = "#version 120\n";
        fragmentShader += STRINGIFY(
                                    uniform sampler2D tMatCap;
                                    
                                    varying vec3 e;
                                    varying vec3 n;
                                    
                                    void main() {
                                        vec3 r = reflect( e, n );
                                        //r = e - 2. * dot( n, e ) * n;
                                        float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                                        vec2 vN = r.xy / m + .5;
                                        
                                        vec3 base = texture2D( tMatCap, vN ).rgb;
                                        
                                        gl_FragColor = vec4( base, 1. );
                                    }
                                    );
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    }

    shader.linkProgram();
    
    textures = NULL;
}

void UISEM::setupUI(){
    gui->addBaseDraws("Texture", &texture,175,175);

    vector<string> list;
    string path = "GUI/materials";
    ofDirectory dir(path);
    if(dir.exists()){
		dir.listDir();
        int total = dir.getFiles().size();
        for (int i = 0; i < total; i++) {
            list.push_back( dir.getName(i) );
        }
        
        if(textures == NULL){
            textures = gui->addRadio("Materials", list);
        }
    }
}

void UISEM::begin(){
    if (texture.isAllocated()&&bEnable){
        shader.begin();
        shader.setUniformTexture("tMatCap", texture, 1);
    }
}

void UISEM::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (name.find('.') != string::npos){
        ofxUIToggle *t = (ofxUIToggle*)e.widget;
        if (t != NULL){
            if (t->getValue()){
                loadTexture("GUI/materials/"+name);
            }
        }
    }
}

void UISEM::loadTexture(string _path){
    ofDisableArbTex();
    ofLoadImage(texture, _path);
    ofEnableArbTex();
}

ofTexture& UISEM::getTexture(){
    return texture;
}

void UISEM::end(){
    if (texture.isAllocated()&&bEnable) {
        shader.end();
    }
}