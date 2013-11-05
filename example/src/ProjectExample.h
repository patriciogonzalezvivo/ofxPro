//
//  ProjectExample.h
//
//  Created by Patricio Gonzalez Vivo on 9/6/13.
//
//

#pragma once

#include "UI2DGrid.h"
#include "UI3DGrid.h"
#include "UIProject.h"

class ProjectExample : public UI3DProject {
public:

    string getSystemName(){
		return "ProjectExample";
	}

    void selfSetupGuis();
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
    void selfDrawOverlay();

	void selfEnd();
    void selfExit();

    void selfKeyPressed(ofKeyEventArgs & args);
    void selfKeyReleased(ofKeyEventArgs & args);

    void selfMouseDragged(ofMouseEventArgs& data);
    void selfMouseMoved(ofMouseEventArgs& data);
    void selfMousePressed(ofMouseEventArgs& data);
    void selfMouseReleased(ofMouseEventArgs& data);

protected:
    UI3DGrid grid;
    UI2DGrid ruler;
};
