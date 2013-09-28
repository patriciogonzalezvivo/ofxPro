//
//  UIText.cpp
//  Thesis
//
//  Created by Patricio Gonzalez Vivo on 9/26/13.
//
//

#include "UIText.h"

UIText::UIText(){
    shape = NULL;
    fontDir = "none";
    bEnable = true;
    
    colorSampleImage.loadImage("GUI/defaultColorPalette.png");
}

void UIText::setupUI(){
    
//    gui->addTextInput("content", "_content_", OFX_UI_FONT_SMALL);
    
    gui->addSlider("font_size",4.0,80.0,&fontSize);
    gui->addSlider("font_scale",0.0,2.0,&fontScale);
    
    ofDirectory dir;
    dir.open( fontDir );
    if (dir.listDir()){
        fonts.clear();
        for(int i = 0; i < dir.getFiles().size(); i++ ){
            fonts.push_back( dir.getFiles()[i].getFileName() );
        }
        gui->addDropDownList("FONTS", fonts );
    }
    
    gui->addImageSampler("font_color", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
    gui->addSlider("font_alpha",0.0,1.0,&fontColor.a);
    
    vector<string> horizontal;
    horizontal.push_back("LEFT");
    horizontal.push_back("RIGHT");
    horizontal.push_back("JUSTIFIED");
    horizontal.push_back("CENTER");
    
    gui->addDropDownList("HORIZONTAL_ALIGN", horizontal);
    
    vector<string> vertical;
    vertical.push_back("TOP");
    vertical.push_back("BOTTOM");
    vertical.push_back("MIDDLE");
    
    gui->addDropDownList("VERTICAL_ALIGN", vertical);
    
    vector<string> type;
    type.push_back("ARC");
    type.push_back("BLOCK");
    gui->addDropDownList("TYPE", type);
    gui->addSlider("arc_angle", -PI, PI, &arcAngle);
}

void UIText::setText(string _text){
    text =_text;
    if(shape!=NULL){
        shape->setText(text);
    }
}

string UIText::getText(){
    return text;
}

void UIText::guiEvent(ofxUIEventArgs &e){
    if (gui != NULL || shape != NULL){
        string name = e.widget->getName();
        
        if( name == "BLOCK" ){
            shapeType = TEXT_SHAPE_BLOCK;
            
            if ( shape != NULL ){
                delete shape;
            }
            
            shape = new TextBlock();
            ((TextBlock*)(shape))->setWrapping(false);
            shape->set(*this);
            shape->loadFont( fontDir+"/"+fontName, fontSize );
            shape->setScale( fontScale );
            shape->setAlignment( textAlignH , textAlignV );
            shape->setText(text);
            
            ofxUIWidget *angWidget =  gui->getWidget("arc_angle");
            angWidget->setVisible(false);
            gui->autoSizeToFitWidgets();
            
        } else if( name == "ARC"){
            shapeType = TEXT_SHAPE_ARC;
            
            if ( shape != NULL ){
                delete shape;
            }
            
            shape = new TextArc();
            shape->set(*this);
            shape->loadFont( fontDir+"/"+fontName, fontSize );
            shape->setScale( fontScale );
            shape->setAlignment( textAlignH , textAlignV );
            shape->setText(text);
            ((TextArc*)(shape))->setRadius(arcAngle);
            
            ofxUIWidget *angWidget =  gui->getWidget("arc_angle");
            angWidget->setVisible(true);
            gui->autoSizeToFitWidgets();
            
        } else if( name == "font_size" || name == "font_scale"){
            shape->loadFont( fontDir+"/"+fontName, fontSize );
            shape->setScale( fontScale );
        } else if ( name == "font_color"){
            ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
            float alpha = fontColor.a;
            fontColor = sampler->getColor();
            fontColor.a = alpha;
        } else if ( name == "content"){
            ofxUITextInput *widget = (ofxUITextInput *)e.widget;
            text = widget->getTextString();
            
            shape->setText(text);
            widget->setTextString(text);
        } else if ( name == "LEFT" ){
            textAlignH = TEXT_ALIGN_LEFT;
            shape->setAlignment( textAlignH , textAlignV );
        } else if ( name == "RIGHT" ){
            textAlignH = TEXT_ALIGN_RIGHT;
            shape->setAlignment( textAlignH , textAlignV );
        } else if ( name == "JUSTIFIED" ){
            textAlignH = TEXT_ALIGN_JUSTIFIED;
            shape->setAlignment( textAlignH , textAlignV );
        } else if ( name == "CENTER" ){
            textAlignH = TEXT_ALIGN_CENTER;
            shape->setAlignment( textAlignH , textAlignV );
        } else if ( name == "TOP" ){
            textAlignV = TEXT_ALIGN_TOP;
            shape->setAlignment( textAlignH , textAlignV );
        } else if ( name == "BOTTOM" ){
            textAlignV = TEXT_ALIGN_BOTTOM;
            shape->setAlignment( textAlignH , textAlignV );
        } else if ( name == "MIDDLE" ){
            textAlignV = TEXT_ALIGN_MIDDLE;
            shape->setAlignment( textAlignH , textAlignV );
        } else if ( name == "ENABLE" || name == "FONTS" || name == "VERTICAL_ALIGN" || name == "HORIZONTAL_ALIGN"){
            
        } else if ( name == "arc_angle"){
            if (shapeType == TEXT_SHAPE_ARC) {
                ((TextArc*)(shape))->setAngle(arcAngle);
            }
        } else {
            for(int i = 0; i < fonts.size(); i++ ){
                if (name == fonts[i]){
                    fontName = name;
                    shape->loadFont( fontDir+fontName, fontSize );
                    break;
                }
            }
        }
    }
}

void UIText::setFontsDir( string _dir ){
    fontDir = _dir;
    if ( shape != NULL ){
        
        ofDirectory dir;
        dir.open( fontDir );
        if (dir.listDir()){
            fonts.clear();
            for(int i = 0; i < dir.getFiles().size(); i++ ){
                fonts.push_back( dir.getFiles()[i].getFileName() );
            }
            
            shape->loadFont( fontDir+"/"+fontName, fontSize );
        }
    }
}

void UIText::draw(){
    if (bEnable){
        if ( shape != NULL ){
            ofPushStyle();
            ofSetColor(fontColor);
            shape->draw();
            ofPopStyle();
        }
    }
}

void UIText::debugDraw(){
    DraggableRectangle::draw();
}

void UIText::loadCustomValues(ofxXmlSettings &_XML){

    shapeType = TextShapeType(_XML.getValue("shapeType", 0));
    
    if ( shape != NULL ){
        delete shape;
    }
    if ( shapeType == TEXT_SHAPE_BLOCK ) {
        shape = new TextBlock();
        ((TextBlock*)(shape))->setWrapping(false);
    } else {
        shape = new TextArc();
    }
    
    shape->set(*this);
    
    if (fontDir == "none"){
        fontDir = _XML.getValue("font:dir", "path/");
    }
    
    fontName = _XML.getValue("font:name", "non.ttf");
    fontSize  = _XML.getValue("font:size", 12);
    fontScale = _XML.getValue("font:scale", 1.0);
    shape->loadFont( fontDir+"/"+fontName, fontSize );
    shape->setScale( fontScale );
    
    textAlignV = TextAlignVertical(_XML.getValue( "align:ver", 0));
    textAlignH = TextAlignHorizontal(_XML.getValue( "align:hor", 0));
    
    shape->setAlignment( textAlignH , textAlignV );
    
    text = _XML.getValue("text","default_text");
    shape->setText( text );
    
    arcAngle = _XML.getValue("arc:angle", 0);
    
    if ( shapeType == TEXT_SHAPE_ARC){
        ((TextArc*)(shape))->setAngle(arcAngle);
    }
}

void UIText::saveCustomValues(ofxXmlSettings &_XML){
    _XML.setValue("shapeType", shapeType);
    _XML.setValue("font:name",fontName);
    _XML.setValue("font:size",fontSize);
    _XML.setValue("font:scale",fontScale);
    _XML.setValue("font:dir",fontDir);
    _XML.setValue("text",text);
    _XML.setValue("align:ver",textAlignV);
    _XML.setValue("align:hor",textAlignH);
    _XML.setValue("arc:angle", arcAngle);
}

void UIText::mouseDragged(ofPoint _mouse){
    if (bEnable){
        ofPoint A = ofPoint(x,y);
        ofPoint B = ofPoint(x+width,y+height);
        
        if ( A.distance( _mouse) < 20 ){
            x += _mouse.x - x;
            y += _mouse.y - y;
            
    
            if ( shape != NULL ){
                shape->set( *this);
                shape->setText(text);
            }
            
        } else if ( B.distance( _mouse) < 20 ){
            width += _mouse.x - x - width;
            height += _mouse.y - y - height;
            
            if ( shape != NULL ){
                shape->set( *this);
                shape->setText(text);
            }
        }
    }
}