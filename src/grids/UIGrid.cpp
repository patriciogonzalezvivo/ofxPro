//
//  UIGrid.cpp
//  Basic
//
//  Created by Patricio Gonzalez Vivo on 8/31/13.
//
//

#include "UIGrid.h"

//  1cm     =   28.3 px.
//  1inch   =   2.54 cm
//  1cm     =   0.393701 inch

float pixelToCm = 28.3f;
float cmToInches = 2.54;
float inchesToCm = 0.393701;
float pixelToInches = pixelToCm*cmToInches;

UIGrid::UIGrid(){
    
    refRow = 0;
    refCol = 0;
    bJump = false;
    
    colorSampler = NULL;
    
    subLines.bJump = false;
    cross.bJump = true;
    boxes.bJump = false;
    
    //  Data
    //
    font.loadFont("GUI/NewMedia Fett.ttf",12,true, true, true,false);
    colorSampleImage.loadImage("GUI/defaultColorPalette.png");
    
    gui = NULL;
}

UIGrid::~UIGrid(){

    UIClass::~UIClass();
    
    if (colorSampler != NULL){
        delete colorSampler;
        colorSampler = NULL;
    }
    
    if ( crossColorSampler != NULL ){
        delete crossColorSampler;
        crossColorSampler = NULL;
    }
    
    if ( refColorSampler != NULL ){
        delete refColorSampler;
        refColorSampler = NULL;
    }
    
}

void UIGrid::setResolutionToCm(){
    
    subLineResolution = 10.0;
    setResolution(pixelToCm*subLineResolution);
}

void UIGrid::setResolutionToInches(){
    
    setResolution(pixelToInches);
    subLineResolution = 8.0;
}

