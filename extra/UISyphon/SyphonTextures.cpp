//
//  SyphonTextures.cpp
//  10-Node
//
//  Created by Patricio Gonzalez Vivo on 5/18/13.
//
//

#include "SyphonTextures.h"

SyphonTexture::SyphonTexture(){
    bSetup = false;
}

void SyphonTexture::setServer(string _server){
    sServer = _server;
    
    vector<string> serverAddress = ofSplitString(_server, "@");
    if (serverAddress.size() == 2){
        setApplicationName(serverAddress[1]);
        setServerName(serverAddress[0]);
    } else if (serverAddress.size() == 1){
        setApplicationName(_server);
    }
}

void SyphonTexture::setUseTexture(bool bUse){
//	bUseTexture = bUse;
}

ofTexture& SyphonTexture::getTextureReference(){
    this->bind();
    this->unbind();
    
    return mTex;
}

