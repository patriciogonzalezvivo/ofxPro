//
//  UISurface.cpp
//  ofxInteractiveSurface
//
//  Created by Patricio Gonzalez Vivo on 5/3/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com . All rights reserved.
//

#include "UISurface.h"

UISurface::UISurface(){
    name        = "SURFACE";
    selectedTextureCorner = -1;

    bEdit       = false;
    bUpdate     = true;
    
    width       = 0;
    height      = 0;
    
    for (int i = 0; i < 4; i++){
        textureCorners.addVertex(ofPoint(0,0));
    }
}

UISurface::~UISurface(){
    if (bEdit){
        ofRemoveListener(ofEvents().mousePressed, this, &UISurface::_mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &UISurface::_mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &UISurface::_mouseReleased);
    }
}

void UISurface::setupUI( ){
    for (int i = 0; i < 4; i++){
        addUIPos("C"+ofToString(i),textureCorners[i],true);
    }
    bool bFalse = false;
    gui->addButton("RESTORE", bFalse);
    gui->addToggle("EDIT", &bEdit);
}

void UISurface::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if(name == "EDIT"){
        if (bEdit) {
            ofAddListener(ofEvents().mousePressed, this, &UISurface::_mousePressed);
            ofAddListener(ofEvents().mouseDragged, this, &UISurface::_mouseDragged);
            ofAddListener(ofEvents().mouseReleased, this, &UISurface::_mouseReleased);
        } else {
            ofRemoveListener(ofEvents().mousePressed, this, &UISurface::_mousePressed);
            ofRemoveListener(ofEvents().mouseDragged, this, &UISurface::_mouseDragged);
            ofRemoveListener(ofEvents().mouseReleased, this, &UISurface::_mouseReleased);
        }
    } else if(name == "RESTORE"){
        textureCorners[0].set(0,0);
        textureCorners[1].set(width,0);
        textureCorners[2].set(width,height);
        textureCorners[3].set(0,height);
        bUpdate = true;
    } else {
        bUpdate = true;
    }
}

void UISurface::setCoorners(ofPoint _coorners[4]){
    for (int i = 0; i < 4; i++){
        textureCorners[i].set(_coorners[i]);
    }
    
    bUpdate = true;
}

void UISurface::draw( ofBaseHasTexture &_texture ){
    draw(_texture.getTextureReference());
}

// ------------------------------------------------------ LOOPS
//
void UISurface::draw( ofTexture &_texture ){
    
    if(bEnable){
        
        if ((width != _texture.getWidth()) ||
            (height != _texture.getHeight()) ){
            width = _texture.getWidth();
            height = _texture.getHeight();
            bUpdate= true;
        }
        
        if (bUpdate){
            doSurfaceToScreenMatrix();
            doScreenToSurfaceMatrix();
            bUpdate= false;
        }
        
        ofPushMatrix();
        
        glMultMatrixf(glMatrix);
        _texture.draw(0,0);
        ofPopMatrix();
        
        if (bEdit){
            ofPushStyle();
            ofFill();
            // Draw dragables texture corners
            //
            for(int i = 0; i < 4; i++){
                if ( ( selectedTextureCorner == i) || ( ofDist(ofGetMouseX(), ofGetMouseY(), textureCorners[i].x, textureCorners[i].y) <= 4 ) ) ofSetColor(200,255);
                else ofSetColor(200,100);
                
                ofRect(textureCorners[i].x-4,textureCorners[i].y-4, 8,8);
                
                // Draw contour Line
                //
                ofLine(textureCorners[i].x, textureCorners[i].y, textureCorners[(i+1)%4].x, textureCorners[(i+1)%4].y);
            }
            ofPopStyle();
        }
    }
}

// -------------------------------------------------------- ACTIONS
//
void UISurface::move(ofPoint _pos){
    ofVec2f diff = _pos - textureCorners.getCentroid2D();
    
    for(int i = 0; i < 4; i++){
        textureCorners[i] += diff;
    }
    
    doScreenToSurfaceMatrix();
}

void UISurface::scale(float _scale){
    ofVec2f center = textureCorners.getCentroid2D();
    
    for(int i = 0; i < 4; i++){
        ofVec2f fromCenterToCorner = textureCorners[i] - center;
        float radio = fromCenterToCorner.length();
        float angle = -1.0*atan2f(fromCenterToCorner.x,fromCenterToCorner.y)+(PI/2);
        radio *= _scale;
        textureCorners[i] = center + ofPoint(radio * cos(angle),
                                             radio * sin(angle),
                                             0.0);
    }
    doSurfaceToScreenMatrix();
}

void UISurface::rotate(float _rotAngle){
    ofVec2f center = textureCorners.getCentroid2D();
    for(int i = 0; i < 4; i++){
        ofVec2f fromCenterToCorner = textureCorners[i] - center;
        float radio = fromCenterToCorner.length();
        float angle = -1.0*atan2f(fromCenterToCorner.x,fromCenterToCorner.y)+(PI/2);
        angle += _rotAngle;
        textureCorners[i] = center + ofPoint(radio * cos(angle),
                                             radio * sin(angle),
                                             0.0);
    }
    doSurfaceToScreenMatrix();
}

