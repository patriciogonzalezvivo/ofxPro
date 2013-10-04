//
//  ShellExample.h
//
//  Created by Patricio Gonzalez Vivo on 9/6/13.
//
//

#pragma once

#include "UIProject.h"

#include "UISuperBackground.h"
#include "UIGrid.h"
#include "UIFog.h"
#include "UIShader.h"

class ShellExample : public UIProject {
public:

    string getSystemName(){
		return "shell";
	}

    void selfSetupGui();
    void selfGuiEvent(ofxUIEventArgs &e);

    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);

    void selfSetupRenderGui();
    void guiRenderEvent(ofxUIEventArgs &e);

    void selfSetup();

    void selfBegin();
	void selfPresetLoaded(string presetPath);
    void selfSceneTransformation();

    void selfUpdate();

    void selfDraw();
	void selfDrawDebug();
	void selfDrawBackground();

    void selfPostDraw();

	void selfEnd();
    void selfExit();

    void selfKeyPressed(ofKeyEventArgs & args);
    void selfKeyReleased(ofKeyEventArgs & args);

    void selfMouseDragged(ofMouseEventArgs& data);
    void selfMouseMoved(ofMouseEventArgs& data);
    void selfMousePressed(ofMouseEventArgs& data);
    void selfMouseReleased(ofMouseEventArgs& data);

//    void setupBackground();
protected:

    UIGrid          grid;
    UIFog           fog;
    UIShader        shader;

    //  Shell
    //
    void    createSkin( int _width );
    ofPolyline  freqArc( vector<float> &_vector, const ofPoint &_center, float _angleBegin, float _angleEnd, float _minRad , bool _bSmooth = false);
    ofVboMesh   mesh;
    vector< vector<float> > averages;
    vector< ofPoint >       points;
    ofPoint     offSet;
    float       shellBuffer;
    float       shellNoise;
    float       shellRadio;
    float       shellTrans;
    float       shellRot;
    int         bufferSize;


};
