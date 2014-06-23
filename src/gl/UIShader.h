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


class UIShader : public UIClass {
public:
    UIShader();
    
    string  getClassName();
    
    void    load(string _name);
    void    load(string _fragShader, string _vectShader, string _geomShader = "");
    void    loadFrag(string _fragShader);
    
    void    setGeometryShaderValues(GLenum _in, GLenum _out, int _numOut);
    ofShader& getShader();
    
    void begin();
    void end();
    
protected:
    void setupUI();
    void guiEvent(ofxUIEventArgs &e);
    
    void checkShaderFile();
    bool reloadShader(string _fragPath, string _vertPath = "", string _geomPath = "");
    void addUniform(UniformType _type, string _name);
    
    std::time_t getLastModified( ofFile& _file );
    
	//  Fragment Shader
    //
    ofFile              fragFile;
    string              fragFilename;
    std::time_t         fragChangedTimes;
    
    //  Vertex Shader
    ofFile              vertFile;
    string              vertFilename;
    std::time_t         vertChangedTimes;
    bool                bVertex;
    
    //  Geom Shader
    ofFile              geomFile;
    string              geomFilename;
    std::time_t         geomChangedTimes;
    GLenum              geomInType;
    GLenum              geomOutType;
    int                 geomOutCount;
    bool                bGeometry;
    
    ofShader            shader;
    
    int                 lastTimeCheckMillis;
    int                 millisBetweenFileCheck;
    
    vector<Uniform*>    uniforms;
    bool                bOpen;
};
