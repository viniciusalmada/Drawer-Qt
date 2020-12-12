#ifndef DRAWER_DELAUNAY_MODEL_H
#define DRAWER_DELAUNAY_MODEL_H

#include "topo/hedsimplified.h"

class DelaunayModel {
public:
	explicit DelaunayModel(const std::vector<QPointF>& points);

private:
	HEDSimpl::Model mModel{};
	
	void addTriangle(const QPointF& pt);
	
	void finish();
	
	void verifyNearEdgesFromVertices(const std::vector<int>& vertices);
	
	static bool predicateLegalEdge(HEDSimpl::Model& model, int edgeId);
};

#endif