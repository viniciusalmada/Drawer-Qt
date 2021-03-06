#include <geom/geomutils.h>
#include "hed.h"

HED::HEDVertex::HEDVertex(double x, double y) : x(x), y(y) {
}

HED::HEDVertex::HEDVertex() = default;

HED::HEDHalfedge::HEDHalfedge(int vertexId) : vtx(vertexId) {
}

HED::HEDHalfedge::HEDHalfedge() = default;

HED::HEDFace::HEDFace() = default;

HED::HEDEdge::HEDEdge(int h0, int h1) : he0(h0), he1(h1) {
}

HED::HEDEdge::HEDEdge() = default;

HED::HEDLoop::HEDLoop() = default;

HED::HEDLoop::HEDLoop(int face) : face(face) {
};

HED::Model::Model() = default;

HED::Model::~Model() {
	mVertices.clear();
	mHalfedges.clear();
	mEdges.clear();
	mFaces.clear();
}

int HED::Model::newVertex(double x, double y) {
	mVertices[mVerticesCount] = HEDVertex(x, y);
	return mVerticesCount++;
}

int HED::Model::newHalfedge(int vertexId) {
	mHalfedges[mHalfedgesCount] = HEDHalfedge(vertexId);
	return mHalfedgesCount++;
}

int HED::Model::newEdge(int he0, int he1) {
	mEdges[mEdgesCount] = HEDEdge();
	mEdges[mEdgesCount].he0 = he0;
	mEdges[mEdgesCount].he1 = he1;
	mHalfedges[he0].edge = mEdgesCount;
	mHalfedges[he1].edge = mEdgesCount;
	return mEdgesCount++;
}

int HED::Model::newFace() {
	mFaces[mFacesCount] = HEDFace();
	return mFacesCount++;
}

void HED::Model::updateHalfedgeOfVertex(int vtxId, int heId) {
	mVertices[vtxId].he = heId;
}

void HED::Model::updateVertexOfHalfedge(int he, int vtx) {
	mHalfedges[he].vtx = vtx;
}

int HED::Model::halfedgeFromVertex(int vtxId) {
	if (mVertices.find(vtxId) == mVertices.end())
		return -1;
	return mVertices[vtxId].he;
}

int HED::Model::edgeFromHalfedge(int he) {
	return mHalfedges[he].edge;
}

int HED::Model::mateOfHalfedge(int he) {
	HEDEdge e = mEdges[mHalfedges[he].edge];
	if (e.he0 == he)
		return e.he1;
	return e.he0;
}

void HED::Model::setHalfedgesOfEdge(int edgeId, int he0, int he1) {
	mEdges[edgeId].he0 = he0;
	mEdges[edgeId].he1 = he1;
	
	mHalfedges[he0].edge = edgeId;
	mHalfedges[he1].edge = edgeId;
}

int HED::Model::newLoop(int face) {
	mLoops[mLoopCount] = HEDLoop(face);
	return mLoopCount++;
}

int HED::Model::getEdgeFromHe(int he) {
	if (mHalfedges.find(he) == mHalfedges.end())
		return -1;
	return mHalfedges[he].edge;
}

void HED::Model::setNextOfHe(int he, int next) {
	mHalfedges[he].next = next;
}

void HED::Model::setEdgeOfHe(int he, int edge) {
	mHalfedges[he].edge = edge;
}

int HED::Model::getLoopFromHe(int he) {
	if (mHalfedges.find(he) == mHalfedges.end())
		return -1;
	return mHalfedges[he].loop;
}

void HED::Model::setLoopOfHe(const int he, const int loop) {
	mLoops[loop].he = he;
	mHalfedges[he].loop = loop;
}

void HED::Model::setLoopOfHalfedges(const int heFirst, const int loop) {
	mLoops[loop].he = heFirst;
	int curr = heFirst;
	do {
		mHalfedges[curr].loop = loop;
		curr = getNextFromHe(heFirst);
	} while (curr != heFirst);
}

int HED::Model::getHe0FromEdge(int edge) {
	if (mEdges.find(edge) == mEdges.end())
		return -1;
	return mEdges[edge].he0;
}

int HED::Model::getHe1FromEdge(int edge) {
	if (mEdges.find(edge) == mEdges.end())
		return -1;
	return mEdges[edge].he1;
}

int HED::Model::getFaceFromLoop(int loop) {
	if (mLoops.find(loop) == mLoops.end())
		return -1;
	return mLoops[loop].face;
}

void HED::Model::setVtxOfHe(int he, int vtx) {
	mHalfedges[he].vtx = vtx;
}

int HED::Model::getNextFromHe(int he) {
	if (mHalfedges.find(he) == mHalfedges.end())
		return -1;
	return mHalfedges[he].next;
}