void UISurface::doSurfaceToScreenMatrix(){
    ofPoint src[4];
    
    src[0].set(0, 0);
    src[1].set(width,0.0);
    src[2].set(width,height);
    src[3].set(0, height);
    
    ofPoint dst[4];
    for(int i = 0; i < 4; i++){
        dst[i] = textureCorners[i];
    }
    
    // create the equation system to be solved
    //
    // from: Multiple View Geometry in Computer Vision 2ed
    //       Hartley R. and Zisserman A.
    //
    // x' = xH
    // where H is the homography: a 3 by 3 matrix
    // that transformed to inhomogeneous coordinates for each point
    // gives the following equations for each point:
    //
    // x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
    // y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
    //
    // as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
    // so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
    // after ordering the terms it gives the following matrix
    // that can be solved with gaussian elimination:
    
    float P[8][9]=
    {
        {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
        {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12
        
        {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
        {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21
        
        {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
        {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23
        
        {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
        {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
    };
    
    doGaussianElimination(&P[0][0],9);
    
    // gaussian elimination gives the results of the equation system
    // in the last column of the original matrix.
    // opengl needs the transposed 4x4 matrix:
    float aux_H[]= {P[0][8],P[3][8],0,P[6][8], // h11  h21 0 h31
        P[1][8],P[4][8],0,P[7][8], // h12  h22 0 h32
        0      ,      0,0,0,       // 0    0   0 0
        P[2][8],P[5][8],0,1        // h13  h23 0 h33
    };                          
    
    for(int i=0; i<16; i++) 
        glMatrix[i] = aux_H[i];
    
    surfaceToScreenMatrix(0,0)=P[0][8];
    surfaceToScreenMatrix(0,1)=P[1][8];
    surfaceToScreenMatrix(0,2)=0;
    surfaceToScreenMatrix(0,3)=P[2][8];
    
    surfaceToScreenMatrix(1,0)=P[3][8];
    surfaceToScreenMatrix(1,1)=P[4][8];
    surfaceToScreenMatrix(1,2)=0;
    surfaceToScreenMatrix(1,3)=P[5][8];
    
    surfaceToScreenMatrix(2,0)=0;
    surfaceToScreenMatrix(2,1)=0;
    surfaceToScreenMatrix(2,2)=0;
    surfaceToScreenMatrix(2,3)=0;
    
    surfaceToScreenMatrix(3,0)=P[6][8];
    surfaceToScreenMatrix(3,1)=P[7][8];
    surfaceToScreenMatrix(3,2)=0;
    surfaceToScreenMatrix(3,3)=1;
}

void UISurface::doScreenToSurfaceMatrix(){
    ofPoint dst[4];
    
    dst[0].set(0, 0);
    dst[1].set(width,0.0);
    dst[2].set(width,height);
    dst[3].set(0, height);
    
    ofPoint src[4];
    for(int i = 0; i < 4; i++){
        src[i] = textureCorners[i];
    }
    
    // create the equation system to be solved
    //
    // from: Multiple View Geometry in Computer Vision 2ed
    //       Hartley R. and Zisserman A.
    //
    // x' = xH
    // where H is the homography: a 3 by 3 matrix
    // that transformed to inhomogeneous coordinates for each point
    // gives the following equations for each point:
    //
    // x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
    // y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
    //
    // as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
    // so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
    // after ordering the terms it gives the following matrix
    // that can be solved with gaussian elimination:
    
    float P[8][9]=
    {
        {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
        {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12
        
        {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
        {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21
        
        {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
        {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23
        
        {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
        {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
    };
    
    doGaussianElimination(&P[0][0],9);
    
    screenToSurfaceMatrix(0,0)=P[0][8];
	screenToSurfaceMatrix(0,1)=P[1][8];
	screenToSurfaceMatrix(0,2)=0;
	screenToSurfaceMatrix(0,3)=P[2][8];
	
	screenToSurfaceMatrix(1,0)=P[3][8];
	screenToSurfaceMatrix(1,1)=P[4][8];
	screenToSurfaceMatrix(1,2)=0;
	screenToSurfaceMatrix(1,3)=P[5][8];
	
	screenToSurfaceMatrix(2,0)=0;
	screenToSurfaceMatrix(2,1)=0;
	screenToSurfaceMatrix(2,2)=0;
	screenToSurfaceMatrix(2,3)=0;
	
	screenToSurfaceMatrix(3,0)=P[6][8];
	screenToSurfaceMatrix(3,1)=P[7][8];
	screenToSurfaceMatrix(3,2)=0;
	screenToSurfaceMatrix(3,3)=1;
    
}

void UISurface::doGaussianElimination(float *input, int n){
    // ported to c from pseudocode in
    // http://en.wikipedia.org/wiki/Gaussian_elimination
    
    float * A = input;
    int i = 0;
    int j = 0;
    int m = n-1;
    while (i < m && j < n)
    {
        // Find pivot in column j, starting in row i:
        int maxi = i;
        for(int k = i+1; k<m; k++)
        {
            if(fabs(A[k*n+j]) > fabs(A[maxi*n+j]))
            {
                maxi = k;
            }
        }
        if (A[maxi*n+j] != 0)
        {
            //swap rows i and maxi, but do not change the value of i
            if(i!=maxi)
                for(int k=0; k<n; k++)
                {
                    float aux = A[i*n+k];
                    A[i*n+k]=A[maxi*n+k];
                    A[maxi*n+k]=aux;
                }
            //Now A[i,j] will contain the old value of A[maxi,j].
            //divide each entry in row i by A[i,j]
            float A_ij=A[i*n+j];
            for(int k=0; k<n; k++)
            {
                A[i*n+k]/=A_ij;
            }
            //Now A[i,j] will have the value 1.
            for(int u = i+1; u< m; u++)
            {
                //subtract A[u,j] * row i from row u
                float A_uj = A[u*n+j];
                for(int k=0; k<n; k++)
                {
                    A[u*n+k]-=A_uj*A[i*n+k];
                }
                //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
            }
            
            i++;
        }
        j++;
    }
    
    //back substitution
    for(int i=m-2; i>=0; i--)
    {
        for(int j=i+1; j<n-1; j++)
        {
            A[i*n+m]-=A[i*n+j]*A[j*n+m];
            //A[i*n+j]=0;
        }
    }
}

// -------------------------------------------------------- Mouse Events

void UISurface::_mousePressed(ofMouseEventArgs &e){
    ofVec3f mouse = ofVec3f(e.x, e.y, 0.0);

    if (bEdit){
        // Editing the texture corners
        //
        for(int i = 0; i < 4; i++){
            if ( ofDist(e.x, e.y, textureCorners[i].x, textureCorners[i].y) <= 10 )
                selectedTextureCorner = i;
        }
    }
}

void UISurface::_mouseDragged(ofMouseEventArgs &e){
    ofVec3f mouse = ofVec3f(e.x, e.y,0);
    ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
    
    if (bEdit){
        // Drag texture corners
        //
        if (( selectedTextureCorner >= 0) && ( selectedTextureCorner < 4) ){
            
            if (getModifierShiftPressed()){
                // Corner Scale
                //
                ofVec2f center = textureCorners.getCentroid2D();
                
                float  opositCorner = (selectedTextureCorner - 2 < 0)? (4+selectedTextureCorner-2) : (selectedTextureCorner-2);
                ofVec2f toOpositCorner = center - textureCorners[opositCorner];
                
                float prevDist = mouseLast.distance( textureCorners[opositCorner] );
                float actualDist = mouse.distance( textureCorners[opositCorner] );
                
                float dif = actualDist/prevDist;
                
                move( textureCorners[opositCorner] + toOpositCorner * dif );
                scale(dif);
            } else if ( getModifierControlPressed() ){
                // Rotation
                //
                ofVec2f center = textureCorners.getCentroid2D();
                
                ofVec2f fromCenterTo = mouseLast - center;
                float prevAngle = -1.0*atan2f(fromCenterTo.x,fromCenterTo.y)+(PI/2);
                
                fromCenterTo = mouse - center;
                float actualAngle = -1.0*atan2f(fromCenterTo.x,fromCenterTo.y)+(PI/2);
                
                float dif = actualAngle-prevAngle;
                
                rotate(dif);
            } else if ( getModifierAltPressed() ){
                // Centered Scale
                //
                float prevDist = mouseLast.distance(textureCorners.getCentroid2D());
                float actualDist = mouse.distance(textureCorners.getCentroid2D());
                
                float dif = actualDist/prevDist;
                
                scale(dif);
            } else {
                // Deformation
                //
                textureCorners[selectedTextureCorner].x = ofGetMouseX();
                textureCorners[selectedTextureCorner].y = ofGetMouseY();
                
                doSurfaceToScreenMatrix();
            }
            bUpdate = true;
            
            // Drag all the surface
            //
        } else if ( textureCorners.inside(mouse) ){
            for (int i = 0; i < 4; i++){
                textureCorners[i] += mouse-mouseLast;
            }
            
            bUpdate= true;
            mouseLast = mouse;
        }
    }
    mouseLast = ofVec2f(e.x, e.y);
}

void UISurface::_mouseReleased(ofMouseEventArgs &e){
    if (bEdit){
        if (( selectedTextureCorner >= 0) && ( selectedTextureCorner < 4) ){
            doSurfaceToScreenMatrix();
            selectedTextureCorner = -1;
        }
    }
}