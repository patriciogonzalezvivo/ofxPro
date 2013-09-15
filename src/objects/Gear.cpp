//
//  Gear.cpp
//  gears
//
//  Created by Patricio Gonzalez Vivo on 3/21/13.
//
//

#include "Gear.h"

Gear::Gear(){
    Gear(ofGetWidth()*0.5, ofGetHeight()*0.5, 6,1);
}

Gear::Gear(float _x, float _y, int _cogs, float _spin, GearData *_data) {
    x = _x;
    y = _y;
    cogs = _cogs;
    spin = _spin;
    
    linked = NULL;
    
    if ( _data == NULL){
        data = new GearData();
        data->speed = .5;
        data->hcog = 5;
        data->wcog = 5;
    } else {
        data = _data;
    }

    angle = 0.0;
    radius = cogs * data->wcog / PI;
}

bool Gear::inside(ofPoint _pos){
    return distance(_pos) <= radius;
}

bool Gear::touching( Gear *_gear){
    return distance( *_gear ) <= radius + data->hcog*2.0 +_gear->radius ;
}

void Gear::setData( GearData *_data ){
    data = _data;
}

GearData* Gear::getData(){
    if (linked != NULL)
        return linked->getData();
    else
        return data;
}

float Gear::getSpeed(){
    return abs(spin / 4.0 / cogs / getData()->wcog * getData()->speed) * ofGetFrameRate();
}

void Gear::setRadius(float _r) {
    
    // set the radius as close to r as possible
    //
    cogs = int(_r * PI / getData()->wcog);
    radius = cogs * getData()->wcog / PI;       //  the actual radius is determined by the number of cogs
    
    int n = cogs * 4.0;
    float rbase = radius - getData()->hcog*0.5; // inner radius
    
    border.clear();
    for (int i = 0; i < n; i++) {
        float t = ofMap(i, .5, n + .5, 0, TWO_PI);
        float u = (t * cogs) / TWO_PI;
        float d = u - floor(u) < .5 ? getData()->hcog : 0;

        border.addVertex(cos(t) * (rbase + d),
                         sin(t) * (rbase + d));
    }
    border.close();
}

float Gear::getRadius(){
    return radius;
}


void Gear::step() {
    
    // rotate a tiny step in spin direction
    //
    angle += TWO_PI * spin / 4.0 / cogs / getData()->wcog * getData()->speed;
    
}

void Gear::drawGear(bool _bFill) {
    ofStyle currentStyle = ofGetStyle();
    
    ofPushMatrix();
    ofTranslate(x, y);
    ofRotateZ(ofRadToDeg(angle));
    
    if (_bFill)ofFill();
    else ofNoFill();
    
    ofBeginShape();
    for (int i = 0; i < border.size(); i++) {
        ofVertex(border[i]);
    }
    ofEndShape(true);
    
    if (_bFill){
        ofPushStyle();
        ofFill();
        ofSetColor(currentStyle.bgColor);
        ofCircle(0, 0, 5);
        ofPopStyle();
    }

    ofNoFill();
    ofCircle(0, 0, 5);
    
    ofPushStyle();
    if ( radius > 60 ){
        
        float rBot = 30;
        float rTop = radius - 20;
        
        float aBot = 20;
        float aTop = aBot*0.25;
        
        int sides = (radius/30);
        ofPolyline hole;
        for (int i = 0 ; i < sides ; i++){
            
            hole.clear();
            hole.arc(ofPoint(0,0), rTop, rTop, (360/sides)*i + aTop, (360/sides)*(i+1) - aTop, true, 60);
            hole.arc(ofPoint(0,0), rBot, rBot, (360/sides)*(i+1) - aBot, (360/sides)*i + aBot, false, 20);
            hole.close();
            
            if (_bFill){
                ofPushStyle();
                ofSetColor(currentStyle.bgColor);
                ofFill();
                ofBeginShape();
                for (int i = 0; i < hole.size(); i++) {
                    ofVertex(hole[i]);
                }
                ofEndShape();
            }
            
            ofPushStyle();
            ofNoFill();
            ofSetLineWidth(0.5);
            hole.draw();
            ofPopStyle();
        }
        
    }
    ofPopStyle();
    
    ofPopMatrix();
}

