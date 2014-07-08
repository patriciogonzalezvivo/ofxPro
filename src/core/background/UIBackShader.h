//
//  UIBackShader.h
//  00-CLOUDS
//
//  Created by Patricio Gonzalez Vivo on 7/8/14.
//
//

#pragma once

#include "UIBackground.h"

#include "Uniform.h"

class UIBackShader : public UIBackground {
public:
    
    UIBackShader();
    UIBackShader(string _name);
    virtual ~UIBackShader(){};
    
    virtual bool load(string _name);
    virtual string  getClassName();
    
    virtual void draw();
    
protected:
    virtual void setupUI();
    virtual void guiEvent(ofxUIEventArgs &e);
    
	virtual void update(ofEventArgs & args);
    virtual void windowResized(ofResizeEventArgs & args);
    
    void    checkShaderFile();
    bool    reloadShader(string _fragPath);
    void    extractUniforms(string _shaderCode);
    void    addUniform(UniformType _type, string _name);
    
    std::time_t getLastModified( ofFile& _file );
    int     lastTimeCheckMillis;
    int     millisBetweenFileCheck;
    
    //  Mesh
    //
    ofMesh  mesh;
    bool    bMeshReady;
    
    //  FragShader
    //
    ofFile      fragFile;
    string      fragFilename;
    std::time_t fragChangedTimes;
    
    string      fragmentShader;
    
    ofShader    shader;
    
    vector<Uniform*>    uniforms;
    bool                bOpen;
};