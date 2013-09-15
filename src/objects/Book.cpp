//
//  Book.cpp
//
//  Created by Patricio Gonzalez Vivo on 2/21/13.
//
//

#include "Book.h"

Book::Book(){
    bChange = true;
}

void Book::loadPage( string _imageFile ){
    ofImage newImage;
    if (newImage.loadImage(_imageFile)){
        
        //  Adapt the size of the book to fit with the bigger image
        //
        if (newImage.getWidth()*2 > width){
            width = newImage.getWidth()*2;
        }
        
        if (newImage.getHeight() > height){
            height = newImage.getHeight();
        }
        
        pages.push_back(newImage);
        
        pageNumber = 0;
    } else {
        ofLog(OF_LOG_ERROR, _imageFile + " is not an image");
    }
}

void Book::loadDirectory( string _folder ){
    ofDirectory dir;
    
    int nFiles = dir.listDir(_folder);
    if(nFiles) {
        for(int i=0; i<dir.numFiles(); i++) {
            loadPage( _folder + "/" + dir.getFile(i).getFileName() );
        }
    } else {
        ofLog(OF_LOG_ERROR, "Could not find folder " + _folder);
    }
}

void Book::setPosition(ofPoint _centerPosition){
    
    setFromCenter(_centerPosition, width, height);
    
    bChange = true;
}

void Book::update(){
    
    if (bChange){
        prevPage = *this;
        prevPage.width = this->width*0.5;
        nextPage = *this;
        nextPage.width = this->width*0.5;
        nextPage.x = this->x + this->width*0.5;
        page.set(nextPage);
        
        if (pageNumber < pages.size())
            page.front = &pages[pageNumber].getTextureReference();
        
        if (pageNumber < pages.size()-1)
            page.back = &pages[pageNumber+1].getTextureReference();
        
        bChange = false;
    }
    
    page.update();
    page.setHandAt(hand);
    
    ofPoint handVel = hand - prevHand;
    if ( prevPage.inside(hand) ){
        if (handVel.x < -3 &&
            page.getTransition() < -0.20 ){
            if (setNext()){
                page.setNormHandAt(ofPoint(1.0,1.0),1.0);
            }
        } else if (handVel.x > 3 &&
                   page.getTransition() > 0.90){
            if (setPrev()){
                page.setNormHandAt(ofPoint(0.0,1.0),1.0);
            }
        }
        
    }
    
    prevHand = hand;
}

bool Book::setNext(){
    
    if (pageNumber+2 < pages.size()){
        pageNumber += 2;
        bChange = true;
    }
    
    return bChange;
}

bool Book::setPrev(){
    if (pageNumber != 0){
        pageNumber -= 2;
        bChange = true;
    } else {
        page.setNormHandAt(ofPoint(1,1),1.0);
    }
    
    return bChange;
}

void Book::draw(bool _bDebug){
    ofPushStyle();
    
    //  Actual Page
    //
    ofSetColor(255);
    page.draw(_bDebug);
    
    //  Pevius & next Page
    //
    ofPushMatrix();
    ofTranslate(0, 0,-1);
    if (pageNumber > 0){
        drawPage( pages[pageNumber-1].getTextureReference(), prevPage);
    }
    if (pageNumber < pages.size()-2){
        drawPage( pages[pageNumber+2].getTextureReference(), nextPage);
    }
    ofPopMatrix();
    

    if (_bDebug){
        ofFill();
        ofSetColor(50, 50, 0,150);
        ofRect(prevPage);
        ofSetColor(0, 50, 50,150);
        ofRect(nextPage);
        
        ofNoFill();
        ofSetColor(255,255);
        ofRect(*this);
        
        ofTranslate(0,0, 1);
        ofSetColor(255,0,0,255);
        ofCircle(hand, 12);
    }
    
    ofPopStyle();
}