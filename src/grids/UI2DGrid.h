//
//  UI2DGrid.h
//
//  Created by Patricio Gonzalez Vivo on 8/30/13.
//
//

#pragma once

#include "GridBox.h"
#include "UIClass.h"

class UI2DGrid : public Grid, public UIClass {
public:
    
    UI2DGrid();
    
    void    setResolutionToCm();
    void    setResolutionToInches();
    
    void    setupUI();
    string  getClassName(){ return "GRID"; }
    
    void    makeGrid();
    
    void    print(int _layerNumber = -1 );
    void    draw();
    
protected:
    
    //  Gui
    //
    void                guiEvent(ofxUIEventArgs &e);
    
    ofImage             colorSampleImage;
    
    Grid                subLines;
    GridCross           cross;
    GridBox             boxes;
    
    ofTrueTypeFont      font;
    ofFloatColor        fontColor;
    float               refRow, refCol, refNumOffset;
    
    float               marginScale;
    float               crossResolution;
    float               subLineResolution, subLineAlpha;
};