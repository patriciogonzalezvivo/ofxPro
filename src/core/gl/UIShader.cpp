//
//  UIShader.cpp
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#include "UIShader.h"

void jumpNextWord(int &_index, vector<string> &_words){
    _index++;
    
    while( ((_words[ _index].size() <= 1) || (_words[ _index] == "\t")) && ( _index < _words.size())){
        _index++;
    }
}

void eraseSemiColon(string &_word){
    int end = _word.find(';') ;
    for(int i = _word.size()-1; i >= end ; i--){
        _word.erase(_word.begin()+i);
    }
}

UIShader::UIShader(){
    fragFilename = "";
    vertFilename = "";
    geomFilename = "";
    
    vertexShader = "#version 120\n\n";
    
    vertexShader += "varying vec4 ambientGlobal, eyeSpaceVertexPos;\n";
    vertexShader += "varying vec4 vertexPos;\n";
    vertexShader += "varying vec3 vertexNormal;\n\n";
    
    vertexShader += "void main() {\n";
    vertexShader += "\tgl_TexCoord[0] = gl_MultiTexCoord0;\n";
    vertexShader += "\tvertexPos = gl_Vertex;\n";
	vertexShader += "\tvertexNormal = normalize(gl_NormalMatrix * gl_Normal);\n";
	vertexShader += "\teyeSpaceVertexPos = gl_ModelViewMatrix * vertexPos;\n";
    vertexShader += "\tambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient + gl_FrontMaterial.emission;\n";
    vertexShader += "\tgl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;\n";
    vertexShader += "}\n";
    
    fragmentShader = "#version 120\n\n";
    fragmentShader += "uniform sampler2DRect tex;\n\n";
    
    fragmentShader += "varying vec4 ambientGlobal, eyeSpaceVertexPos;\n";
    fragmentShader += "varying vec4 vertexPos;\n";
    fragmentShader += "varying vec3 vertexNormal;\n\n";
    
    fragmentShader += "void main() {\n";
    fragmentShader += "\tvec2 st = gl_TexCoord[0].st;\n";
	fragmentShader += "\tvec4 color = texture2DRect( tex, st );\n";
	fragmentShader += "\tgl_FragColor = gl_Color;\n";
    fragmentShader += "}\n";
    
    geometryShader = "";
    
    geomInType = GL_POINTS;
    geomOutType = GL_TRIANGLES;
    geomOutCount = 6;
    
    bVertex = false;
    bGeometry = false;
}

void UIShader::setupUI(){
    float length = (gui->getGlobalCanvasWidth()-gui->getWidgetSpacing()*5);
    float dim = gui->getGlobalSliderHeight();
    
    for(int i = 0; i < uniforms.size(); i++){
        if ( uniforms[i]->bNeedUI ){
            if (uniforms[i]->type == UNIFORM_FLOAT){
                gui->addSlider( uniforms[i]->name, 0.0, 1.0, &uniforms[i]->value.x );
            } else if (uniforms[i]->type == UNIFORM_VEC2){
                gui->addLabel( uniforms[i]->name, OFX_UI_FONT_SMALL);
                gui->addMinimalSlider("x", 0.0, 1.0, &uniforms[i]->value.x,length/2.0, dim);
                gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                gui->addMinimalSlider("y", 0.0, 1.0, &uniforms[i]->value.y, length/2.0, dim);
                gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                gui->addSpacer();
            } else if (uniforms[i]->type == UNIFORM_VEC3){
                gui->addLabel( uniforms[i]->name, OFX_UI_FONT_SMALL);
                gui->addMinimalSlider("x", 0.0, 1.0, &uniforms[i]->value.x,length/3.0, dim);
                gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                gui->addMinimalSlider("y", 0.0, 1.0, &uniforms[i]->value.y, length/3.0, dim);
                gui->addMinimalSlider("z", 0.0, 1.0, &uniforms[i]->value.z, length/3.0, dim);
                gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                gui->addSpacer();
            }
        }
    }
    
    if(fragFilename != ""){
        gui->addButton("OPEN",&bOpen);
    }
}

