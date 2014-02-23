
/*
 *  BrushedLine.h
 *
 *  Created by Marek Bereza on 28/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

/**
 * This class lets you draw a line with an image
 * and it'll stretch it across the line to give
 * you a nice smoothed stroke.
 *
 * see: http://answers.oreilly.com/topic/949-how-to-work-around-the-iphones-anti-aliasing-limitations/
 */
#pragma once

#include "ofMain.h"


class BrushedLine {
public:
    BrushedLine() {
        tex = NULL;
        size = 10;
        
        // texcoords are always the same.
        texCoords[0] = ofVec2f(0, 1);
        texCoords[1] = ofVec2f(0, 0);
        texCoords[2] = ofVec2f(0.5, 1);
        texCoords[3] = ofVec2f(0.5, 0);
        texCoords[4] = ofVec2f(0.5, 1);
        texCoords[5] = ofVec2f(0.5, 0);
        texCoords[6] = ofVec2f(1, 1);
        texCoords[7] = ofVec2f(1, 0);
        bBound = false;
        
    }
    void setup(ofTexture *_tex, float _size) {
        this->tex = _tex;
        this->size = _size;
    }
    
    void drawLine(ofPoint a, ofPoint b) {
        
        bool mustUnbind = false;
        if(!bBound) {
            bind();
            mustUnbind = true;
        }
        
        ofVec2f E = (b - a).getNormalized() * size;
        ofVec2f N = ofVec2f(-E.y, E.x);
        ofVec2f S = -N;
        ofVec2f NE = N + E;
        ofVec2f NW = N - E;
        ofVec2f SW = -NE;
        ofVec2f SE = -NW;
        coords[0] = (a + SW);
        coords[1] = (a + NW);
        coords[2] = (a + S);
        coords[3] = (a + N);
        coords[4] = (b + S);
        coords[5] = (b + N);
        coords[6] = (b + SE);
        coords[7] = (b + NE);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glVertexPointer(2, GL_FLOAT, 0, &coords[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        if(mustUnbind) unbind();
        
    }
    
    void drawPolyline(ofPolyline *line) {
        int numPoints = line->getVertices().size();
        vector < ofPoint > & pts = line->getVertices();
        
        bind();
        for(int i = 0; i+1 < numPoints; i++) {
            ofVec2f a = pts[i];
            ofVec2f b = pts[i+1];
            
            drawLine(pts[i], pts[i+1]);
            
            
        }
        unbind();
    }
    
    void drawLines(ofVec2f *points, int numPoints, bool close = false) {
        
        bind();
        for(int i = 0; i+1 < numPoints; i++) {
            ofVec2f a = points[i];
            ofVec2f b = points[i+1];
            
            drawLine(points[i], points[i+1]);
        }
        
        // close the loop if necessary.
        if(close) {
            drawLine(points[0], points[numPoints-1]);
        }
        unbind();
    }
    
    ofTexture   *tex;
    float       size;
    
protected:
    void bind() {
        if(!bBound && tex != NULL) {
            bBound = true;
//            ofDisableArbTex();
            tex->bind();
        }
    }
    
    void unbind() {
        if(bBound && tex != NULL) {
            tex->unbind();
//            ofEnableArbTex();
            bBound = false;
        }
    }

    ofVec2f texCoords[8];
    ofVec2f coords[8];
    bool    bBound;
};