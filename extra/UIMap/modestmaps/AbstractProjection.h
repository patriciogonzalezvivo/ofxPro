#pragma once

#include "Transformation.h"
#include "Coordinate.h"
#include "Location.h"

class AbstractProjection {
public:

	float zoom;
	Transformation transformation;

	AbstractProjection(float _zoom): zoom(_zoom), transformation(Transformation()) { }
	AbstractProjection(float _zoom, Transformation _t): zoom(_zoom), transformation(_t) { }
    
    virtual ~AbstractProjection() { cout << "Abstract Projection destroyed" << endl; }

	virtual ofPoint rawProject(const ofPoint &point)=0;
	virtual ofPoint rawUnproject(const ofPoint &point)=0;

	ofPoint project(const ofPoint &point) {
		return transformation.transform(rawProject(point));
	}

	ofPoint unproject(const ofPoint &point) {
		return rawUnproject(transformation.untransform(point));
	}

	Coordinate locationCoordinate(const Location &location) {
		ofPoint point = project(ofPoint(DEG_TO_RAD * location.lon, DEG_TO_RAD * location.lat));
		return Coordinate(point.y, point.x, zoom);
	}

	Location coordinateLocation(const Coordinate &coordinate) {
		Coordinate zoomed = coordinate.zoomTo(zoom);
		ofPoint point = unproject(ofPoint(zoomed.column, zoomed.row));
		return Location(RAD_TO_DEG * point.y, RAD_TO_DEG * point.x);
	}

};

typedef std::shared_ptr<AbstractProjection> ProjectionRef;