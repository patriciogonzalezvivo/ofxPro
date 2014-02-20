//
//  SyphonTextures.h
//
//  Created by Patricio Gonzalez Vivo on 5/18/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"

class SyphonTexture : public ofxSyphonClient, public ofBaseHasTexture {
public:
    SyphonTexture();
    
    void    setServer(string _server);
    void    setUseTexture(bool bUse);
    
    ofTexture &	getTextureReference();
    
    string sServer;
protected:
    
};
