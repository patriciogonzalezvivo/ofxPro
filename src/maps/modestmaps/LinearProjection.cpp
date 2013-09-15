#include "LinearProjection.h"

ofPoint LinearProjection::rawProject(ofPoint point) {
	return ofPoint(point.x, point.y);
}

ofPoint LinearProjection::rawUnproject(ofPoint point) {
	return ofPoint(point.x, point.y);
}
