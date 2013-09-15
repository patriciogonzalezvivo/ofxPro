//
//  Grid.cpp
//  Basic
//
//  Created by Patricio Gonzalez Vivo on 8/31/13.
//
//

#include "Grid.h"

bool sortX(  ofRectangle * a, ofRectangle * b ) {
	return a->getRight() < b->getLeft();
}

bool sortY(  ofRectangle * a, ofRectangle * b ) {
	return a->getBottom() < b->getTop();
}

Grid::Grid(){
    color.set(1.0,1.0);
    thickness = 1;
    resolution = 0.0;
    margins = 0.5;
    bJump = false;
};

void Grid::setResolution(float _resolution, bool _calculateOffset){
    resolution = _resolution;
    fixResolutionAndOffSet();
    
    if (!_calculateOffset){
        offset *= 0;
    }
    makeGrid();
}

void Grid::setSkipList( vector<ofRectangle> &_skip){
    skip = _skip;
}

void Grid::forceOffSet( ofPoint _offset ){
    offset = _offset;
    makeGrid();
}

bool Grid::fixResolutionAndOffSet(){
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_LINES);
    
    if ( cols != width/resolution || realWidth != cols*resolution || offset.x != (width - realWidth)*0.5 ){
        if ( resolution > 1.0 ){
            cols = width/resolution;
            rows = height/resolution;
            
            realWidth = cols*resolution;
            realHeight = rows*resolution;
            
            offset.x = (width - realWidth)*0.5;
            offset.y = (height - realHeight)*0.5;
        }
        
        return true;
    } else {
        return false;
    }
};

void Grid::makeGrid(){
    
    if ( resolution > 1.0 ){
        
        ofPoint normal = ofPoint(0.0,0.0,1.0);
        
        //  Making COLUMNS
        //
        for(int i = 0; i <= cols; i++){
            
            ofFloatColor tmpColor = color;
            if (bJump && i%2 == 1 ){
                tmpColor.a = color.a*0.5;
            }
            
            ofPoint A = offset + ofPoint(x+i*resolution,y);
            ofPoint B = offset + ofPoint(x+i*resolution,y+realHeight);
            
            vector<ofRectangle*> obstacles = getVertColitionAreasFor(A, B);
            sort( obstacles.begin(), obstacles.end(), sortY );
            
            if (obstacles.size() == 0){
                mesh.addColor(tmpColor);
                mesh.addNormal(normal);
                mesh.addVertex( A );
                
                mesh.addColor(tmpColor);
                mesh.addNormal(normal);
                mesh.addVertex( B );
            } else {
                
                //  Draw colum Skiping obstacles
                //
                ofPoint lastPoint = A;
                for(int j = 0; j < obstacles.size(); j++){
                    mesh.addColor(tmpColor);
                    mesh.addNormal(normal);
                    mesh.addVertex( lastPoint );
                    
                    mesh.addColor(tmpColor);
                    mesh.addNormal(normal);
                    mesh.addVertex( ofPoint( A.x, obstacles[j]->getTop() - margins ));
                    
                    lastPoint.y = obstacles[j]->getBottom() + margins;
                }
                
                mesh.addColor(tmpColor);
                mesh.addNormal(normal);
                mesh.addVertex( lastPoint );
                
                mesh.addColor(tmpColor);
                mesh.addNormal(normal);
                mesh.addVertex( B );
            }
        }
        
        //  Makin ROWS
        //
        for(int i = 0; i <= rows; i++){
            
            ofFloatColor tmpColor = color;
            if (bJump && i%2 == 1 ){
                tmpColor.a = color.a*0.5;
            }
            
            ofPoint A = offset + ofPoint(x,y+i*resolution);
            ofPoint B = offset + ofPoint(x+realWidth,y+i*resolution);
            
            vector<ofRectangle*> obstacles = getHoriColitionAreasFor(A, B);
            sort( obstacles.begin(), obstacles.end(), sortX );
            
            if (obstacles.size() == 0){
                mesh.addColor(tmpColor);
                mesh.addNormal(normal);
                mesh.addVertex( A );
                
                mesh.addColor(tmpColor);
                mesh.addNormal(normal);
                mesh.addVertex( B );
            } else {
                
                //  Draw Rows Skiping obstacles
                //
                ofPoint lastPoint = A;
                for(int j = 0; j < obstacles.size(); j++){
                    mesh.addColor(tmpColor);
                    mesh.addNormal(normal);
                    mesh.addVertex( lastPoint );
                    
                    mesh.addColor(tmpColor);
                    mesh.addNormal(normal);
                    mesh.addVertex( ofPoint( obstacles[j]->getLeft() - margins, A.y ));
                    
                    lastPoint.x = obstacles[j]->getRight() + margins;
                }
                
                mesh.addColor(tmpColor);
                mesh.addNormal(normal);
                mesh.addVertex( lastPoint );
                
                mesh.addColor(tmpColor);
                mesh.addNormal(normal);
                mesh.addVertex( B );
            }
        }
    }
}

vector<ofRectangle*> Grid::getVertColitionAreasFor(ofPoint _top,ofPoint _bottom){
    vector<ofRectangle*> rta;

    for (int i = 0; i < skip.size(); i++) {
        if ( skip[i].getBottom() > _top.y && skip[i].getTop() < _bottom.y ){
            float x = (_top.x + _bottom.x)*0.5;
            if ( x > skip[i].getLeft() && x < skip[i].getRight() ){
                rta.push_back( &skip[i] );
            }
        }
    }
    
    return rta;
}

vector<ofRectangle*> Grid::getHoriColitionAreasFor(ofPoint _left,ofPoint _right){
    vector<ofRectangle*> rta;
    
    for (int i = 0; i < skip.size(); i++) {
        if ( skip[i].getRight() > _left.x && skip[i].getLeft() < _right.x ){
            float y = ( _left.y + _right.y)*0.5;
            if ( y < skip[i].getBottom() && y > skip[i].getTop() ){
                rta.push_back( &skip[i] );
            }
        }
    }
    
    return rta;
}

void Grid::print(){
    ofPushStyle();
    ofSetColor(color);
    ofSetLineWidth(thickness);
    vector<ofPoint> points = mesh.getVertices();
    
    for (int i = 0; i < points.size(); i+=2) {
        ofLine(points[i]*ofPoint(1,-1),points[i+1]*ofPoint(1,-1));
    }
    ofPopStyle();
};

void Grid::draw(){
    ofPushStyle();
    ofSetLineWidth(thickness);
    mesh.draw();
    ofPopStyle();
};