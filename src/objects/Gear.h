//
//  Gear.h
//  gears
//
//  Created by Patricio Gonzalez Vivo on 3/21/13.
//
//

#ifndef GEAR
#define GEAR

#include "ofMain.h"

struct GearData {
    float   speed;
    float   hcog, wcog;
};

class Gear : public ofPoint {
public:
    
    Gear();
    Gear(float _x, float _y, int _cogs, float _spin, GearData *_data = NULL);

    void        setData( GearData *_data );
    void        setRadius(float _r);   // set the radius as close to r as possible
    float       getRadius();
    GearData*   getData();
    float       getSpeed();
    
    bool        touching( Gear *_gear);
    bool        inside(ofPoint _pos);

    Gear        *linked;
    float       angle;
    float       spin;
    
protected:
    void        step();
    void        drawGear(bool _bFill);

    ofPolyline  border;
    GearData    *data;
    
    float       radius;
    int         cogs;
};

#endif
