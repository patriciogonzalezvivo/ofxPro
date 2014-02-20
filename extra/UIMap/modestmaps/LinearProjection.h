#ifndef LINEAR_PROJECTION
#define LINEAR_PROJECTION

#include "AbstractProjection.h"
#include "Transformation.h"

class LinearProjection : public AbstractProjection {
	
public:

	LinearProjection(double _zoom=0): AbstractProjection(_zoom, Transformation()) { }
	LinearProjection(double _zoom, Transformation t): AbstractProjection(_zoom,t) { }

	ofPoint rawProject(ofPoint point);
	ofPoint rawUnproject(ofPoint point);

};

#endif
