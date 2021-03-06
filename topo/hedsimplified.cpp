#include <geom/geomutils.h>
#include <cstdio>
#include "hedsimplified.h"

HEDSimpl::HEDVertex::HEDVertex(QPointF pt) : pt(pt) {
}

HEDSimpl::HEDHalfedge::HEDHalfedge(int vertexId) : vtx(vertexId) {

}

HEDSimpl::HEDTriangle::HEDTriangle(int h0, int h1, int h2) : he0(h0), he1(h1), he2(h2) {

}

HEDSimpl::HEDEdge::HEDEdge(int h0, int h1) : he0(h0), he1(h1) {

}

HEDSimpl::Model::Model() : mVertices(), mHalfedges(), mEdges(), mTriangles() {
	/*mVertices.reserve(10000);
	mHalfedges.reserve(10000);
	mEdges.reserve(10000);
	mTriangles.reserve(10000);*/
};

int HEDSimpl::Model::newVertex(const QPointF& pt) {
	mVertices.push_back(HEDVertex(pt));
	return static_cast<int>(mVertices.size()) - 1;
}

int HEDSimpl::Model::newHalfedge(int vertexId) {
	HEDHalfedge hed{vertexId};
	mHalfedges.push_back(hed);
	mVertices[vertexId].he = static_cast<int>(mHalfedges.size()) - 1;
	return static_cast<int>(mHalfedges.size()) - 1;
}

int HEDSimpl::Model::newEdge(int he1, int he2) {
	mEdges.push_back(HEDEdge(he1, he2));
	mHalfedges[he1].edge = (int) mEdges.size() - 1;
	mHalfedges[he2].edge = (int) mEdges.size() - 1;
	return static_cast<int>(mEdges.size()) - 1;
}

int HEDSimpl::Model::newTriangle(int he1, int he2, int he3) {
	mTriangles.push_back(HEDTriangle(he1, he2, he3));
	mHalfedges[he1].tri = (int) mTriangles.size() - 1;
	mHalfedges[he2].tri = (int) mTriangles.size() - 1;
	mHalfedges[he3].tri = (int) mTriangles.size() - 1;
	
	mHalfedges[he1].next = he2;
	mHalfedges[he2].next = he3;
	mHalfedges[he3].next = he1;
	return static_cast<int>(mTriangles.size()) - 1;
}

void HEDSimpl::Model::updateHalfedgeOfVertex(int vtxId, int heId) {
	mVertices[vtxId].he = heId;
}

void HEDSimpl::Model::updateHalfedgesOfTriangle(int triId, int he0, int he1, int he2) {
	mTriangles[triId].he0 = he0;
	mTriangles[triId].he1 = he1;
	mTriangles[triId].he2 = he2;
	
	mHalfedges[he0].tri = triId;
	mHalfedges[he1].tri = triId;
	mHalfedges[he2].tri = triId;
	
	mHalfedges[he0].next = he1;
	mHalfedges[he1].next = he2;
	mHalfedges[he2].next = he0;
}

void HEDSimpl::Model::updateVertexOfHalfedge(int he, int vtx) {
	mHalfedges[he].vtx = vtx;
}

int HEDSimpl::Model::halfedgeFromTriangle(int triId, int pos) const {
	if (pos == 0)
		return mTriangles[triId].he0;
	else if (pos == 1)
		return mTriangles[triId].he1;
	else if (pos == 2)
		return mTriangles[triId].he2;
	return -1;
}

int HEDSimpl::Model::vertexOfTriangle(int triId, int pos) const {
	int he = halfedgeFromTriangle(triId, pos);
	return mHalfedges[he].vtx;
}

std::vector<int> HEDSimpl::Model::verticesOfTriangles(int triId) const {
	int v0 = vertexOfTriangle(triId, 0);
	int v1 = vertexOfTriangle(triId, 1);
	int v2 = vertexOfTriangle(triId, 2);
	return {v0, v1, v2};
}

int HEDSimpl::Model::halfedgeFromVertex(int vtxId) {
	return mVertices[vtxId].he;
}

int HEDSimpl::Model::edgeFromHalfedge(int he) {
	return mHalfedges[he].edge;
}