bool UIShader::load(string _name){
    ofFile fragFile = ofFile(_name+".frag");
    ofFile vertFile = ofFile(_name+".vert");
    ofFile geomFile = ofFile(_name+".geom");
    
    if( !vertFile.exists()){
        ofBuffer vert;
        vert.append(vertexShader);
        ofBufferToFile(_name+".vert", vert);
    }
    
    if (!fragFile.exists() ){
        ofBuffer frag;
        frag.append(fragmentShader);
        ofBufferToFile(_name+".frag", frag);
    }
    
    if(geomFile.exists()){
        return load(_name+".frag",_name+".vert",_name+".geom");
    } else if (geometryShader.size() > 2){
        ofBuffer geom;
        geom.append(geometryShader);
        ofBufferToFile(_name+".geom", geom);
        
        return load(_name+".frag",_name+".vert",_name+".geom");
    } else {
        return load(_name+".frag",_name+".vert");
    }
}

bool UIShader::load(string _fragShader, string _vertShader, string _geomShader){
    return reloadShader(_fragShader,_vertShader, _geomShader);
}

bool UIShader::loadFrag(string _fragShader){
    bVertex = false;
    
    ofFile fragFile = ofFile(_fragShader);
    if (!fragFile.exists() ){
        ofBuffer frag;
        frag.append(fragmentShader);
        ofBufferToFile(_fragShader, frag);
    }
    
    return reloadShader( _fragShader );
}

void UIShader::setGeometryShaderValues(GLenum _in, GLenum _out, int _numOut){
    geomInType = _in;
    geomOutType = _out;
    geomOutCount = _numOut;
}

string UIShader::getClassName(){
    return fragFile.getBaseName();
}

void UIShader::guiEvent(ofxUIEventArgs &e){
    if (gui != NULL){
        string name = e.widget->getName();
        
        if( name == "OPEN"){
            
            string comand = "open " + fragFilename;

            if (bVertex){
                comand += " " + vertFilename;
            }
            
            if(bGeometry){
                comand += " " + geomFilename;
            }
            
            system(comand.c_str());
            
        }
    }
}

bool UIShader::reloadShader(string _fragPath, string _vertPath, string _geomPath){
    ofShader::unload();
	
	// hackety hack, clear errors or shader will fail to compile
	GLuint err = glGetError();
	
    fragFile.clear();
    fragFilename = ofToDataPath( _fragPath );
    fragFile = ofFile( fragFilename );
    fragChangedTimes = getLastModified( fragFile );
    ofBuffer fragBuffer = ofBufferFromFile( fragFilename );
    if( fragBuffer.size() > 0 ){
        setupShaderFromSource(GL_FRAGMENT_SHADER, fragBuffer.getText());
    }
    
    
    if (_vertPath != ""){
        vertFile.clear();
        vertFilename = ofToDataPath(_vertPath);
        vertFile = ofFile( vertFilename );
        vertChangedTimes = getLastModified( vertFile );
        ofBuffer vertBuffer = ofBufferFromFile( vertFilename );
        
        if( vertBuffer.size() > 0 ){
            setupShaderFromSource(GL_VERTEX_SHADER, vertBuffer.getText());
        }
        
        bVertex = true;
    }
    
    if (_geomPath != ""){
        geomFile.clear();
        geomFilename = ofToDataPath(_geomPath);
        geomFile = ofFile( geomFilename );
        geomChangedTimes = getLastModified( geomFile );
        ofBuffer geomBuffer = ofBufferFromFile( geomFilename );
        
        if( geomBuffer.size() > 0 ){
            setupShaderFromSource(GL_GEOMETRY_SHADER, geomBuffer.getText());
        }
        
        setGeometryInputType(geomInType);
        setGeometryOutputType(geomOutType);
        setGeometryOutputCount(geomOutCount);
        
        bGeometry = true;
    }
    
	lastTimeCheckMillis = ofGetElapsedTimeMillis();
    
    if (linkProgram()){
        extractUniforms(fragBuffer.getText());
        return true;
    } else {
        return false;
    }
}

