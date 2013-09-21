//
//  UIShader.h
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/15/13.
//
//

#pragma once

#include "UIClass.h"

#include "PingPong.h"
#include "Uniform.h"


class UIShader : public UIClass {
public:
    UIShader();
    
    string getClassName(){return "SHADER"; }
    void setupUI();
    
    void loadFrag(string _fragShader );
    void setTexture(ofBaseDraws& tex, int _texNum = 0);
    
    void begin();
    void end();
    
protected:
    void guiEvent(ofxUIEventArgs &e);
    void checkShaderFile();
    bool reloadShader(string _filePath = "none");
    void addUniform(UniformType _type, string _name);
    
    std::time_t getLastModified( ofFile& _file );
    
	ofShader            shader;
    ofFile              shaderFile;
    string              shaderFilename;
    std::time_t         shaderChangedTimes;
    int                 lastTimeCheckMillis;
    int                 millisBetweenFileCheck;
    
    vector<Uniform*>    uniforms;
    ofFbo               *textures;
    
    int                 nTextures;
    int                 textureCounter;
};
