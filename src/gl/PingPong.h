//
//  PingPong.h
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#ifndef PINGPONG
#define PINGPONG

#include "ofMain.h"

class PingPong : public ofBaseHasTexture {
public:
    virtual void    allocate( int _width, int _height, int _internalformat = GL_RGBA){
        width = _width;
        height = _height;
        
        // Allocate
        //
        for(int i = 0; i < 2; i++)
            FBOs[i].allocate(width,height, _internalformat );
        
        // Clean
        //
        clear();
        
        // Set everything to 0
        //
        flag = 0;
        swap();
        flag = 0;
    }
    
    virtual int     getWidth(){return width;};
    virtual int     getHeight(){return height;};
    
    virtual void    setUseTexture(bool bUseTex){ };
    virtual ofTexture& getTextureReference() { return dst->getTextureReference(); };
    
    virtual void    swap(){
        src = &(FBOs[(flag)%2]);
        dst = &(FBOs[++(flag)%2]);
    }
    
    virtual void beginBoth(){
        FBOs[0].begin();
    }
    
    virtual void endBoth(){
        FBOs[0].end();
        
        FBOs[1].begin();
        FBOs[0].draw(0,0);
        FBOs[1].end();
    }
    
    virtual void clear(float _alpha = 0){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(ofFloatColor(0,_alpha));
            FBOs[i].end();
        }
    }
    
    virtual ofFbo& operator[]( int n ){ return FBOs[n];}
    
    ofFbo   *src;       // Source       ->  Ping
    ofFbo   *dst;       // Destination  ->  Pong
    
private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO¬¥s
    int     flag;       // Integer for making a quick swap
    int     width,height;
};

#endif
