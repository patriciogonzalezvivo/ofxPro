//
//  DraggableRectangle.cpp
//

#include "DraggableRectangle.h"

bool DraggableRectangle::loadSettings(string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        fileFound = true;
        
        
        x = XML.getValue("rect:x", 0);
        y = XML.getValue("rect:y", 0);
        width = XML.getValue("rect:width", ofGetScreenWidth());
        height = XML.getValue("rect:height", ofGetScreenHeight());
        name = XML.getValue("rect:name", " ");
    }
    
    this->set(x, y, width, height);
    
    return fileFound;
}

bool DraggableRectangle::loadSettings(int _num, string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        if (XML.pushTag("rect", _num) ){
            fileFound = true;
            x = XML.getValue("x", 0);
            y = XML.getValue("y", 0);
            width = XML.getValue("width", ofGetScreenWidth());
            height = XML.getValue("height", ofGetScreenHeight());
            name = XML.getValue("name", " ");
        }
    }
    
    this->set(x, y, width, height);
    
    return fileFound;
}

bool DraggableRectangle::loadSettings(string _name, string _fileConfig){
    bool fileFound = false;
    name = _name;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        int total = XML.getNumTags("rect");
        for (int i = 0; i < total; i++){
        
            if ( XML.pushTag("rect", i) ){
                if ( name == XML.getValue("name", " ") ){
                    fileFound = true;
                    x = XML.getValue("x", 0);
                    y = XML.getValue("y", 0);
                    width = XML.getValue("width", ofGetScreenWidth());
                    height = XML.getValue("height", ofGetScreenHeight());
                    break;
                }
                XML.popTag();
            }
        }
    }
    
    this->set(x, y, width, height);
    
    return fileFound;
}

bool DraggableRectangle::saveSettings(string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        int total = XML.getNumTags("rect");
        
        if (total == 1){
            if ( XML.pushTag("rect") ){
                XML.setValue("x", x);
                XML.setValue("y", y);
                XML.setValue("width", width);
                XML.setValue("height", height);
                XML.setValue("name", name);
                XML.popTag();
                fileFound = XML.saveFile();
            }
        } else {
            saveSettings(name,_fileConfig);
        }
    }
    
    return fileFound;
}

bool DraggableRectangle::saveSettings(int _num, string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        if ( XML.pushTag("rect",_num) ){
            XML.setValue("x", x);
            XML.setValue("y", y);
            XML.setValue("width", width);
            XML.setValue("height", height);
            XML.setValue("name", name);
            XML.popTag();
            fileFound = XML.saveFile();
        }
    }
    
    return fileFound;
}

bool DraggableRectangle::saveSettings(string _name, string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        int total = XML.getNumTags("rect");
        
        for (int i = 0; i < total; i++){
            if ( XML.pushTag("rect", i) ){
                if ( name == XML.getValue("name", " ") ){
                    XML.setValue("x", x);
                    XML.setValue("y", y);
                    XML.setValue("width", width);
                    XML.setValue("height", height);
                    XML.popTag();
                    fileFound = XML.saveFile();
                    return fileFound;
                }
                XML.popTag();
            }
        }
    }
    
    return fileFound;
}

void DraggableRectangle::draw(){
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(x, y);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
    ofSetColor( 0 );
    ofDrawBitmapString(name,15,-15);
    
    ofNoFill();
    ofSetColor(255,0,0,150);
    ofRect(0,0,width,height);
    
    ofFill();
    ofSetColor(255,0,0,100);
    
    drawBrakets(ofRectangle(-7,-7,14,14), 5, 2 );
    drawBrakets(ofRectangle(width-7,height-7,14,14), 5, 2 );
    
    ofPopMatrix();
    ofPopStyle();
    
}

void DraggableRectangle::drawBrakets(ofRectangle _rect, float size, float margin){
    
    ofPushStyle();
    ofSetLineWidth(2);
    
    ofPushMatrix();
    ofTranslate(_rect.getTopLeft() + ofPoint(-margin,-margin));
    ofLine(0, 0, size, 0);
    ofLine(0, 0, 0, size);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(_rect.getTopRight() + ofPoint(margin,-margin));
    ofLine(0, 0, -size, 0);
    ofLine(0, 0, 0, size);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(_rect.getBottomLeft() + ofPoint(-margin,margin));
    ofLine(0, 0, size, 0);
    ofLine(0, 0, 0, -size);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(_rect.getBottomRight() + ofPoint(margin,margin));
    ofLine(0, 0, -size, 0);
    ofLine(0, 0, 0, -size);
    ofPopMatrix();
    
    ofPopStyle();
}

void DraggableRectangle::mouseDragged(ofPoint _mouse){
    ofPoint A = ofPoint(x,y);
    ofPoint B = ofPoint(x+width,y+height);
    
    if ( A.distance( _mouse) < 20 ){
        x += _mouse.x - x;
        y += _mouse.y - y;
        this->set(x,y, width, height);
    }
    
    if ( B.distance( _mouse) < 20 ){
        width += _mouse.x - x - width;
        height += _mouse.y - y - height;
        this->set(x,y, width, height);
    }
}