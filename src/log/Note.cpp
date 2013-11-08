//
//  Note.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/7/13.
//
//

#include "Note.h"
#include "ofxXmlSettings.h"

void Note::load(string _file){
    lines.clear();
    
    ofxXmlSettings XML;
    if (XML.loadFile(_file)){
        name = XML.getValue("name","none");
        
        int totalLines = XML.getNumTags("line");
        
        for (int i = 0; i < totalLines; i++) {
            XML.pushTag("line",i);
            int totalPnts = XML.getNumTags("pnt");
            ofPolyline newLine;
            for (int j = 0; j < totalPnts; j++){
                ofPoint newPoint;
                newPoint.x = XML.getValue("pnt:x", 0.0, j);
                newPoint.y = XML.getValue("pnt:y", 0.0, j);
                newPoint.z = XML.getValue("pnt:z", 0.0, j);
                newLine.addVertex(newPoint);
            }
            lines.push_back(newLine);
            XML.popTag();
        }
    }
    
    bEnable = true;
}

void Note::save(string _file){
    ofxXmlSettings XML;
    
    XML.setValue("name",name);
    int totalLines = lines.size();
    for(int i = 0; i < totalLines; i++){
        XML.addTag("line");
    }
    
    for(int i = 0; i < totalLines; i++){
        XML.pushTag("line",i);
        
        int totalPnts = lines[i].size();
        for(int j = 0; j < totalPnts; j++){
            XML.addTag("pnt");
        }
        
        for(int j = 0; j < totalPnts; j++){
            XML.setValue("pnt:x", lines[i][j].x , j);
            XML.setValue("pnt:y", lines[i][j].y , j);
            XML.setValue("pnt:z", lines[i][j].z , j);
        }
        
        XML.popTag();
    }
    
    XML.saveFile(_file);
}

void Note::addPoint( ofPoint _point ){
    if(!bPenDown){
        bPenDown = true;
    }
    actualLine.addVertex(_point);
}

void Note::penUp(){
    if(bPenDown){
        if(actualLine.size()){
            lines.push_back(actualLine);
            actualLine.clear();
        }
        bPenDown = false;
    }
}

void Note::clearAll(){
    actualLine.clear();
    lines.clear();
}

void Note::clearLast(){
    if(bPenDown){
        actualLine.clear();
        bPenDown = false;
    } else {
        lines.erase(lines.end());
    }
}

void Note::draw(){
    if(bEnable){
        for(int i = 0; i < lines.size(); i++){
            lines[i].draw();
        }
        actualLine.draw();
    }
}