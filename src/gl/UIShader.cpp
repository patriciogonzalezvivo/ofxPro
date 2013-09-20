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

void UIShader::setupUI(){
    
}

void UIShader::loadFrag(string _fragShader){
    reloadShader( _fragShader );
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

                    if (words[j] == "sampler2DRect"){
                        jumpNextWord(j,words);
                        eraseSemiColon(words[j]);
                        addUniform(UNIFORM_SAMPLE2DRECT,words[j]);
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
        
        //
        //
        newUniform->bUpdated = true;
        
        //  Predefine uniforms
        //
        if (_name == "time" && _type == UNIFORM_FLOAT){
            
        } else if (_name == "mouse" && _type == UNIFORM_VEC2){
            
        } else if ( (_name == "resolution" || _name == "windows" ) && _type == UNIFORM_VEC2 ){
            
        } else if ( _name == "screen" && _type == UNIFORM_VEC2 ){
            
        } else if ( _name == "backbuffer" && _type == UNIFORM_SAMPLE2DRECT ){
            
        } else if ( _name == "tex0" && _type == UNIFORM_SAMPLE2DRECT ){
            
        } else {
//            newUniform->slider = gui->addSlider(_name, 0.0, 1.0, &(newUniform->fValue) );
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

void UIShader::guiEvent(ofxUIEventArgs &e){
    
}

void UIShader::begin(){
    if (bEnable){
        checkShaderFile();
        
        shader.begin();
        
        int texLoc = 0;
        for (int i = 0; i < uniforms.size(); i++){
            
            //  TEXTURES
            //
            if (uniforms[i]->type == UNIFORM_SAMPLE2DRECT) {
//
//                if ( uniforms[i]->name == "video" ){
//                    shader.setUniformTexture(uniforms[i]->name.c_str(),
//                                             video->getTextureReference(),
//                                             nTex - texLoc);
//                    
//                } else if ( uniforms[i]->name == "backbuffer" ){
//                    shader.setUniformTexture(uniforms[i]->name.c_str(),
//                                             pingpong.src->getTextureReference(),
//                                             nTex - texLoc);
//                    
//                }
                texLoc++;
                
            //  FLOATS
            //
            } else if (uniforms[i]->type == UNIFORM_FLOAT){
                
                if (uniforms[i]->name == "time"){
                    shader.setUniform1f(uniforms[i]->name.c_str(), ofGetElapsedTimef());
                }
                
//                else {
//                    shader.setUniform1f(uniforms[i]->name.c_str(), ofToFloat(uniforms[i]->name) );
//                }
                
            //  VEC2
            //
            } else if (uniforms[i]->type == UNIFORM_VEC2){
                
                if (uniforms[i]->name == "mouse"){
                    shader.setUniform2f(uniforms[i]->name.c_str(), ofGetMouseX(), ofGetMouseY() );
                } else if (uniforms[i]->name == "screen"){
                    shader.setUniform2f(uniforms[i]->name.c_str(), ofGetScreenWidth(), ofGetScreenHeight() );
                } else if (uniforms[i]->name == "windows" || uniforms[i]->name == "resolution" ){
                    shader.setUniform2f(uniforms[i]->name.c_str(), ofGetWidth(), ofGetHeight() );
                }
                
            }
        }
        nTex = texLoc;
    }
}

void UIShader::end(){
    if (bEnable){
        shader.end();
    }
}