//
//  UIBackShader.cpp
//  00-CLOUDS
//
//  Created by Patricio Gonzalez Vivo on 7/8/14.
//
//

#include "UIBackShader.h"

void bsJumpNextWord(int &_index, vector<string> &_words){
    _index++;
    
    while( ((_words[ _index].size() <= 1) || (_words[ _index] == "\t")) && ( _index < _words.size())){
        _index++;
    }
}

void bsEraseSemiColon(string &_word){
    int end = _word.find(';') ;
    for(int i = _word.size()-1; i >= end ; i--){
        _word.erase(_word.begin()+i);
    }
}

UIBackShader::UIBackShader(){
    UIBackground();
    
    fragmentShader = "#version 120\n\n\
\n\
uniform vec3 colorOne;\n\
uniform vec3 colorTwo;\n\
\n\
uniform vec2 resolution;\n\
\n\
uniform float time;\n\
uniform int gradient;\n\
\n\
void main(){\n\
    vec2 st = gl_FragCoord.xy/resolution.xy;\n\
\n\
    if(gradient==1){\n\
        gl_FragColor = vec4( mix(colorOne,colorTwo, st.y), 1.0);\n\
    } else {\n\
        gl_FragColor = vec4( colorOne, 1.0);\n\
    }\n\
}";
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    shader.linkProgram();
    
    bMeshReady = false;
    
    ofAddListener(ofEvents().update, this, &UIBackShader::update);
    ofAddListener(ofEvents().windowResized , this, &UIBackShader::windowResized);
}

UIBackShader::UIBackShader(string _name){
    UIBackground();
    
    fragmentShader = "#version 120\n\n\
\n\
uniform vec3 colorOne;\n\
uniform vec3 colorTwo;\n\
\n\
uniform vec2 resolution;\n\
\n\
uniform float time;\n\
uniform int gradient;\n\
\n\
void main(){\n\
    vec2 st = gl_FragCoord.xy/resolution.xy;\n\
    \n\
    if(gradient==1){\n\
        gl_FragColor = vec4( mix(colorOne,colorTwo, st.y), 1.0);\n\
    } else {\n\
        gl_FragColor = vec4( colorOne, 1.0);\n\
    }\n\
}";
    
    load(_name);
    
    bMeshReady = false;
    ofAddListener(ofEvents().update, this, &UIBackShader::update);
    ofAddListener(ofEvents().windowResized , this, &UIBackShader::windowResized);
}

bool UIBackShader::load(string _name){
    
    if( _name.find(".frag")==-1){
        _name+=".frag";
    }
    
    ofFile fragFile = ofFile(_name);

    if (!fragFile.exists() ){
        ofBuffer frag;
        frag.append(fragmentShader);
        ofBufferToFile(_name, frag);
    }
    
    reloadShader(_name);
}

string  UIBackShader::getClassName(){
    if(fragFile.isFile()){
        return fragFile.getBaseName();
    } else {
        return "BACKGROUND";
    }
}


bool UIBackShader::reloadShader(string _fragPath){
    shader.unload();
	
	// hackety hack, clear errors or shader will fail to compile
	GLuint err = glGetError();
	
    fragFile.clear();
    fragFilename = ofToDataPath( _fragPath );
    fragFile = ofFile( fragFilename );
    fragChangedTimes = getLastModified( fragFile );
    ofBuffer fragBuffer = ofBufferFromFile( fragFilename );
    if( fragBuffer.size() > 0 ){
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragBuffer.getText());
    }
    
	lastTimeCheckMillis = ofGetElapsedTimeMillis();
    
    if (shader.linkProgram()){
        extractUniforms(fragBuffer.getText());
        return true;
    } else {
        return false;
    }
}

