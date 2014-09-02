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
    bRecording = false;
    
    lastPicture = "";
    lastRercord = "";
    
    pixels.allocate(ofGetScreenWidth(),ofGetScreenHeight(),OF_IMAGE_COLOR);
}

void UILog::setupUI(){
    gui->addToggle("REC", &bRecording);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    
    gui->autoSizeToFitWidgets();
}

void UILog::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if( name == "REC" ){
        record(bRecording);
    }
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

void UILog::linkRenderTarget(ofBaseHasTexture *_tex){
    renderTarget = _tex;
    if(renderTarget->getTextureReference().getTextureData().glTypeInternal == GL_RGBA ){
        recorder.setPixelFormat("rgba");
    } else {
        recorder.setPixelFormat("rgb24");
    }
};

void UILog::recordAddFrame(){
    if(renderTarget != NULL){
        if(pixels.getWidth() != renderTarget->getTextureReference().getWidth() ||
           pixels.getHeight() != renderTarget->getTextureReference().getHeight()){
            pixels.allocate(renderTarget->getTextureReference().getWidth(),
                            renderTarget->getTextureReference().getHeight(),
                            OF_IMAGE_COLOR);
        }
        renderTarget->getTextureReference().readToPixels(pixels);
        recorder.addFrame(pixels);
    }
}

void UILog::upload(){
    if(bEnable){
        if (!flickrAPI.bAuthenticated){
            flickrAPI.authenticate("c8219f379ec3cdd264e5aab216e98f5a","d7e8d3f12300d0e9",Flickr::FLICKR_WRITE);
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