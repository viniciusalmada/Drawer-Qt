#include "delaunaymodel.h"
#include <geom/geomutils.h>

DelaunayModel::DelaunayModel(const std::vector<QPointF>& points) {
	std::array<QPointF, 3> triangleBox = GeomUtils::triangleBox(points);
	
	int v0 = mModel.newVertex(triangleBox[0]);
	int v1 = mModel.newVertex(triangleBox[1]);
	int v2 = mModel.newVertex(triangleBox[2]);
	
	int h1e0 = mModel.newHalfedge(v0);
	int h2e0 = mModel.newHalfedge(v1);
	int h1e1 = mModel.newHalfedge(v1);
	int h2e1 = mModel.newHalfedge(v2);
	int h1e2 = mModel.newHalfedge(v2);
	int h2e2 = mModel.newHalfedge(v0);
	
	mModel.newEdge(h1e0, h2e0);
	mModel.newEdge(h1e1, h2e1);
	mModel.newEdge(h1e2, h2e2);
	
	mModel.newTriangle(h1e0, h1e1, h1e2);
	
	for (const auto& pt : points) {
		addTriangle(pt);
	}
	finish();
}

void DelaunayModel::addTriangle(const QPointF& pt) {
	int edge = mModel.findEdgeThatContainsPoint(pt);
	if (edge != -1) {
		auto vertices = mModel.splitEdge(edge, pt);
		verifyNearEdgesFromVertices(vertices);
		return;
	}
	
	int tri = mModel.findTriangleThatContainsPoint(pt);
	auto vertices = mModel.splitTriangle(tri, pt);
	verifyNearEdgesFromVertices(vertices);
}

void DelaunayModel::finish() {

}

void DelaunayModel::verifyNearEdgesFromVertices(const std::vector<int>& vertices) {
	for (const auto& i : vertices) {
		auto edges = mModel.nearEdges({i});
		for (const auto& e : edges) {
			if (!mModel.isEdgeLegal(e, predicateLegalEdge)) {
				auto changedVertices = mModel.makeFlip(e);
				verifyNearEdgesFromVertices(changedVertices);
			}
		}
	}
}

bool DelaunayModel::predicateLegalEdge(HEDSimpl::Model& model, int edgeId) {
	if (!model.isEdgeNearTwoFaces(edgeId)) return true;
	
	auto edge = model.getEdge(edgeId);
	int hed1 = edge.he0;
	int hed1next = model.nextOfHalfedge(hed1);
	int hed1next2 = model.nextOfHalfedge(hed1next);
	int hed2 = edge.he1;
	int hed2next = model.nextOfHalfedge(hed2);
	int hed2next2 = model.nextOfHalfedge(hed2next);
	
	auto p0 = model.pointOfHalfedge(hed1);
	auto p1 = model.pointOfHalfedge(hed2next2);
	auto p2 = model.pointOfHalfedge(hed2);
	auto p3 = model.pointOfHalfedge(hed1next2);
	
	if (!GeomUtils::isConvexPolygon({p0, p1, p2, p3}))
		return true;
	
	auto circle012 = GeomUtils::circle3Points(p0, p1, p2);
	if (!circle012.contains(p3))
		return true;
	
	auto circle230 = GeomUtils::circle3Points(p2, p3, p0);
	if (!circle230.contains(p1))
		return true;
	
	return false;
}