void UIGrid::setupUI(){
    
    gui->addLabel("Lines");
    colorSampler =  gui->addImageSampler("Lines_Color", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
    gui->addSlider("Lines_Alpha", 0.0, 1.0, &color.a);
    //        gui->addSlider("SubLine_Resolution", 0.1,100.0, &subLineResolution);
    gui->addSlider("SubLine_Alpha", 0.0, 1.0, &subLineAlpha);
    
    gui->addLabel("Cross");
    gui->addSlider("Cross_Resolution", 0.1, 100.0, &crossResolution);
    gui->addSlider("Cross_Size", 0.0, 1.0, &cross.crossSize);
    
    crossColorSampler =  gui->addImageSampler("Cross_Color", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
    
    gui->addSlider("Cross_Alpha", 0.0, 1.0, &cross.color.a);
    gui->addLabel("References");
    gui->addSlider("Reference_Margin", 0.0, 0.5, &marginScale);
    gui->addSlider("Reference_Min", -1.0, 0.1, &refNumOffset);
    gui->addSlider("Col_Reference", 0.0, 1.0, &refCol);
    gui->addSlider("Row_Reference", 0.0, 1.0, &refRow);
    refColorSampler =  gui->addImageSampler("Reference_Color", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
    gui->addSlider("Reference_Color_Alpha", 0.0, 1.0, &fontColor.a);
    
}

void UIGrid::guiEvent(ofxUIEventArgs &e){
    
    if (gui != NULL){
        string name = e.widget->getName();
        
        cout << "HERE " << name << endl;
        
        if( name == "Cross_Color"){
            ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
            float alpha = cross.color.a;
            cross.color = sampler->getColor();
            cross.color.a = alpha;
        } else if( name == "Lines_Color"){
            ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
            float alpha = color.a;
            color = sampler->getColor();
            color.a = alpha;
            subLines.color = color;
            subLines.color.a = color.a*subLineAlpha;
        } else if ( name == "SubLine_Alpha"){
            subLines.color.a = color.a*subLineAlpha;
        } else if( name == "Reference_Color"){
            ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
            float alpha = fontColor.a;
            fontColor = sampler->getColor();
            fontColor.a = alpha;
        } else if (name == "ENABLE"){
            ofxUIToggle *t = (ofxUIToggle *) e.widget;
            bEnable = t->getValue();
        }
        
        makeGrid();
    }
}

void UIGrid::makeGrid(){
    margins = marginScale*resolution;
    
    skip.clear();
    for(int i = 0; i <= cols-1; i++){
        string number = ofToString(i+(int)(cols*refNumOffset));
        ofRectangle box = font.getStringBoundingBox( number ,0,0);
        ofPoint pos = offset + ofPoint(x+i*resolution+resolution*0.5,y+(rows*refCol)*resolution) - box.getCenter();
        pos *= ofPoint(1,-1);
        skip.push_back( ofRectangle(pos, box.width, box.height) );
    }
    
    int A = char('A');
    for(int i = 0; i <= rows-1; i++){
        string letter = ofToString(char(A+i));
        ofRectangle box = font.getStringBoundingBox( letter , 0,0);
        ofPoint pos = offset + ofPoint(x+(cols*refRow)*resolution,y+i*resolution+resolution*0.5) - box.getCenter();
        pos *= ofPoint(1,-1);
        skip.push_back( ofRectangle(pos, box.width, box.height) );
    }
    Grid::fixResolutionAndOffSet();
    Grid::makeGrid();
    
    subLines.setFromCenter(0,0,realWidth,realHeight);
    subLines.margins = margins;
    subLines.setSkipList(skip);
    subLines.setResolution(resolution*(1.0/ceil(subLineResolution)),false );
    subLines.makeGrid();
    
    cross.setFromCenter(0,0,realWidth,realHeight);
    cross.margins = margins;
    cross.setSkipList(skip);
    cross.setResolution(resolution*(1.0/ceil(crossResolution)),false);
    cross.makeGrid();
};

void UIGrid::print(int _layerNumber){
    
    if (bEnable){
        if (_layerNumber == -1 || _layerNumber == 0){
            cross.print();
        }
        
        if (_layerNumber == -1 || _layerNumber == 1){
            ofTranslate(0, 0, 0.1);
            subLines.print();
        }
        
        if (_layerNumber == -1 || _layerNumber == 2){
            ofTranslate(0, 0, 0.1);
            Grid::print();
        }
        
        if (_layerNumber == -1 || _layerNumber == 3){
            ofTranslate(0, 0, 0.1);
            ofSetColor(color);
            for(int i = 0; i <= cols-1; i++){
                string number = ofToString(i+(int)(cols*refNumOffset));
                ofRectangle box = font.getStringBoundingBox( number ,0,0);
                ofPoint pos = offset + ofPoint(x+i*resolution+resolution*0.5,y+(rows*refCol)*resolution) - box.getCenter();
                font.drawStringAsShapes(number,pos.x,pos.y);
            }
            
            int A = char('A');
            for(int i = 0; i <= rows-1; i++){
                string letter = ofToString(char(A+i));
                ofRectangle box = font.getStringBoundingBox( letter , 0,0);
                ofPoint pos = offset + ofPoint(x+(cols*refRow)*resolution,y+i*resolution+resolution*0.5) - box.getCenter();
                font.drawStringAsShapes(letter,pos.x, pos.y);
            }
        }

    }
}

void UIGrid::draw(){
    
    if (bEnable){
        cross.draw();
        
        ofTranslate(0, 0, 0.1);
        subLines.draw();
        
        ofTranslate(0, 0, 0.1);
        Grid::draw();
        
        ofTranslate(0, 0, 0.1);
        ofSetColor(fontColor);
        for(int i = 0; i <= cols-1; i++){
            string number = ofToString(i+(int)(cols*refNumOffset));
            ofRectangle box = font.getStringBoundingBox( number ,0,0);
            ofPoint pos = offset + ofPoint(x+i*resolution+resolution*0.5,y+(rows*refCol)*resolution) - box.getCenter();
            pos *= ofPoint(1,-1);
            font.drawStringAsShapes(number,pos.x,pos.y);
        }
        
        int A = char('A');
        for(int i = 0; i <= rows-1; i++){
            string letter = ofToString(char(A+i));
            ofRectangle box = font.getStringBoundingBox( letter , 0,0);
            ofPoint pos = offset + ofPoint(x+(cols*refRow)*resolution,y+i*resolution+resolution*0.5) - box.getCenter();
            pos *= ofPoint(1,-1);
            font.drawStringAsShapes(letter,pos.x, pos.y);
        }
        
        ofTranslate(0, 0, 0.1);
        boxes.draw();
    }
}