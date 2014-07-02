//
//  UIMatCap.cpp
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#include "UIMatCap.h"

UIMatCap::UIMatCap(){
    
    vertexShader = "#version 120\n\n";
    vertexShader += "uniform float time;\n";
    vertexShader += "uniform vec2 _repeat;\n";
    vertexShader += "uniform float _useNormal;\n";
    vertexShader += "uniform float useRim;\n\n";
    
    vertexShader += "varying vec4 vPos;\n";
    vertexShader += "varying vec2 vTexCoord;\n";
    vertexShader += "varying vec2 vUv;\n";
    vertexShader += "varying vec3 vNormal;\n";
    vertexShader += "varying vec3 vEye;\n";
    vertexShader += "varying vec3 vU;\n";
    vertexShader += "varying vec2 vN;\n\n";
    
    vertexShader += "void main() {\n\n";
        
    vertexShader += "\tvU = normalize( vec3( gl_ModelViewMatrix * vec4( gl_Vertex.xyz, 1.0 ) ) );\n\n";
        
    vertexShader += "\tif( _useNormal == 0. ) {\n";
    vertexShader += "\t\tvec3 n = normalize( gl_NormalMatrix * gl_Normal );\n";
    vertexShader += "\t\tvec3 r = reflect( vU, n );\n";
    vertexShader += "\t\tfloat m = 2.0 * sqrt( r.x * r.x + r.y * r.y + ( r.z + 1.0 ) * ( r.z+1.0 ) );\n";
    vertexShader += "\t\tvN = vec2( r.x / m + 0.5,  r.y / m + 0.5 );\n";
    vertexShader += "\t} else {\n";
    vertexShader += "\t\tvN = vec2( 0. );\n";
    vertexShader += "\t}\n\n";
    
    vertexShader += "\tvTexCoord = gl_MultiTexCoord0.xy;\n";
    vertexShader += "\tvUv = _repeat * vTexCoord;\n\n";
        
    vertexShader += "\tvPos = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4( gl_Vertex.xyz, 1.0 );\n";
    vertexShader += "\tgl_Position = vPos;\n\n";
        
    vertexShader += "\tvNormal = normalize(gl_NormalMatrix * gl_Normal);\n\n";
        
    vertexShader += "\tif( useRim > 0. ) {\n";
    vertexShader += "\t\tvEye = ( gl_ModelViewMatrix * vec4( gl_Vertex.xyz, 1.0 ) ).xyz;\n";
    vertexShader += "\t} else {\n";
    vertexShader += "\t\tvEye = vec3( 0. );\n";
    vertexShader += "\t}\n";
    vertexShader += "}\n";
    
    geometryShader = "#version 120\n";
    geometryShader += "#extension GL_EXT_geometry_shader4 : enable\n";
    geometryShader += "#extension GL_EXT_gpu_shader4 : enable\n\n";
    
    geometryShader += "uniform float _useNormal;\n\n";
    
    geometryShader += "varying in vec4 vPos[3];\n";
    geometryShader += "varying in vec2 vTexCoord[3];\n";
    geometryShader += "varying in vec2 vUv[3];\n";
    geometryShader += "varying in vec3 vNormal[3];\n";
    geometryShader += "varying in vec3 vEye[3];\n";
    geometryShader += "varying in vec3 vU[3];\n";
    geometryShader += "varying in vec2 vN[3];\n\n";
    
    geometryShader += "varying out vec2 Uv;\n";
    geometryShader += "varying out vec3 Tangent;\n";
    geometryShader += "varying out vec3 Binormal;\n";
    geometryShader += "varying out vec3 Normal;\n";
    geometryShader += "varying out vec3 Eye;\n";
    geometryShader += "varying out vec3 U;\n";
    geometryShader += "varying out vec2 N;\n\n";
    
    geometryShader += "void main(void){\n\n";
        
    geometryShader += "\tvec3 sdir = vec3(1.0);\n";
    geometryShader += "\tvec3 tdir = vec3(1.0);\n\n";
        
    geometryShader += "\tif( _useNormal == 1. ) {\n";
    geometryShader += "\t\tvec3 v1 = vPos[0].xyz;\n";
    geometryShader += "\t\tvec3 v2 = vPos[1].xyz;\n";
    geometryShader += "\t\tvec3 v3 = vPos[2].xyz;\n\n";
            
    geometryShader += "\t\tvec2 w1 = vTexCoord[0];\n";
    geometryShader += "\t\tvec2 w2 = vTexCoord[1];\n";
    geometryShader += "\t\tvec2 w3 = vTexCoord[2];\n\n";
            
    geometryShader += "\t\tfloat x1 = v2.x - v1.x;\n";
    geometryShader += "\t\tfloat x2 = v3.x - v1.x;\n";
    geometryShader += "\t\tfloat y1 = v2.y - v1.y;\n";
    geometryShader += "\t\tfloat y2 = v3.y - v1.y;\n";
    geometryShader += "\t\tfloat z1 = v2.z - v1.z;\n";
    geometryShader += "\t\tfloat z2 = v3.z - v1.z;\n\n";
            
    geometryShader += "\t\tfloat s1 = w2.x - w1.x;\n";
    geometryShader += "\t\tfloat s2 = w3.x - w1.x;\n";
    geometryShader += "\t\tfloat t1 = w2.y - w1.y;\n";
    geometryShader += "\t\tfloat t2 = w3.y - w1.y;\n\n";
            
    geometryShader += "\t\tfloat r = 1.0F / (s1 * t2 - s2 * t1);\n";
    geometryShader += "\t\tsdir = vec3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);\n";
    geometryShader += "\t\ttdir = vec3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);\n";
    geometryShader += "\t}\n\n";
        
    geometryShader += "\tfor(int i = 0; i < 3; i++){\n";
    geometryShader += "\t\tUv = vUv[i];\n\n";
            
    geometryShader += "\t\tNormal = vNormal[i];\n";
    geometryShader += "\t\tif( _useNormal == 1. ) {\n";
    geometryShader += "\t\t\tvec3 t = normalize(sdir-vNormal[i]*dot(vNormal[i],sdir));\n";
    geometryShader += "\t\t\tfloat w = (dot(cross(vNormal[i],sdir),tdir)<0.0)? -1.0:1.0;\n\n";
                
    geometryShader += "\t\t\tTangent = normalize( gl_NormalMatrix * t);\n";
    geometryShader += "\t\t\tBinormal = normalize( cross( Normal, Tangent ) * w );\n";
    geometryShader += "\t\t}\n\n";
            
    geometryShader += "\t\tEye = vEye[i];\n";
    geometryShader += "\t\tU = vU[i];\n";
    geometryShader += "\t\tN = vN[i];\n";
    geometryShader += "\t\tgl_TexCoord[0].xy = vTexCoord[i];\n";
    geometryShader += "\t\tgl_Position = vPos[i];\n";
    geometryShader += "\t\tEmitVertex();\n";
    geometryShader += "\t}\n\n;";
        
    geometryShader += "\tEndPrimitive();\n";
    geometryShader += "}\n";
    
    fragmentShader = "#version 120\n\n";
    fragmentShader += "uniform sampler2D tMatCap;\n";
    fragmentShader += "uniform sampler2D tNormal;\n\n";
    
    fragmentShader += "uniform vec2 _repeat;\n\n";
    
    fragmentShader += "uniform float _useNormal;\n";
    fragmentShader += "uniform float useRim;\n";
    fragmentShader += "uniform float time;\n";
    fragmentShader += "uniform float normalScale;\n";
    fragmentShader += "uniform float normalRepeat;\n\n";
    
    fragmentShader += "varying vec2 Uv;\n";
    fragmentShader += "varying vec3 Tangent;\n";
    fragmentShader += "varying vec3 Binormal;\n";
    fragmentShader += "varying vec3 Normal;\n";
    fragmentShader += "varying vec3 Eye;\n";
    fragmentShader += "varying vec3 U;\n";
    fragmentShader += "varying vec2 N;\n\n";
    
    fragmentShader += "void main() {\n";
    fragmentShader += "\tvec3 finalNormal = Normal;\n";
    fragmentShader += "\tvec2 calculatedNormal = N;\n\n";
        
    fragmentShader += "\tif( _useNormal == 1. ) {\n";
    fragmentShader += "\t\tvec3 normalTex = texture2D( tNormal, Uv * normalRepeat*100.0 ).xyz * 2.0 - 1.0;\n";
    fragmentShader += "\t\tnormalTex.xy *= normalScale*0.2;\n";
    fragmentShader += "\t\tnormalTex.y *= -1.;\n";
    fragmentShader += "\t\tnormalTex = normalize( normalTex );\n";
    fragmentShader += "\t\tmat3 tsb = mat3( normalize( Tangent ), normalize( Binormal ), normalize( Normal ) );\n";
    fragmentShader += "\t\tfinalNormal = tsb * normalTex;\n\n";
            
    fragmentShader += "\t\tvec3 r = reflect( U, normalize( finalNormal ) );\n";
    fragmentShader += "\t\tfloat m = 2.0 * sqrt( r.x * r.x + r.y * r.y + ( r.z + 1.0 ) * ( r.z+1.0 ) );\n";
    fragmentShader += "\t\tcalculatedNormal = vec2( r.x / m + 0.5,  r.y / m + 0.5 );\n";
    fragmentShader += "\t}\n\n";
        
    fragmentShader += "\tcalculatedNormal.y = 1.0-calculatedNormal.y;\n\n";
        
    fragmentShader += "\tvec3 base = texture2D( tMatCap, calculatedNormal ).rgb;\n\n";
        
    fragmentShader += "\tif( useRim > 0. ) {\n";
    fragmentShader += "\t\tfloat f = abs( dot( Normal, normalize( Eye ) ) );\n";
    fragmentShader += "\t\tf = useRim * ( 1. - smoothstep( 0.0, 1., f ) );\n";
    fragmentShader += "\t\tbase += vec3( f );\n";
    fragmentShader += "\t}\n\n";
        
    fragmentShader += "\tgl_FragColor = vec4( base, 1. );\n";
    fragmentShader += "}\n";
    
    setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    setupShaderFromSource(GL_GEOMETRY_SHADER, geometryShader);

    geomInType = GL_TRIANGLES;
    geomOutType = GL_TRIANGLES;
    geomOutCount = 3;
    
    setGeometryInputType(geomInType);
    setGeometryOutputType(geomOutType);
    setGeometryOutputCount(geomOutCount);
    
    linkProgram();
    
    extractUniforms(fragmentShader);
}

