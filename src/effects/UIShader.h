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

enum UniformType{
	UNIFORM_SAMPLE2DRECT = 0,
    UNIFORM_VEC2,
    UNIFORM_FLOAT
};

class UIShader : public UIClass {
public:
    
    string getClassName(){return "SHADER"; }
    void setupUI();
    
    void loadFrag(string _fragShader );
    
    void begin();
    void end();
    
protected:
    void guiEvent(ofxUIEventArgs &e);
    void checkShaderFile();
    bool reloadShader(string _filePath = "none");
    
    void addUniform(UniformType _type, string _name);
    void reloadUniforms(string &_text);
    
    std::time_t getLastModified( ofFile& _file );
    
	ofShader            shader;
    ofFile              shaderFile;
    string              shaderFilename;
    std::time_t         shaderChangedTimes;
    int                 lastTimeCheckMillis;
    int                 millisBetweenFileCheck;
    
//    vector<TextBox*>        uniforms;
    
    int                 width, height, nTex;
};
