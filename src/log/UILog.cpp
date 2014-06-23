//
//  UILog.cpp
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#include "UILog.h"

#define STRINGIFY(A) #A

UILog::UILog(){
#ifdef TARGET_OSX
    dataPath = "../../../data/";
#else
    dataPath = "";
#endif
    
    renderTarget = NULL;
    camera = NULL;
    
    bRecording = false;
    bRecordAll = false;
    addNewNote = false;
    
    lastPicture = "";
    lastRercord = "";
    actualNote = new Note();
    actualNote->BL = &BL;
    
    string vertexShader = STRINGIFY(uniform float minDistance;
                                    uniform float maxDistance;
                                    
                                    float map(float value, float inputMin, float inputMax, float outputMin, float outputMax) {;
                                        return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
                                    }
                                    
                                    const float epsilon = 1e-6;
                                    void main(void){
                                        vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex;
                                        gl_Position = gl_ProjectionMatrix * eyeCoord;
                                        gl_FrontColor = gl_Color * clamp(map(gl_Position.z, minDistance, maxDistance, 1.0, 0.0), 0.0, 1.0);
                                    }
                                    );
    
    string fragShader = STRINGIFY(
                                  void main (void) {
                                      vec2 st = gl_TexCoord[0].xy;
                                      gl_FragColor = gl_Color;
                                  }
                                  );

    shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
    BL.size = 1.8;
    ofDisableArbTex();
    ofLoadImage(BLTexture, "GUI/dot.png");
    ofEnableArbTex();
    BL.tex = &BLTexture;
}

UILog::~UILog(){
    delete actualNote;
    for(int i = 0; i< notes.size(); i++){
        delete notes[i];
    }
    notes.clear();
}

void UILog::setupUI(){
    gui->addToggle("REC", false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
//    gui->addToggle("ALL",&bRecordAll);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
    gui->addToggle("ANOTATE",&bNoteTaking);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
    textField = gui->addTextInput("ADD", "", OFX_UI_FONT_SMALL);
    gui->setTriggerWidgetsUponLoad(false);
    string pathToNotes = dataPath+"notes/";
    ofDirectory dir(pathToNotes);
    if(dir.exists()){
        dir.listDir();
        int total = dir.getFiles().size();
        for (int i = 0; i < total; i++) {
            if (dir.getFiles()[i].getExtension() == "note" ){
                Note *newNote = new Note();
                newNote->BL = &BL;
                newNote->load(pathToNotes+dir.getFiles()[i].getBaseName()+".note");
                newNote->bEnable = false;
                notes.push_back(newNote);
                gui->addToggle(dir.getFiles()[i].getBaseName(),&notes[notes.size()-1]->bEnable);
            }
        }
    } else {
        dir.createDirectory(pathToNotes);
    }
    
    gui->autoSizeToFitWidgets();
}

void UILog::guiEvent(ofxUIEventArgs &e){

    string name = e.widget->getName();
    if( name == "REC" ){
        record(!bRecording);
    } else if( name == "ALL" ){
        
    } else if( name == "ANOTATE"){
        if(!bNoteTaking){
            actualNote->penUp();
        }
    } else if( name == "ADD" ){
        string noteName = textField->getTextString();
        
        if(noteName.length()>1){
            bNoteTaking = false;
            string noteFile = dataPath+"notes/"+noteName;
            
            actualNote->name = noteName;
            actualNote->save(noteFile+".note");
            
            bool isNew = true;
            for (int i = 0; i < notes.size(); i++) {
                if(notes[i]->name == noteFile){
                    isNew = false;
                    break;
                }
            }
            if(camera != NULL){
                camera->save(noteFile+".cam");
            }
            
            notes.push_back(actualNote);
            actualNote = new Note();
            actualNote->BL = &BL;
            addNewNote = isNew;
        }
        
    } else if ( name == "ENABLE" ) {
        if(!bEnable){
            bNoteTaking = false;
            actualNote->penUp();
        }
    } else if ( name == "NOTES" ) {
        
    } else {
        ofxUIToggle *t = ((ofxUIToggle *)gui->getWidget(name));
        if(t != NULL ){
            if (t->getValue()){
                if (camera != NULL){
                    camera->load(dataPath+"notes/"+name+".cam");
                }
            }
        }
    }
}

bool UILog::penDown(ofPoint _mouse){
    if(bEnable){
        if(camera != NULL){
            actualNote->addPoint(camera->getCameraPtn()->screenToWorld(_mouse) );
        } else {
            actualNote->addPoint(_mouse);
        }
    }
}

bool UILog::penUp(){
    actualNote->penUp();
}

void UILog::screenShot(string _post){
    ofImage img;
    img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
    if( !ofDirectory(dataPath+"snapshots/").exists() ){
        ofDirectory(dataPath+"snapshots/").create();
    }
    string picturePath = dataPath+"snapshots/" + ofGetTimestampString("%y-%m-%d-%H-%M-%S")+"_post" + ".png";
    img.saveImage(picturePath);
    lastPicture = picturePath;
    upload();
}

void UILog::record(bool _state){
    if(bRecording && !_state){
        recorder.close();
        bRecording = false;
        ofxUIToggle *rec = (ofxUIToggle*)gui->getWidget("REC");
        rec->setValue(false);
        upload();
    } else if (!bRecording && _state) {
        if( !ofDirectory(dataPath+"videos/").exists() ){
            ofDirectory(dataPath+"videos/").create();
        }
        string recordPath = dataPath+"videos/" + ofGetTimestampString() + ".mov";
        //        recorder.setup(recordPath, ofGetWidth(), ofGetHeight(), 30, sampleRate, channels);
        recorder.setup(recordPath, ofGetWidth(), ofGetHeight(), 24); // no audio
        lastRercord = recordPath;
        bRecording = true;
        ofxUIToggle *rec = (ofxUIToggle*)gui->getWidget("REC");
        rec->setValue(true);
    }
}

void UILog::recordAddFrame(ofBaseHasTexture &_texBase){
    if (bRecordAll){
        ofImage img;
        img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
        recorder.addFrame(img.getPixelsRef());
    } else {
        ofPixels pixels;
        pixels.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR_ALPHA);
        _texBase.getTextureReference().readToPixels(pixels);
        recorder.addFrame(pixels);
        
    }
}

