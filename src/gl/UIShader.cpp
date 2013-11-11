//
//  UIShader.cpp
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/15/13.
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
                gui->addMinimalSlider("x", 0.0, 1.0, &uniforms[i]->value.x,length/2.0, dim);//->setShowValue(false);
                gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                gui->addMinimalSlider("y", 0.0, 1.0, &uniforms[i]->value.y, length/2.0, dim);//->setShowValue(false);
                gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                gui->addSpacer();
            } else if (uniforms[i]->type == UNIFORM_VEC3){
                gui->addLabel( uniforms[i]->name, OFX_UI_FONT_SMALL);
                gui->addMinimalSlider("x", 0.0, 1.0, &uniforms[i]->value.x,length/3.0, dim);//->setShowValue(false);
                gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
                gui->addMinimalSlider("y", 0.0, 1.0, &uniforms[i]->value.y, length/3.0, dim);//->setShowValue(false);
                gui->addMinimalSlider("z", 0.0, 1.0, &uniforms[i]->value.z, length/3.0, dim);//->setShowValue(false);
                gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
                gui->addSpacer();
            }
        }
    }
    
    gui->addButton("OPEN",&bOpen);
}

void UIShader::loadFrag(string _fragShader){
    reloadShader( _fragShader );
}

string UIShader::getClassName(){
    return shaderFile.getBaseName();
}

ofShader& UIShader::getShader(){
    return shader;
}

void UIShader::guiEvent(ofxUIEventArgs &e){
    if (gui != NULL){
        string name = e.widget->getName();
        
        if( name == "OPEN"){
            string comand = "open " + shaderFilename ;
            system(comand.c_str());
        }
    }
}

bool UIShader::reloadShader(string _filePath){
    shader.unload();
	
	// hackety hack, clear errors or shader will fail to compile
	GLuint err = glGetError();
	
    if (_filePath != "none"){
        shaderFilename = ofToDataPath( _filePath );
	}
    
	shaderFile.clear();
	shaderFile = ofFile( shaderFilename );
	shaderChangedTimes = getLastModified( shaderFile );
    lastTimeCheckMillis = ofGetElapsedTimeMillis();
    
	ofBuffer shaderBuffer = ofBufferFromFile( shaderFilename );
	
	if( shaderBuffer.size() > 0 ){
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderBuffer.getText());
	}
	
    if (shader.linkProgram()){
        
        //  Prepear the uniform to be checked
        //
        for (int i = 0; i < uniforms.size(); i++) {
            uniforms[i]->bUpdated = false;
        }
        
        //  Read the shader and check what needs to be added;
        //
        string fragmentShaderText = shaderBuffer.getText();
        vector<string> lines = ofSplitString(shaderBuffer.getText(), "\n");

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
        
        //  Get rid of extra uniforms
        //
        for (int i = uniforms.size()-1; i >= 0; i--) {
            if (!uniforms[i]->bUpdated){
                
                gui->removeWidget( uniforms[i]->name );
                delete uniforms[i];
                uniforms.erase(uniforms.begin()+i);
            }
        }
        
        return true;
    } else {
        return false;
    }
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
        if (_name == "time" && _type == UNIFORM_FLOAT){
            newUniform->bNeedUI = false;
        } else if (_name == "mouse" && _type == UNIFORM_VEC2){
            newUniform->bNeedUI = false;
        } else if ( (_name == "resolution" || _name == "windows" ) && _type == UNIFORM_VEC2 ){
            newUniform->bNeedUI = false;
        } else if ( _name == "screen" && _type == UNIFORM_VEC2 ){
            newUniform->bNeedUI = false;
        }
        
        uniforms.push_back(newUniform);
    }
}

void UIShader::checkShaderFile(){
	int currTime = ofGetElapsedTimeMillis();
	if( ((currTime - lastTimeCheckMillis) > millisBetweenFileCheck) ){
        if( shaderFile.exists() ){
            std::time_t fragmentShaderFileLastChangeTime = getLastModified( shaderFile );
            if( fragmentShaderFileLastChangeTime != shaderChangedTimes ){
                shaderChangedTimes = fragmentShaderFileLastChangeTime;
                reloadShader();
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
        
        shader.begin();
        
        for (int i = 0; i < uniforms.size(); i++){
            
            if (uniforms[i]->type == UNIFORM_FLOAT){
                
                if (uniforms[i]->name == "time"){
                    shader.setUniform1f(uniforms[i]->name.c_str(), ofGetElapsedTimef());
                } else {
                    shader.setUniform1f(uniforms[i]->name.c_str(), uniforms[i]->value.x );
                }
               
            } else if (uniforms[i]->type == UNIFORM_VEC2){
                
                if (uniforms[i]->name == "mouse"){
                    shader.setUniform2f(uniforms[i]->name.c_str(), ofGetMouseX(), ofGetMouseY() );
                } else if (uniforms[i]->name == "screen"){
                    shader.setUniform2f(uniforms[i]->name.c_str(), ofGetScreenWidth(), ofGetScreenHeight() );
                } else if (uniforms[i]->name == "windows" || uniforms[i]->name == "resolution" ){
                    shader.setUniform2f(uniforms[i]->name.c_str(), ofGetWidth(), ofGetHeight() );
                } else {
                    shader.setUniform2f(uniforms[i]->name.c_str(), uniforms[i]->value.x, uniforms[i]->value.y );
                }
                
            } else if (uniforms[i]->type == UNIFORM_VEC3){
                
                shader.setUniform3f(uniforms[i]->name.c_str(), uniforms[i]->value.x, uniforms[i]->value.y, uniforms[i]->value.z );
                
            }
        }
    }
}

void UIShader::end(){
    if (bEnable){
        shader.end();
    }
}