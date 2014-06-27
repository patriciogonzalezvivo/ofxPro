//
//  UIShader.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#pragma once

#include "UIClass.h"

#include "PingPong.h"
#include "Uniform.h"


class UIShader : public UIClass , public ofShader {
public:
    UIShader();
    
    virtual string  getClassName();
    
    virtual bool    load(string _name);
    virtual bool    load(string _fragShader, string _vectShader, string _geomShader = "");
    virtual bool    loadFrag(string _fragShader);
    
    virtual void    setGeometryShaderValues(GLenum _in, GLenum _out, int _numOut);
    
    virtual void    begin();
    virtual void    end();
    
protected:
    void    setupUI();
    void    guiEvent(ofxUIEventArgs &e);
    
    void    checkShaderFile();
    bool    reloadShader(string _fragPath, string _vertPath = "", string _geomPath = "");
    void    extractUniforms(string _shaderCode);
    void    addUniform(UniformType _type, string _name);
    
    std::time_t getLastModified( ofFile& _file );
    
	//  Fragment Shader
    //
    ofFile              fragFile;
    string              fragFilename;
    string              fragmentShader;
    std::time_t         fragChangedTimes;
    
    //  Vertex Shader
    ofFile              vertFile;
    string              vertFilename;
    string              vertexShader;
    std::time_t         vertChangedTimes;
    bool                bVertex;
    
    //  Geom Shader
    ofFile              geomFile;
    string              geomFilename;
    string              geometryShader;
    std::time_t         geomChangedTimes;
    GLenum              geomInType;
    GLenum              geomOutType;
    int                 geomOutCount;
    bool                bGeometry;
    
    int                 lastTimeCheckMillis;
    int                 millisBetweenFileCheck;
    
    vector<Uniform*>    uniforms;
    bool                bOpen;
};
