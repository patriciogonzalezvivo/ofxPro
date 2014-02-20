//
//  UISyphon.cpp
//
//  Created by Patricio Gonzalez Vivo on 2/18/14.
//
//

#include "UISyphon.h"

UISyphon::UISyphon(){
    inPut.setup();
    outPut.setName(ofGetTimestampString("%H-%M-%S"));
}

void UISyphon::setupUI(){
    gui->addLabel("IN");
    gui->addLabel("OUT");
}

void UISyphon::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
}