void UIBackShader::extractUniforms(string _shaderCode){
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
                bsJumpNextWord(j,words);
                
                if (words[j] == "vec3"){
                    bsJumpNextWord(j,words);
                    bsEraseSemiColon(words[j]);
                    addUniform(UNIFORM_VEC3,words[j]);
                } else if (words[j] == "vec2"){
                    bsJumpNextWord(j,words);
                    bsEraseSemiColon(words[j]);
                    addUniform(UNIFORM_VEC2,words[j]);
                } else if (words[j] == "float"){
                    bsJumpNextWord(j,words);
                    bsEraseSemiColon(words[j]);
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
}

void UIBackShader::addUniform(UniformType _type, string _name){
    
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
        } else if ( (_name == "colorOne" || _name == "colorTwo") && _type == UNIFORM_VEC3 ){
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

void UIBackShader::checkShaderFile(){
	int currTime = ofGetElapsedTimeMillis();
	if( ((currTime - lastTimeCheckMillis) > millisBetweenFileCheck) ){
        if( fragFile.exists() ){
            std::time_t fragmentShaderFileLastChangeTime = getLastModified( fragFile );
            if( fragmentShaderFileLastChangeTime != fragChangedTimes ){
                fragChangedTimes = fragmentShaderFileLastChangeTime;
                reloadShader(fragFilename);
            }
        }
        
		lastTimeCheckMillis = currTime;
	}
}

std::time_t UIBackShader::getLastModified( ofFile& _file ){
	if( _file.exists() ){
		Poco::File& pocoFile		= _file.getPocoFile();
		Poco::Timestamp timestamp	= pocoFile.getLastModified();
		std::time_t fileChangedTime = timestamp.epochTime();
		
		return fileChangedTime;
	} else {
		return 0;
	}
}

void UIBackShader::setupUI(){
    UIBackground::setupUI();
    gui->addSpacer();
    
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

void UIBackShader::guiEvent(ofxUIEventArgs &e){
    if (gui != NULL){
        string name = e.widget->getName();
        
        if( name == "OPEN"){
            
            string comand = "open " + fragFilename;

            system(comand.c_str());
            
        }
    }
}

void UIBackShader::update(ofEventArgs & args){
    if(bEnable){
        if(!bMeshReady){
            ofRectangle screenRect(0,0,ofGetWidth(), ofGetHeight());
            
            mesh.clear();
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            
            mesh.addVertex(ofVec3f(screenRect.getMinX(),screenRect.getMinY()));
            mesh.addTexCoord(ofVec2f(0,0));
            
            mesh.addVertex(ofVec3f(screenRect.getMinX(),screenRect.getMaxY()));
            mesh.addTexCoord(ofVec2f(0,1));
            
            mesh.addVertex(ofVec3f(screenRect.getMaxX(),screenRect.getMinY()));
            mesh.addTexCoord(ofVec2f(1,0));
            
            mesh.addVertex(ofVec3f(screenRect.getMaxX(),screenRect.getMaxY()));
            mesh.addTexCoord(ofVec2f(1,1));
            
            bMeshReady = true;
        }
        
        color.update();
        if (bGradient){
            color2.update();
        }
    }
}

void UIBackShader::windowResized(ofResizeEventArgs & args){
    bMeshReady = false;
}

void UIBackShader::draw(){
    if(bEnable){
        ofBackground(color);
        
        ofPushStyle();
        ofSetColor(255);
        
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
                
                if(uniforms[i]->name == "colorOne" ){
                    shader.setUniform3f("colorOne", color.r, color.g, color.b);
                } else if (uniforms[i]->name == "colorTwo"){
                    shader.setUniform3f("colorTwo", color2.r, color2.g, color2.b);
                } else {
                    shader.setUniform3f(uniforms[i]->name.c_str(), uniforms[i]->value.x, uniforms[i]->value.y, uniforms[i]->value.z );
                }
                
            }
        }
        shader.setUniform1i("gradient", bGradient);
        
        mesh.draw();
        
        shader.end();
        
        ofPopStyle();
    }
}