void UIShader::extractUniforms(string _shaderCode){
    //  Prepear the uniform to be checked
    //
    for (int i = 0; i < uniforms.size(); i++) {
        uniforms[i]->bUpdated = false;
    }
    
    //  Read the shader and check what needs to be added;
    //
    vector<string> lines = ofSplitString(_shaderCode, "\n");
    for(int i = 0; i<lines.size(); i++){
        vector<string> words = ofSplitString( lines[i], " ");
        
        for(int j = 0; j < words.size(); j++){
            if (words[j] == "uniform"){
                jumpNextWord(j,words);
                
                if (words[j] == "vec3"){
                    jumpNextWord(j,words);
                    eraseSemiColon(words[j]);
                    addUniform(UNIFORM_VEC3,words[j]);
                } else if (words[j] == "vec2"){
                    jumpNextWord(j,words);
                    eraseSemiColon(words[j]);
                    addUniform(UNIFORM_VEC2,words[j]);
                } else if (words[j] == "float"){
                    jumpNextWord(j,words);
                    eraseSemiColon(words[j]);
                    addUniform(UNIFORM_FLOAT,words[j]);
                }
                
            } else {
                break;
            }
        }
    }
    
//    //  Get rid of extra uniforms
//    //
//    for (int i = uniforms.size()-1; i >= 0; i--) {
//        if (!uniforms[i]->bUpdated){
//            
//            gui->removeWidget( uniforms[i]->name );
//            delete uniforms[i];
//            uniforms.erase(uniforms.begin()+i);
//        }
//    }
}

void UIShader::addUniform(UniformType _type, string _name){

    //  Check for an uniform with the same name in order not to lose the syphone address
    //
    bool bExist = false;
    for(int i = 0; i < uniforms.size(); i++){
        if ( uniforms[i]->name == _name ){
            uniforms[i]->bUpdated = true;
            bExist = true;
            break;
        }
    }
    
    if (!bExist){
        Uniform *newUniform = new Uniform(_type, _name);
        newUniform->bUpdated = true;
        
        //  Predefine uniforms
        //
        if (_name.find('_') == 0){
            newUniform->bNeedUI = false;
        } else if (_name == "time" && _type == UNIFORM_FLOAT){
            newUniform->bNeedUI = false;
        } else if (_name == "mouse" && _type == UNIFORM_VEC2){
            newUniform->bNeedUI = false;
        } else if ( (_name == "resolution" || _name == "windows" ) && _type == UNIFORM_VEC2 ){
            newUniform->bNeedUI = false;
        } else if ( _name == "screen" && _type == UNIFORM_VEC2 ){
            newUniform->bNeedUI = false;
        } else {
            
            if(gui != NULL){
                float length = (gui->getGlobalCanvasWidth()-gui->getWidgetSpacing()*5);
                float dim = gui->getGlobalSliderHeight();
                
                if (newUniform->type == UNIFORM_FLOAT){
                    gui->addSlider( newUniform->name, 0.0, 1.0, &newUniform->value.x );
                } else if (newUniform->type == UNIFORM_VEC2){
                    gui->addLabel( newUniform->name, OFX_UI_FONT_SMALL);
                    gui->addMinimalSlider("x", 0.0, 1.0, &newUniform->value.x,length/2.0, dim);
                    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                    gui->addMinimalSlider("y", 0.0, 1.0, &newUniform->value.y, length/2.0, dim);
                    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                    gui->addSpacer();
                } else if (newUniform->type == UNIFORM_VEC3){
                    gui->addLabel( newUniform->name, OFX_UI_FONT_SMALL);
                    gui->addMinimalSlider("x", 0.0, 1.0, &newUniform->value.x,length/3.0, dim);
                    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                    gui->addMinimalSlider("y", 0.0, 1.0, &newUniform->value.y, length/3.0, dim);
                    gui->addMinimalSlider("z", 0.0, 1.0, &newUniform->value.z, length/3.0, dim);
                    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                    gui->addSpacer();
                }
                
                gui->autoSizeToFitWidgets();
            }
        }
        
        uniforms.push_back(newUniform);
    }
}

