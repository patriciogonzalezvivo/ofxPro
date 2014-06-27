//
//  UILUT.cpp
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#include "UILUT.h"
#define STRINGIFY(A) #A

UILUT::UILUT(){
#ifdef TARGET_OPENGLES
    //  OpenGL ES
    vertexShader = STRINGIFY(
                                     attribute vec4 position;
                                     uniform mat4 modelViewMatrix;
                                     uniform mat4 projectionMatrix;
                                     
                                     void main(){
                                         vec4 pos = projectionMatrix * modelViewMatrix * position;
                                         gl_Position = pos;
                                     }
                                     );
    
    setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    
    fragmentShader = STRINGIFY(precision highp float;
                                      uniform sampler2D tex0;
                                      uniform sampler2D tex1;
                               
                                      uniform vec2 resolution;
                                      uniform float size;
                               
                                      vec4 cheapLUT(sampler2D tex, vec3 texCoord){
                                          vec2 pos = vec2(0.0,0.0);
                                          pos.x = texCoord.r/size+(1.0/(size*size))*0.5;
                                          pos.x += floor(texCoord.g*size)/size;
                                          pos.y = texCoord.b;
                                          return texture2D(tex1, pos);
                                      }
                                      vec4 sampleAs3DTexture(sampler2D tex, vec3 texCoord){
                                          float sliceSize = 1.0/size;
                                          float slicePixelSize = sliceSize/size;
                                          float sliceInnerSize = slicePixelSize*(size-1.0);
                                          float zSlice0 = min(floor(texCoord.z*size),size-1.0);
                                          float zSlice1 = min(zSlice0+1.0,size-1.0);
                                          float xOffset = slicePixelSize*0.5+texCoord.x*sliceInnerSize;
                                          float s0 = xOffset+(zSlice0*sliceSize);
                                          float s1 = xOffset+(zSlice1*sliceSize);
                                          vec4 slice0Color = texture2D(tex,vec2(s0,texCoord.y));
                                          vec4 slice1Color = texture2D(tex,vec2(s1,texCoord.y));
                                          float zOffset = mod(texCoord.z*size,1.0);
                                          return mix(slice0Color,slice1Color,zOffset);
                                      }
                                      
                                      void main(){
                                          vec2 st = gl_FragCoord.xy / resolution.xy;
                                          st.y = 1.0 - st.y;
                                          vec4 srcColor = texture2D(tex0, st);
                                          gl_FragColor = sampleAs3DTexture(tex1,srcColor.rgb);
//                                          gl_FragColor = cheapLUT(tex1,srcColor.rgb);
                                          gl_FragColor.a = srcColor.a;
                                      }
                                      );
    setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
#else
    if( ofIsGLProgrammableRenderer() ){
        
        //  OpenGL 3.0
        vertexShader = "#version 150\n";
        vertexShader += STRINGIFY(
                                  uniform mat4 modelViewProjectionMatrix;
                                  uniform mat4 textureMatrix;
                                  
                                  in vec4 position;
                                  in vec2 texcoord;

                                  out vec2 texCoordVarying;
                                  
                                  void main(){
                                      texCoordVarying = texcoord;
                                      gl_Position = modelViewProjectionMatrix * position;
                                  });
        
        setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        
        fragmentShader = "#version 150\n";
        fragmentShader += STRINGIFY(
                                    uniform sampler2DRect tex0;
                                    uniform sampler2DRect tex1;
                                    uniform float size;
                                    
                                    in vec2 texCoordVarying;
                                    out vec4 outputColor;
                                    
                                    vec4 cheapLUT(sampler2D tex, vec3 texCoord){
                                        vec2 pos = vec2(0.0,0.0);
                                        pos.x = texCoord.r/size+(1.0/(size*size))*0.5;
                                        pos.x += floor(texCoord.g*size)/size;
                                        pos.y = texCoord.b;
                                        return texture(tex1, pos);
                                    }
                                    
                                    vec4 sampleAs3DTexture(sampler2D tex, vec3 texCoord){
                                        float sliceSize = 1.0/size;
                                        float slicePixelSize = sliceSize/size;
                                        float sliceInnerSize = slicePixelSize*(size-1.0);
                                        float zSlice0 = min(floor(texCoord.z*size),size-1.0);
                                        float zSlice1 = min(zSlice0+1.0,size-1.0);
                                        float xOffset = slicePixelSize*0.5+texCoord.x*sliceInnerSize;
                                        float s0 = xOffset+(zSlice0*sliceSize);
                                        float s1 = xOffset+(zSlice1*sliceSize);
                                        vec4 slice0Color = texture(tex,vec2(s0,texCoord.y));
                                        vec4 slice1Color = texture(tex,vec2(s1,texCoord.y));
                                        float zOffset = mod(texCoord.z*size,1.0);
                                        return mix(slice0Color,slice1Color,zOffset);
                                    }
                                    
                                    void main(){
                                        vec4 srcColor = texture(tex0, texCoordVarying);
                                        // outputColor = sampleAs3DTexture(tex1,srcColor.rgb);
                                        outputColor = cheapLUT(tex1,srcColor.rgb);
                                        outputColor.a = srcColor.a;
                                    }
                                    );
        setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        
    } else {
        //  OpenGL 2.0
        
        vertexShader = "#version 120\n";
        vertexShader += STRINGIFY(
                                  void main(){
                                      // gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                                      gl_TexCoord[0] = gl_MultiTexCoord0;
                                      gl_Position = ftransform();
                                  }
                                  );
        
        setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        
        fragmentShader = "#version 120\n";
        fragmentShader += STRINGIFY(
                                    uniform sampler2DRect tex0;
                                    uniform sampler2DRect tex1;
                                    
                                    uniform float size;
                                    
                                    void main(void){
                                        vec2 st = gl_TexCoord[0].st;
                                        vec4 srcColor = texture2DRect(tex0, st);
                                        
                                        float x = srcColor.r * (size-1.0);
                                        float y = srcColor.g * (size-1.0);
                                        float z = srcColor.b * (size-1.0);
                                        
                                        vec3 color = texture2DRect(tex1, vec2(floor(x)+0.5 +(floor(y)*size+0.5),
                                                                              floor(z)+0.5) ).rgb;
                                        
                                        gl_FragColor = vec4(color,srcColor.a);
                                    });
        setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    }
#endif
    linkProgram();
    
    cubes = NULL;
}

