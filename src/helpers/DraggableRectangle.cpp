//
//  DraggableRectangle.cpp
//

#include "DraggableRectangle.h"


bool DraggableRectangle::loadSettings(string _fileConfig, string _name, int _num ){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        name = _name;
        
        if (XML.pushTag( name , _num) ){
            fileFound = true;
            
            x = XML.getValue("x", 0);
            y = XML.getValue("y", 0);
            width = XML.getValue("width", ofGetScreenWidth());
            height = XML.getValue("height", ofGetScreenHeight());
            
            loadCustomValues(XML);
        }
    }
    
    this->set(x, y, width, height);
    
    return fileFound;
}

bool DraggableRectangle::saveSettings(string _fileConfig, string _name, int _num ){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        if ( XML.pushTag(_name,_num) ){
            
            XML.setValue("x", x);
            XML.setValue("y", y);
            XML.setValue("width", width);
            XML.setValue("height", height);
            saveCustomValues( XML );
            
            XML.popTag();
            fileFound = XML.saveFile();
        }
    }
    
    return fileFound;
}

void DraggableRectangle::draw(){
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(x, y);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
    ofSetColor(255,0,0,150);
    ofDrawBitmapString(name,15,-5);
    
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

bool DraggableRectangle::mouseDragged(ofPoint _mouse){
    ofPoint A = ofPoint(x,y);
    ofPoint B = ofPoint(x+width,y+height);
    
    if ( A.distance( _mouse) < 20 ){
        x += _mouse.x - x;
        y += _mouse.y - y;
        this->set(x,y, width, height);
        return true;
    } else if ( B.distance( _mouse) < 20 ){
        width += _mouse.x - x - width;
        height += _mouse.y - y - height;
        this->set(x,y, width, height);
        return true;
    } else {
        return false;
    }
}