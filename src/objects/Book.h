//
//  Book.h
//
//  Created by Patricio Gonzalez Vivo on 2/21/13.
//
//

#ifndef BOOK
#define BOOK

#include "ofMain.h"

#include "Page.h"

class Book : public ofRectangle {
public:
    
    Book();
    
    void    loadPage( string _imageFile );
    void    loadDirectory( string _dir );
    
    void    setPosition(ofPoint _centerPosition);
    bool    setNext();
    bool    setPrev();
    
    void    update();
    void    draw(bool _bDebug = false);

    ofPoint hand;
    bool    bChange;
private:
    
    ofRectangle     prevPage,nextPage;
    Page            page;
    
    vector<ofImage> pages;
    
    ofPoint prevHand;
    int     pageNumber;
};

#endif
