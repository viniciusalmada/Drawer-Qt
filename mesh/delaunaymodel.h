#ifndef DRAWER_DELAUNAY_MODEL_H
#define DRAWER_DELAUNAY_MODEL_H

#include "topo/hedsimplified.h"

class DelaunayModel {
private:
	HEDSimpl::Model mModel{};
	
	explicit DelaunayModel(const std::vector<QPointF>& points);
	
	void addTriangle(const QPointF& pt);
	
	void finish();
	
	void verifyNearEdgesFromVertices(const std::vector<int>& vertices);
	
	static bool predicateLegalEdge(HEDSimpl::Model& model, int edgeId);
};

#endif