std::vector<int> HEDSimpl::Model::nearEdges(const std::vector<int>& vertices) {
	std::vector<int> edges{};
	for (auto vtxId : vertices) {
		const int firstHe = halfedgeFromVertex(vtxId);
		const int firstEdge = edgeFromHalfedge(firstHe);
		edges.push_back(firstEdge);
		
		int currentHe = nextOfHalfedge(firstHe);
		currentHe = nextOfHalfedge(currentHe);
		
		if (continueCollectEdges(edges, currentHe, firstEdge))
			continue;
		
		const int firstHeMate = mateOfHalfedge(firstHe);
		if (triangleOfHalfedge(firstHeMate) == -1)
			continue;
		
		currentHe = nextOfHalfedge(firstHeMate);
		collectEdgesReverse(edges, currentHe);
	}
	return edges;
}

bool HEDSimpl::Model::continueCollectEdges(std::vector<int>& edges, int& he, const int& firstEdge) {
	while (he != -1) {
		int edge = edgeFromHalfedge(he);
		if (edge == firstEdge)
			return true;
		
		edges.push_back(edge);
		he = mateOfHalfedge(he);
		if (triangleOfHalfedge(he) == -1)
			return false;
		
		he = nextOfHalfedge(he);
		he = nextOfHalfedge(he);
	}
	return false;
}

void HEDSimpl::Model::collectEdgesReverse(std::vector<int>& edges, int& he) {
	while (he != -1) {
		const int edge = edgeFromHalfedge(he);
		edges.push_back(edge);
		he = mateOfHalfedge(he);
		if (triangleOfHalfedge(he) == -1)
			break;
		he = nextOfHalfedge(he);
	}
}

int HEDSimpl::Model::mateOfHalfedge(int he) {
	HEDEdge e = mEdges[mHalfedges[he].edge];
	if (e.he0 == he)
		return e.he1;
	return e.he0;
}

int HEDSimpl::Model::triangleOfHalfedge(int he) {
	return mHalfedges[he].tri;
}

int HEDSimpl::Model::nextOfHalfedge(int he) {
	return mHalfedges[he].next;
}

QPointF HEDSimpl::Model::pointOfHalfedge(int he) {
	HEDVertex vtx = mVertices[vertexOfHalfedge(he)];
	return vtx.pt;
}

int HEDSimpl::Model::vertexOfHalfedge(int he) {
	return mHalfedges[he].vtx;
}

QPointF HEDSimpl::Model::pointOfVertex(int vtx) {
	return mVertices[vtx].pt;
}

bool HEDSimpl::Model::isEdgeLegal(int edge, const std::function<bool(Model&, int)>& predicate) {
	return predicate(*this, edge);
}

std::vector<int> HEDSimpl::Model::makeFlip(int edgeId) {
	HEDEdge edge = mEdges[edgeId];
	int tri0 = triangleOfHalfedge(edge.he0);
	int tri1 = triangleOfHalfedge(edge.he1);
	
	int nextHed0 = nextOfHalfedge(edge.he0);
	int nextNextHed0 = nextOfHalfedge(nextHed0);
	
	int nextHed1 = nextOfHalfedge(edge.he1);
	int nextNextHed1 = nextOfHalfedge(nextHed1);
	
	int vtx0 = vertexOfHalfedge(edge.he0);
	int hedVtx0 = halfedgeFromVertex(vtx0);
	if (hedVtx0 == edge.he0)
		updateHalfedgeOfVertex(vtx0, nextHed1);
	
	int vtx1 = vertexOfHalfedge(edge.he1);
	int hedVtx1 = halfedgeFromVertex(vtx1);
	if (hedVtx1 == edge.he1)
		updateHalfedgeOfVertex(vtx1, nextHed0);
	
	int newVtxOfHed0 = vertexOfHalfedge(nextNextHed0);
	int newVtxOfHed1 = vertexOfHalfedge(nextNextHed1);
	
	updateVertexOfHalfedge(edge.he0, newVtxOfHed0);
	updateVertexOfHalfedge(edge.he1, newVtxOfHed1);
	
	updateHalfedgesOfTriangle(tri0, edge.he0, nextNextHed1, nextHed0);
	updateHalfedgesOfTriangle(tri1, edge.he1, nextNextHed0, nextHed1);
	
	return {newVtxOfHed0, newVtxOfHed1};
}

bool HEDSimpl::Model::isEdgeNearTwoFaces(int edgeId) {
	HEDEdge edge = mEdges[edgeId];
	HEDHalfedge h0 = mHalfedges[edge.he0];
	HEDHalfedge h1 = mHalfedges[edge.he1];
	
	return h0.tri != -1 && h1.tri != -1;
}

