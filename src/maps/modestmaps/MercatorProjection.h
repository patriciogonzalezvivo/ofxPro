
#ifndef MERCATOR_PROJECTION
#define MERCATOR_PROJECTION

#include "AbstractProjection.h"
#include "Transformation.h"

class MercatorProjection : public AbstractProjection {
	
public:
	
	MercatorProjection(double _zoom=0): AbstractProjection(_zoom, Transformation()) { }
	MercatorProjection(double _zoom, Transformation t): AbstractProjection(_zoom,t) { }
	
	ofPoint rawProject(ofPoint point) {
        return ofPoint(point.x, log(tan(0.25 * PI + 0.5 * point.y)));
    }
    
    ofPoint rawUnproject(ofPoint point) {
        return ofPoint(point.x, 2.0 * atan( powf(M_E, point.y)) - 0.5 * PI);
    }

	
};

#endif