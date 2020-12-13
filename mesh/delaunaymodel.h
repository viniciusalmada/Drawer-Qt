#ifndef DRAWER_DELAUNAY_MODEL_H
#define DRAWER_DELAUNAY_MODEL_H

#include "topo/hedsimplified.h"
#include <QPolygonF>

class DelaunayModel {
public:
	explicit DelaunayModel(const std::vector<QPointF>& points);
	
	HEDSimpl::Model mModel;
	
	
	QVector<QPolygonF> finish();
	
	QVector<QPointF> totalPoints();

private:
	
	std::array<QPointF, 3> mTriangleBox;
	
	void addTriangle(const QPointF& pt);
	
	void verifyNearEdgesFromVertices(const std::vector<int>& vertices);
	
	static bool predicateLegalEdge(HEDSimpl::Model& model, int edgeId);
};

#endif