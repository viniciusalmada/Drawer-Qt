#include "delaunaymodel.h"
#include <geom/geomutils.h>
#include <geom/point.h>
#include <cstdio>
#include <QPolygon>
#include <set>

DelaunayModel::DelaunayModel(const std::vector<QPointF>& points) {
	for (QPointF _p : points) {
		printf("%1.5f, %1.5f\n", _p.x(), _p.y());
	}
	mTriangleBox = GeomUtils::triangleBox(points);
	
	int v0 = mModel.newVertex(mTriangleBox[0]);
	int v1 = mModel.newVertex(mTriangleBox[1]);
	int v2 = mModel.newVertex(mTriangleBox[2]);
	
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
	if (tri == -1) {
		printf("NOT FOUND = %1.5f,  %1.5f\n", pt.x(), pt.y());
		return;
	}
	auto vertices = mModel.splitTriangle(tri, pt);
	verifyNearEdgesFromVertices(vertices);
}

QVector<QPolygonF> DelaunayModel::finish() {
	auto edges = mModel.nearEdges({0, 1, 2});
	std::vector<int> verticesToIgn{0, 1, 2};
	std::set<int> trianglesToIgn;
	
	std::vector<int> trianglesFinal;
	
	for (int ed : edges) {
		HEDSimpl::HEDEdge edge = mModel.getEdge(ed);
		auto he1 = mModel.mHalfedges[edge.he0];
		auto he2 = mModel.mHalfedges[edge.he1];
		
		trianglesToIgn.insert(he1.tri);
		trianglesToIgn.insert(he2.tri);
	}
	
	for (int i = 0; i < mModel.mTriangles.size(); ++i) {
		if (trianglesToIgn.find(i) != trianglesToIgn.end())
			continue;
		trianglesFinal.push_back(i);
	}
	
	QVector<QPolygonF> triangles{};
	for (int i : trianglesFinal) {
		auto v = mModel.verticesOfTriangles(i);
		QPointF pt0 = mModel.mVertices[v[0]].pt;
		QPointF pt1 = mModel.mVertices[v[1]].pt;
		QPointF pt2 = mModel.mVertices[v[2]].pt;
		QPolygonF pol{{pt0, pt1, pt2}};
		triangles.push_back(pol);
	}
	return triangles;
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
	auto circle230 = GeomUtils::circle3Points(p2, p3, p0);
	
	if (circle012.radius - circle230.radius < 0.0001 &&
	    PointUtils::dist(circle012.center, circle230.center) < 0.0001)
		return true;
	
	if (!circle012.contains(p3))
		return true;
	
	if (!circle230.contains(p1))
		return true;
	
	return false;
}

QVector<QPointF> DelaunayModel::totalPoints() {
	QVector<QPointF> vertices;
	
	for (HEDSimpl::HEDVertex vtx : mModel.mVertices) {
		double dist = PointUtils::dist(vtx.pt, mTriangleBox[0]);
		if (dist < 0.000001)
			continue;
		dist = PointUtils::dist(vtx.pt, mTriangleBox[1]);
		if (dist < 0.000001)
			continue;
		dist = PointUtils::dist(vtx.pt, mTriangleBox[2]);
		if (dist < 0.000001)
			continue;
		
		vertices.push_back(vtx.pt);
	}
	return vertices;
}