int HEDSimpl::Model::findEdgeThatContainsPoint(const QPointF& pt) {
	for (int i = 0; i < mEdges.size(); i++) {
		HEDEdge edge = mEdges[i];
		QPointF pt0 = pointOfHalfedge(edge.he0);
		QPointF pt1 = pointOfHalfedge(edge.he1);
		
		if (GeomUtils::edgeContains(pt0, pt1, pt))
			return i;
	}
	return -1;
}

int HEDSimpl::Model::findTriangleThatContainsPoint(const QPointF& pt) {
	for (int i = 0; i < mTriangles.size(); i++) {
		HEDTriangle tri = mTriangles[i];
		QPointF p0 = pointOfHalfedge(tri.he0);
		QPointF p1 = pointOfHalfedge(tri.he1);
		QPointF p2 = pointOfHalfedge(tri.he2);
		
		if (GeomUtils::polygonContains(p0, p1, p2, pt))
			return i;
	}
	return -1;
}

std::vector<int> HEDSimpl::Model::splitEdge(int edgeId, const QPointF& pt) {
	int vtx = newVertex(pt);
	HEDEdge edge = mEdges[edgeId];
	
	int hed1 = edge.he0;
	int hed2 = edge.he1;
	int tri0 = triangleOfHalfedge(hed1);
	int tri1 = triangleOfHalfedge(hed2);
	int hed3 = newHalfedge(vtx);
	int hed4 = newHalfedge(vtx);
	int hed5 = newHalfedge(vtx);
	int hed6 = newHalfedge(vtx);
	
	int nextHed1 = nextOfHalfedge(hed1);
	int nextNextHed1 = nextOfHalfedge(nextHed1);
	int vtxNextNextHed1 = vertexOfHalfedge(nextNextHed1);
	int hed7 = newHalfedge(vtxNextNextHed1);
	
	int nextHed2 = nextOfHalfedge(hed2);
	int nextNextHed2 = nextOfHalfedge(nextHed2);
	int vtxNextNextHed2 = vertexOfHalfedge(nextNextHed2);
	int hed8 = newHalfedge(vtxNextNextHed2);
	
	setHalfedgesOfEdge(edgeId, hed3, hed2);
	newEdge(hed6, hed1);
	newEdge(hed4, hed7);
	newEdge(hed5, hed8);
	
	newTriangle(hed4, nextNextHed1, hed1);
	newTriangle(hed6, nextHed2, hed8);
	
	updateHalfedgesOfTriangle(tri0, hed3, nextHed1, hed7);
	updateHalfedgesOfTriangle(tri1, hed5, nextNextHed2, hed2);
	
	return {vtx, vtxNextNextHed1, vtxNextNextHed2};
}

std::vector<int> HEDSimpl::Model::splitTriangle(int tri, QPointF pt) {
	std::vector<int> verticesToCheck = verticesOfTriangles(tri);
	
	int newV = newVertex(pt);
	
	int h1e0 = newHalfedge(newV);
	int h1e1 = newHalfedge(newV);
	int h1e2 = newHalfedge(newV);
	int h2e0 = newHalfedge(vertexOfTriangle(tri, 0));
	int h2e1 = newHalfedge(vertexOfTriangle(tri, 1));
	int h2e2 = newHalfedge(vertexOfTriangle(tri, 2));
	
	newEdge(h1e0, h2e0);
	newEdge(h1e1, h2e1);
	newEdge(h1e2, h2e2);
	
	int h0Tri0 = halfedgeFromTriangle(tri, 0);
	int h1Tri0 = halfedgeFromTriangle(tri, 1);
	int h2Tri0 = halfedgeFromTriangle(tri, 2);
	
	newTriangle(h1e1, h1Tri0, h2e2);
	
	newTriangle(h1e2, h2Tri0, h2e0);
	
	updateHalfedgesOfTriangle(tri, h0Tri0, h2e1, h1e0);
	
	return verticesToCheck;
}

void HEDSimpl::Model::setHalfedgesOfEdge(int edgeId, int he0, int he1) {
	mEdges[edgeId].he0 = he0;
	mEdges[edgeId].he1 = he1;
	
	mHalfedges[he0].edge = edgeId;
	mHalfedges[he1].edge = edgeId;
}

HEDSimpl::HEDEdge HEDSimpl::Model::getEdge(int i) {
	return mEdges[i];
}

std::vector<QPointF> HEDSimpl::Model::getVerticesTriangle(int tri) const {
	auto pts = verticesOfTriangles(tri);
	return {mVertices[pts[0]].pt, mVertices[pts[1]].pt, mVertices[pts[2]].pt};
}