void UILog::upload(){
    if(bEnable){
        if (!flickrAPI.bAuthenticated){
            flickrAPI.authenticate("6394ea9fdcad0043386fbfd07f57a419","abf7c1706ee0fd7f",Flickr::FLICKR_WRITE);
        }
        
        if (flickrAPI.bAuthenticated){
            
            string fileToUpload;
            
            if(lastPicture!=""){
                fileToUpload = lastPicture;
                lastPicture = "";
            } else {
                if(bRecording){
                    record(false);
                }
                fileToUpload = lastRercord;
                lastRercord = "";
            }
            
            if(fileToUpload!=""){
                FileUploader *file = flickrAPI.upload(fileToUpload);
                
                if (file != NULL){
                    ofAddListener(file->isDone, this, &UILog::uploadCompleted);
                    uploadingFiles.push_back(file);
                    lastRercord = "";
                }
            }
        }
    }
}

void UILog::uploadCompleted(string &_result){
    string _recordID;

    ofxXmlSettings xml;
    xml.loadFromBuffer(_result);
    xml.pushTag("rsp");{
        _recordID = xml.getValue("photoid", "");
    }; xml.popTag();
    
    string shortURL = flickrAPI.getMediaById(_recordID).getShortURL();
    cout << "UPLOAD COMPLETE! Check it at " << shortURL << endl;
}

void UILog::draw(){
    if(addNewNote){
        gui->addToggle(notes[notes.size()-1]->name, &notes[notes.size()-1]->bEnable);
        gui->autoSizeToFitWidgets();
        addNewNote = false;
    }
    
    if(bEnable){
        if(camera != NULL){
            ofPushStyle();
            ofSetLineWidth(2);
            shader.begin();
            shader.setUniform1f("minDistance", 0);
            shader.setUniform1f("maxDistance", 100);
            actualNote->draw();
            for(int i = 0; i < notes.size();i++){
                notes[i]->draw();
            }
            shader.end();
            ofPopStyle();
        } else {
            ofPushStyle();
            actualNote->smoothDraw();
            for(int i = 0; i < notes.size();i++){
                notes[i]->smoothDraw();
            }
            ofPopStyle();
        }
    }
}

void UILog::drawStatus(){
    if(isRecording()){
        ofPushStyle();
        ofFill();
        ofSetColor(255, 0, 0,abs(sin(ofGetElapsedTimef()))*255);
        ofCircle(ofGetWidth()-20, 20, 5);
        ofPopStyle();
    }
    
    ofPushStyle();
    ofFill();
    for(int i = 0; i < uploadingFiles.size(); i++ ){
        ofSetColor(0, 255, 0,abs(sin(ofGetElapsedTimef()+((float)i*0.5)))*255);
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()-20-10*i, 20+15);
        ofBeginShape();
        ofVertex(0, -5);
        ofVertex(-5, 2);
        ofVertex(5, 2);
        ofEndShape(true);
        ofPopMatrix();
    }
    ofPopStyle();
    
    for(int i = uploadingFiles.size()-1; i>=0;i--){
        if ( uploadingFiles[i]->bFinish ){
            ofRemoveListener(uploadingFiles[i]->isDone, this, &UILog::uploadCompleted);
            delete uploadingFiles[i];
            uploadingFiles.erase(uploadingFiles.begin()+i);
        }
    }
}