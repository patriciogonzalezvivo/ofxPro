#pragma once

#include "ofMain.h"
#include "PingPong.h"
#include "DraggableRectangle.h"

class VectorField2D : public ofRectangle {
public:
    
    VectorField2D();
    virtual ~VectorField2D();
	
	void    setupField(float _scale);
    void    setupField(int _cols, int _rows, int _width, int _height);   // pass in internal dimensions and outer dimensions
	
    int     size();
    void    clear();
    ofPoint& operator [](int _index);
    
    int     getIndexFor(float _x, float _y);
    ofPoint	getForceFromPos(ofPoint pos);
    ofPoint	getForceFromPos(float xpos, float ypos);
    
    void    addOutwardCircle(float x, float y, float radius, float strength);
    void    addInwardCircle(float x, float y, float radius, float strength);
    void    addClockwiseCircle(float x, float y, float radius, float strength);
    void    addCounterClockwiseCircle(float x, float y, float radius, float strength);
    void    addVectorCircle(float x, float y, float vx, float vy, float radius, float strength);
    
    void    addRandom(float _pct);
    void    addNoise(float _pct = 1.0f, float _speed = 0.01, float _turbulence = 3, bool _signed = true);
    
    void    addTextureNormals(ofBaseHasTexture &_texture, float _pct = 1.0f);
    
    void    fade(float _fadeAmount);
    void    propagate();
    
    void    draw();

protected:
    virtual void loadCustomValues(ofxXmlSettings &_XML);
    virtual void saveCustomValues(ofxXmlSettings &_XML);
    ofPoint *buffer[2];
    
    int     cols, rows, nTotal;
    int     activeBuffer;
    float   scale;
};
