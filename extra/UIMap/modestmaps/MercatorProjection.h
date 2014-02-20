
#ifndef MERCATOR_PROJECTION
#define MERCATOR_PROJECTION

#include "AbstractProjection.h"
#include "Transformation.h"

class MercatorProjection : public AbstractProjection {
public:
    static ProjectionRef create(float zoom = 0){
        return ProjectionRef( new MercatorProjection( zoom ) );
    }
    
    static ProjectionRef create(float zoom, Transformation t){
        return ProjectionRef( new MercatorProjection( zoom, t ) );
    }
    
    static ProjectionRef createWebMercator(){
        Transformation t = Transformation::deriveTransformation(-M_PI,  M_PI, 0, 0,
                                                                 M_PI,  M_PI, 1, 0,
                                                                -M_PI, -M_PI, 0, 1 );
        return ProjectionRef( new MercatorProjection( 0, t) );
    }
    
	ofPoint rawProject(const ofPoint &point){
        return ofPoint(point.x, log(tan(0.25 * PI + 0.5 * point.y)));
    }
    
    ofPoint rawUnproject(const ofPoint &point) {
        return ofPoint(point.x, 2.0 * atan( powf(M_E, point.y)) - 0.5 * PI);
    }
    
//private:
	
	MercatorProjection(float _zoom=0): AbstractProjection(_zoom, Transformation()) { }
	MercatorProjection(float _zoom, Transformation t): AbstractProjection(_zoom,t) { }
};

#endif