void UIShader::checkShaderFile(){
	int currTime = ofGetElapsedTimeMillis();
	if( ((currTime - lastTimeCheckMillis) > millisBetweenFileCheck) ){
        if( fragFile.exists() ){
            std::time_t fragmentShaderFileLastChangeTime = getLastModified( fragFile );
            if( fragmentShaderFileLastChangeTime != fragChangedTimes ){
                fragChangedTimes = fragmentShaderFileLastChangeTime;
                reloadShader(fragFilename,vertFilename,geomFilename);
            } else if (bVertex){
                if( vertFile.exists() ){
                    std::time_t vertexShaderFileLastChangeTime = getLastModified( vertFile );
                    if( vertexShaderFileLastChangeTime != vertChangedTimes ){
                        vertChangedTimes = vertexShaderFileLastChangeTime;
                        reloadShader(fragFilename,vertFilename,geomFilename);
                    }
                }
                
                if( geomFile.exists() ){
                    std::time_t geometryShaderFileLastChangeTime = getLastModified( geomFile );
                    if( geometryShaderFileLastChangeTime != geomChangedTimes ){
                        vertChangedTimes = geometryShaderFileLastChangeTime;
                        reloadShader(fragFilename,vertFilename,geomFilename);
                    }
                }
            }
        }
        
		lastTimeCheckMillis = currTime;
	}
}

std::time_t UIShader::getLastModified( ofFile& _file ){
	if( _file.exists() ){
		Poco::File& pocoFile		= _file.getPocoFile();
		Poco::Timestamp timestamp	= pocoFile.getLastModified();
		std::time_t fileChangedTime = timestamp.epochTime();
		
		return fileChangedTime;
	} else {
		return 0;
	}
}

void UIShader::begin(){
    if (bEnable){
        checkShaderFile();
        
        ofShader::begin();
        
        for (int i = 0; i < uniforms.size(); i++){
            
            if (uniforms[i]->type == UNIFORM_FLOAT){
                
                if (uniforms[i]->name == "time"){
                    setUniform1f(uniforms[i]->name.c_str(), ofGetElapsedTimef());
                } else {
                    setUniform1f(uniforms[i]->name.c_str(), uniforms[i]->value.x );
                }
               
            } else if (uniforms[i]->type == UNIFORM_VEC2){
                
                if (uniforms[i]->name == "mouse"){
                    setUniform2f(uniforms[i]->name.c_str(), ofGetMouseX(), ofGetMouseY() );
                } else if (uniforms[i]->name == "screen"){
                    setUniform2f(uniforms[i]->name.c_str(), ofGetScreenWidth(), ofGetScreenHeight() );
                } else if (uniforms[i]->name == "windows" || uniforms[i]->name == "resolution" ){
                    setUniform2f(uniforms[i]->name.c_str(), ofGetWidth(), ofGetHeight() );
                } else {
                    setUniform2f(uniforms[i]->name.c_str(), uniforms[i]->value.x, uniforms[i]->value.y );
                }
                
            } else if (uniforms[i]->type == UNIFORM_VEC3){
                
                setUniform3f(uniforms[i]->name.c_str(), uniforms[i]->value.x, uniforms[i]->value.y, uniforms[i]->value.z );
                
            }
        }
    }
}

void UIShader::end(){
    if (bEnable){
        ofShader::end();
    }
}