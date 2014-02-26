//
//  Page.h
//
//  Based on Eric Brumer article about Project Austin:
//
//      http://blogs.msdn.com/b/vcblog/archive/2012/09/27/10348494.aspx
//

#pragma once

#include "ofMain.h"

static void drawPage(ofTexture &_page,ofRectangle &_on){
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
class Page : public ofRectangle {
public:
    
    Page();

    void    setHandAt(ofPoint _hand, float _lerpPct = -1.0);
    void    setNormHandAt(ofPoint _hand, float _lerpPct = -1.0);
    
    float   getTransition();
    ofMesh& getMesh();
    
    void    reset();
    void    update();
    void    forceUpdate(){bChange=true;};
    void    draw();
    
    ofTexture *front;
    ofTexture *back;
    
    float   coneAxisDist;
    
    float   handLerpPct;
    int     meshDefinition;
    
    bool    bFlipHorizontal;
    bool    bFlipVertical;
    bool    bLeftRight;
    bool    bDebug;
    
protected:
    ofPoint getCurlPos(ofPoint pos, float _zOffset = 0);
    void    computeConeValues();
    void    updateMesh();

    ofMesh  mesh;
    ofPoint hand;
    
    float   axis;               // location on y axis of cone axis
    float   theta;				// angle of right-cone
    float   angle;				// rotation about y axis
    float   conicContribution;	// how much of (south tip cone == -1, cylinder == 0, north tip cone == 1)
    float   cylRadius;
    
    float   posNegOne;
    
    bool    bChange;
};