int HED::Model::getVtxFromHe(int he) {
	if (mHalfedges.find(he) == mHalfedges.end())
		return -1;
	return mHalfedges[he].vtx;
}

void HED::Model::setHeOfVtx(int vtx, int he) {
	mVertices[vtx].he = he;
}

std::set<int> HED::Model::getNearEdgesFromVtx(int vtx) {
	std::set<int> edges{};
	const int he = getHeFromVtx(vtx);
	if (getEdgeFromHe(he) == -1)
		return edges;
	
	edges.insert(getEdgeFromHe(he));
	int next = he;
	do {
		int mateHe = mateOfHalfedge(next);
		next = getNextFromHe(mateHe);
		edges.insert(getEdgeFromHe(next));
	} while (he != next);
	
	return edges;
}

int HED::Model::getHeFromVtx(int vtx) {
	if (mVertices.find(vtx) == mVertices.end())
		return -1;
	return mVertices[vtx].he;
}

std::pair<double, double> HED::Model::getPtFromVtx(int vtx) {
	if (mVertices.find(vtx) == mVertices.end())
		return {NAN, NAN};
	HEDVertex v = mVertices[vtx];
	return {v.x, v.y};
}

std::pair<double, double> HED::Model::getPtFromHe(int he) {
	if (mHalfedges.find(he) == mHalfedges.end())
		return {NAN, NAN};
	HEDHalfedge h = mHalfedges[he];
	return getPtFromVtx(h.vtx);
}

std::vector<std::pair<int, double>> HED::Model::getEdgesAngles(const std::set<int>& edges, int vtx, double normBase) {
	std::vector<std::pair<int, double>> angles{};
	for (const auto& ed : edges) {
		HEDEdge edge = mEdges[ed];
		HEDHalfedge he0 = mHalfedges[edge.he0];
		HEDHalfedge he1 = mHalfedges[edge.he1];
		
		HEDVertex vtxOrigin = mVertices[vtx];
		HEDVertex vtxOther = he0.vtx == vtx ? mVertices[he1.vtx] : mVertices[he0.vtx];
		
		double angle = GeomUtils::pseudoAngle(vtxOrigin.x, vtxOrigin.y, vtxOther.x, vtxOther.y);
		angles.emplace_back(ed, angle);
	}
	
	std::sort(angles.begin(), angles.end(), [](std::pair<int, double> l, std::pair<int, double> r) {
		return l.second < r.second;
	});
	
	for (auto& edgesAngle : angles) {
		double angleNorm = edgesAngle.second - normBase;
		if (angleNorm < 0.0)
			angleNorm = 8.0 + angleNorm;
		edgesAngle.second = angleNorm;
	}
	
	return angles;
}

int HED::Model::getHeFromEdgeOppositeVtx(int edge, int vtx) {
	if (mEdges.find(edge) == mEdges.end())
		return -1;
	
	int h0 = getHe0FromEdge(edge);
	int h1 = getHe1FromEdge(edge);
	if (getVtxFromHe(h0) == vtx)
		return h1;
	else
		return h0;
}

int HED::Model::getHeFromEdgeFromVtx(int edge, int vtx) {
	if (mEdges.find(edge) == mEdges.end())
		return -1;
	
	int h0 = getHe0FromEdge(edge);
	int h1 = getHe1FromEdge(edge);
	if (getVtxFromHe(h0) == vtx)
		return h0;
	else
		return h1;
}

std::pair<int, int> HED::Model::getPrevAndNext(int vtx, double x, double y) {
	double origX = getPtFromVtx(vtx).first;
	double origY = getPtFromVtx(vtx).second;
	double angleNewEdge = GeomUtils::pseudoAngle(origX, origY, x, y);
	
	std::set<int> nearEdges = getNearEdgesFromVtx(vtx);
	if (nearEdges.empty())
		return {-1, -1};
	else if (nearEdges.size() == 1)
		return {*nearEdges.begin(), -1};
	
	std::vector<std::pair<int, double>> edgesAngles = getEdgesAngles(nearEdges, vtx, angleNewEdge);
	
	
	std::pair<int, double> prevEdge = *std::min_element(edgesAngles.begin(), edgesAngles.end(),
	                                                    [](std::pair<int, double> l, std::pair<int, double> r) {
		                                                    return l.second < r.second;
	                                                    });
	std::pair<int, double> nextEdge = *std::max_element(edgesAngles.begin(), edgesAngles.end(),
	                                                    [](std::pair<int, double> l, std::pair<int, double> r) {
		                                                    return l.second < r.second;
	                                                    });
	return {prevEdge.first, nextEdge.first};
}