string UIMatCap::getClassName(){
    if(fragFile.isFile()){
        return fragFile.getBaseName();
    } else {
        return "MatCap";
    }
}

void UIMatCap::setupUI(){
    
    int w = gui->getGlobalButtonDimension();
    
    
    {
        gui->addLabel("Materials");
        gui->setGlobalButtonDimension(34);
        vector<string> list;
        string path = "GUI/materials";
        ofDirectory dir(path);
        int count = 0;
        if(dir.exists()){
            dir.listDir();
            int total = dir.getFiles().size();
            for (int i = 0; i < total; i++) {
                list.push_back( dir.getName(i) );
                gui->addImageToggle("materials/"+dir.getName(i), "GUI/materials/"+dir.getName(i),false);
                gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                count++;
                if(count == 5){
                    count = 0;
                    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                }
            }
        }
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        gui->addSpacer();
    }
    
    gui->setGlobalButtonDimension(w);
    {
        gui->addLabel("Normals");
        
        ofxUIToggle *toggle = gui->addToggle("'", &bNormals);
        gui->addWidgetUp(toggle, OFX_UI_ALIGN_RIGHT, true);
        
        gui->setGlobalButtonDimension(34);
        vector<string> list;
        string path = "GUI/normals";
        ofDirectory dir(path);
        int count = 0;
        if(dir.exists()){
            dir.listDir();
            int total = dir.getFiles().size();
            for (int i = 0; i < total; i++) {
                list.push_back( dir.getName(i) );
                gui->addImageToggle("normals/"+dir.getName(i), "GUI/normals/"+dir.getName(i), false);
                gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                count++;
                if(count == 5){
                    count = 0;
                    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                }
            }
        }
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        gui->addSpacer();
    }
    gui->setGlobalButtonDimension(w);
    UIShader::setupUI();
}