void UILUT::setupUI(){
    vector<string> cubeList;
    string path = "GUI/LUTs";
    ofDirectory cubesDir(path);
    if(cubesDir.exists()){
		cubesDir.listDir();
        int total = cubesDir.getFiles().size();
        for (int i = 0; i < total; i++) {
            cubeList.push_back( cubesDir.getName(i) );
        }
        
        if(cubes == NULL){
            cubes = gui->addRadio("LUTs", cubeList);
        }
    }
}

void UILUT::begin(){
    if (cube.isAllocated()&&bEnable){
        UIShader::begin();
        setUniformTexture("tex1", cube, 1);
        setUniform1f("size", (float)cube.getHeight());
#ifdef TARGET_OPENGLES
        setUniform2f("resolution", (float)width, (float)height);
#endif
    }
}

void UILUT::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (name.find('.') != string::npos){
        ofxUIToggle *t = (ofxUIToggle*)e.widget;
        if (t != NULL){
            if (t->getValue()){
                loadCube("GUI/LUTs/"+name);
            }
        }
    }
}

bool UILUT::load(string _path){
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

bool UILUT::loadCube(string _path){
    return ofLoadImage(cube, _path);
}

ofTexture& UILUT::getCube(){
    return cube;
}

void UILUT::end(){
    if (cube.isAllocated()&&bEnable) {
        UIShader::end();
    }
}

