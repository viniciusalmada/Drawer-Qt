#ifndef DRAWER_DELAUNAY_MODEL_H
#define DRAWER_DELAUNAY_MODEL_H

#include "topo/hed.h"

class DelaunayModel {
private:
	HED::Model mModel;
	
	explicit DelaunayModel(const std::vector<QPointF>& points);
};

#endif