void UIMatCap::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (name.find('.') != string::npos){
        
        ofxUIImageToggle *toggle = (ofxUIImageToggle*)e.widget;

        if(toggle->getValue() == true){
            if(name.find("materials") == 0){
                loadMaterial("GUI/"+name);
                
                vector<ofxUIWidget*> w = gui->getWidgets();
                for (int i = 0; i < w.size(); i++) {
                    string n = w[i]->getName();
                    
                    if( w[i]->getKind() == 20 &&
                       n.find('.') != string::npos &&
                       n.find("materials") == 0 &&
                       n != name){
                        
                        ofxUIImageToggle* t = (ofxUIImageToggle*)w[i];
                        t->setValue(false);
                    }
                }
                
            } else if (name.find("normals") == 0){
                loadNormal("GUI/"+name);
                
                vector<ofxUIWidget*> w = gui->getWidgets();
                for (int i = 0; i < w.size(); i++) {
                    string n = w[i]->getName();
                    
                    if( w[i]->getKind() == 20 &&
                       n.find('.') != string::npos &&
                       n.find("normals") == 0 &&
                       n != name){
                    
                        ofxUIImageToggle* t = (ofxUIImageToggle*)w[i];
                        t->setDrawFill(true);
                        t->setValue(false);
                    }
                }
            }
        }
    }
    
    UIShader::guiEvent(e);
}

void UIMatCap::loadMaterial(string _path){
    ofDisableArbTex();
    ofLoadImage(matCap, _path);
    ofEnableArbTex();
}

void UIMatCap::loadNormal(string _path){
    ofDisableArbTex();
    ofLoadImage(normal, _path);
    normal.setTextureWrap(GL_REPEAT, GL_REPEAT);
    ofEnableArbTex();
}

void UIMatCap::begin(){
    if (matCap.isAllocated()&&bEnable){
        UIShader::begin();
        setUniformTexture("tMatCap", matCap, 0);
        
        if(normal.isAllocated()){
            setUniformTexture("tNormal", normal, 1);
            setUniform1f("_useNormal", bNormals);
            setUniform2f("_repeat", 1, 1);
        } else {
            setUniform1f("_useNormal", 0);
        }
    }
}

void UIMatCap::end(){
    if (matCap.isAllocated()&&bEnable) {
        UIShader::end();
    }
}