//
//  GridComposition.h
//
//  Created by Patricio Gonzalez Vivo on 8/30/13.
//
//

#pragma once

#include "GridBox.h"
#include "UIClass.h"

class GridComposition : public Grid, public UIClass {
public:
    
    GridComposition();
    ~GridComposition();
    
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
    ofxUIImageSampler*  colorSampler;
    ofxUIImageSampler*  crossColorSampler;
    ofxUIImageSampler*  refColorSampler;
    
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