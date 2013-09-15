//
//  Page.h
//
//  Based on Eric Brumer article about Project Austin:
//
//      http://blogs.msdn.com/b/vcblog/archive/2012/09/27/10348494.aspx
//

#pragma once

#include "ofMain.h"

// Helper macro for a straight line F(x) that passes through {x1, y1} and {x2, y2}.
// We can't make this a template function (C++ doesn't let you have float literals
// as template parameters).
#define STRAIGHT_LINE(x1, y1, x2, y2, x) (((y2 - y1) / (x2 - x1)) * (x - x1) + y1)

struct vectorFace{
    ofPoint a,b,c,d;        //  TexCoord
    ofPoint A, B, C, D;     //  Vectex
};

static void drawPage( ofTexture &_page, ofRectangle &_on ){
    
    ofPoint normal = ofPoint(0,0,-1);
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP );
    
    mesh.addNormal( normal );
    mesh.addVertex( _on.getTopLeft() );
    mesh.addTexCoord( ofVec2f(0,_page.getHeight()) );
    
    mesh.addNormal( normal );
    mesh.addVertex( _on.getTopRight() );
    mesh.addTexCoord( ofVec2f(_page.getWidth(),_page.getHeight()) );
    
    mesh.addNormal( normal );
    mesh.addVertex( _on.getBottomLeft() );
    mesh.addTexCoord( ofVec2f(0,0) );
    
    mesh.addNormal( normal );
    mesh.addVertex( _on.getBottomRight() );
    mesh.addTexCoord( ofVec2f(_page.getWidth(),0) );
    
    _page.bind();
    mesh.draw();
    _page.unbind();
};

//--------------------------------------------------------------
//--------------------------------------------------------------
class Page : public ofRectangle {
public:
    
    Page();

    float   getTransition();
    void    setHandAt(ofPoint _hand, float lerpPct = 0.1);
    void    setNormHandAt(ofPoint _hand, float lerpPct = 0.1);
    
    void    update();
    void    draw(bool _bDebug = false);
    
    ofTexture *front;
    ofTexture *back;
    
    int     meshDefinition;
    
private:
    ofPoint getCurlPos(ofPoint pos, float _zOffset = 0 );

    ofMesh  mesh;
    ofPoint hand;
    
    float   theta;				// angle of right-cone
    float   ay;					// location on y axis of cone axis
    float   angle;				// rotation about y axis
    float   conicContribution;	// how much of (south tip cone == -1, cylinder == 0, north tip cone == 1)
    
    float   posNegOne;
    
    float   cylR;
    float   cylRadius;
    
    bool    bChange;
};
