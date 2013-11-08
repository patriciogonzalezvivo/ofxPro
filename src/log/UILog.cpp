//
//  UILog.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/8/13.
//
//

#include "UILog.h"

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
    bSaveNote = false;
}

UILog::~UILog(){
    
}

void UILog::setupUI(){
    gui->addToggle("REC", false);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addToggle("ALL",&bRecordAll);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
    gui->addToggle("ANOTATE",&bNoteTaking);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addButton("ADD",&bSaveNote);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addSpacer();
    
    string pathToNotes = dataPath+"notes/";
    ofDirectory dir(pathToNotes);
    if(dir.exists()){
        dir.listDir();
        int total = dir.getFiles().size();
        for (int i = 0; i < total; i++) {
            if (dir.getFiles()[i].getExtension() == "note" ){
                Note newNote;
                newNote.load(pathToNotes+dir.getFiles()[i].getBaseName()+".note");
                newNote.bEnable = false;
                notes.push_back(newNote);
                cout << "NOTE " << notes[notes.size()-1].name << " added " << endl;
                gui->addToggle(dir.getFiles()[i].getBaseName(),&notes[notes.size()-1].bEnable);
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
        //record(bRecording);
    } else if( name == "ALL" ){
        
    } else if( name == "ANOTATE"){
        if(!bNoteTaking){
            cout << "PEN UP" << endl;
            actualNote.penUp();
        }
    } else if( name == "ADD" ){
        if(bSaveNote){
            string noteName = ofSystemTextBoxDialog("Save Note as");
            if(noteName.length()){
                bNoteTaking = false;
                string noteFile = dataPath+"notes/"+noteName;
                
                actualNote.name = noteName;
                actualNote.save(noteFile+".note");
                notes.push_back(actualNote);
                actualNote.clearAll();
                
                if(camera != NULL){
                    camera->save(noteFile+".cam");
                }
                
                gui->addToggle(noteName, &notes[notes.size()-1].bEnable);
                gui->autoSizeToFitWidgets();
            }
        }
    } else if ( name == "ENABLE" ) {
        if(!bEnable){
            bNoteTaking = false;
            actualNote.penUp();
        }
    } else if ( name == "NOTES" ) {
        
    } else {
        
        cout << "CHECKING " << name << endl;
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
            actualNote.addPoint( camera->getCameraPtn()->screenToWorld(_mouse) );
        } else {
            actualNote.addPoint(_mouse);
        }
    }
}

bool UILog::penUp(){
    actualNote.penUp();
}

void UILog::screenShot(){
    ofImage img;
    img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
    if( !ofDirectory(dataPath+"snapshots/").exists() ){
        ofDirectory(dataPath+"snapshots/").create();
    }
    string recordPath = dataPath+"snapshots/" + ofGetTimestampString() + ".png";
    img.saveImage(recordPath);
    lastRercord = recordPath;
    upload();
}

void UILog::record(bool _state){
    if(bRecording){
        recorder.close();
        bRecording = false;
        ofxUIToggle *rec = (ofxUIToggle*)gui->getWidget("REC");
        rec->setValue(false);
    } else {
        if( !ofDirectory(dataPath+"videos/").exists() ){
            ofDirectory(dataPath+"videos/").create();
        }
        string recordPath = dataPath+"videos/" + ofGetTimestampString() + ".mov";
        //        recorder.setup(recordPath, ofGetWidth(), ofGetHeight(), 30, sampleRate, channels);
        recorder.setup(recordPath, ofGetWidth(), ofGetHeight(), 30); // no audio
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
        _texBase.getTextureReference().readToPixels(pixels);
        recorder.addFrame(pixels);
        
    }
}

void UILog::upload(){
    if(bRecording){
        record(false);
    }
    
    if(lastRercord!=""){
        if (!flickrAPI.bAuthenticated){
            if ( flickrAPI.authenticate("6394ea9fdcad0043386fbfd07f57a419","abf7c1706ee0fd7f",Flickr::FLICKR_WRITE) ){
                ofAddListener(flickrAPI.uploadComplete, this, &UILog::uploadCompleted);
            }
        }
        
        if (flickrAPI.bAuthenticated){
            string photoID = flickrAPI.upload(lastRercord);
            lastRercord = "";
        }
    }
}

void UILog::uploadCompleted(string &_recordID){
    string shortURL = flickrAPI.getMediaById(_recordID).getShortURL();
    cout << "UPLOAD COMPLETE! Check it at " << shortURL << endl;
}

void UILog::draw(){
    if(bEnable){
        actualNote.draw();
        for(int i = 0; i < notes.size();i++){
            notes[i].draw();
        }
    }
}