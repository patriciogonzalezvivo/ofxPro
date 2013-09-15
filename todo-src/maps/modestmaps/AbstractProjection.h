#ifndef ABSTRACT_PROJECTION
#define ABSTRACT_PROJECTION

#include "Transformation.h"
#include "Coordinate.h"
#include "Location.h"

class AbstractProjection {

public:

	double zoom;
	Transformation transformation;

	AbstractProjection(double _zoom): zoom(_zoom), transformation(Transformation()) { }
	AbstractProjection(double _zoom, Transformation _t): zoom(_zoom), transformation(_t) { }

	virtual ofPoint rawProject(ofPoint point)=0;
	virtual ofPoint rawUnproject(ofPoint point)=0;

	ofPoint project(ofPoint point) {
		return transformation.transform(rawProject(point));
	}

	ofPoint unproject(ofPoint point) {
		return rawUnproject(transformation.untransform(point));
	}

	Coordinate locationCoordinate(Location location) {
		ofPoint point = project(ofPoint(DEG_TO_RAD * location.lon, DEG_TO_RAD * location.lat));
		return Coordinate(point.y, point.x, zoom);
	}

	Location coordinateLocation(Coordinate coordinate) {
		coordinate = coordinate.zoomTo(zoom);
		ofPoint point = unproject(ofPoint(coordinate.column, coordinate.row));
		return Location(RAD_TO_DEG * point.y, RAD_TO_DEG * point.x);
	}

};

#endif