#ifndef DRAWER_DELAUNAY_MODEL_H
#define DRAWER_DELAUNAY_MODEL_H

#include "topo/hed.h"

class DelaunayModel {
private:
	HED::Model mModel{};
	
	explicit DelaunayModel(const std::vector<QPointF>& points);
	
	void addTriangle(const QPointF& pt);
	
	void finish();
	
	void verifyNearEdgesFromVertices(const std::vector<int>& vertices);
	
	static bool predicateLegalEdge(HED::Model& model, int edgeId);
};

#endif