//
//  UISurface
//
//  Created by Patricio Gonzalez Vivo on 5/3/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com . All rights reserved.
//

#pragma once

#include "UIClass.h"
#include "HotKeys.h"

class UISurface : public UIClass {
public:
  
    UISurface();
    virtual ~UISurface();
    
    virtual string getClassName(){return name;};

    void        setCoorners(ofPoint _coorners[4]);
    
    int         getWidth() const {return  width; };
    int         getHeight() const {return height; };
    ofPoint     getSurfaceToScreen(ofPoint _pos){ return surfaceToScreenMatrix * _pos; };
    ofPoint     getScreenToSurface(ofPoint _pos){ return screenToSurfaceMatrix * _pos; };
    GLfloat*    getGlMatrix() { return glMatrix; };
    
    void        move(ofPoint _pos);
    void        scale(float _scale);
    void        rotate(float _angle);
    
    void        draw( ofBaseHasTexture &_texture );
    void        draw( ofTexture &_texture );
    
    bool        isOver(ofPoint _loc) { return textureCorners.inside(_loc); };
    
    string      name;
    float       width, height;
    bool        bEdit;
    
protected:
    virtual void    setupUI( );
    virtual void    guiEvent(ofxUIEventArgs &e);
    
    void        doSurfaceToScreenMatrix();                  // Update the SurfaceToScreen transformation matrix
    void        doScreenToSurfaceMatrix();                  // Update the ScreenToSurface transformation matrix
    void        doGaussianElimination(float *input, int n); // This is used making the matrix
    
    // Mouse & Key Events from the testApp
    //
    void        _mousePressed(ofMouseEventArgs &e);
    void        _mouseDragged(ofMouseEventArgs &e);
    void        _mouseReleased(ofMouseEventArgs &e);
    
    // Texture varialbes
    //
    ofPolyline  textureCorners;
    int         selectedTextureCorner;
    int         textureWidth, textureHeight;
    
    ofPoint     src[4];
    ofMatrix4x4 surfaceToScreenMatrix;
    ofMatrix4x4 screenToSurfaceMatrix;
    GLfloat     glMatrix[16];

    bool        bUpdate;
};
