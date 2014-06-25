//
//  UISEM.cpp
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#include "UISEM.h"
#define STRINGIFY(A) #A

UISEM::UISEM(){
    
    vertexShader = "#version 120\n\n";
    vertexShader += "varying vec3 e;\n";
    vertexShader += "varying vec3 n;\n\n";
    vertexShader += "void main() {\n";
    vertexShader += "\te = normalize( vec3( gl_ModelViewMatrix * vec4( gl_Vertex.xyz, 1.0 ) ) );\n";
    vertexShader += "\tn = normalize( gl_NormalMatrix * gl_Normal );\n";
    vertexShader += "\tgl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;\n";
    vertexShader += "}\n";
    
    fragmentShader = "#version 120\n\n";
    fragmentShader += "uniform sampler2D tMatCap;\n";
    fragmentShader += "varying vec3 e;\n";
    fragmentShader += "varying vec3 n;\n\n";
    fragmentShader += "void main() {\n";
    fragmentShader += "\tvec3 r = reflect( e, n );\n";
    //r = e - 2. * dot( n, e ) * n;
    fragmentShader += "\tfloat m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );\n";
    fragmentShader += "\tvec2 vN = r.xy / m + .5;\n";
    fragmentShader += "\tvec3 base = texture2D( tMatCap, vN ).rgb;\n";
    fragmentShader += "\tgl_FragColor = vec4( base, 1. );\n";
    fragmentShader += "}\n";
    
    setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);


    linkProgram();
    textures = NULL;
}

bool UISEM::load(string _path){
    ofFile fragFile = ofFile(_path+".frag");
    ofFile vertFile = ofFile(_path+".vert");
    
    if( !vertFile.exists()){
        ofBuffer vert;
        vert.append(vertexShader);
        ofBufferToFile(_path+".vert", vert);
    }
    
    if (!fragFile.exists() ){
        ofBuffer frag;
        frag.append(fragmentShader);
        ofBufferToFile(_path+".frag", frag);
    }
    
    return UIShader::load(_path);
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
    
    gui->addSpacer();
    
    UIShader::setupUI();
}

void UISEM::begin(){
    if (texture.isAllocated()&&bEnable){
        UIShader::begin();
        setUniformTexture("tMatCap", texture, 1);
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
    
    UIShader::guiEvent(e);
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
        UIShader::end();
    }
}