#include <set>
#include <geom/geomutils.h>
#include "eulerop.h"

void HED::EulerOp::mvfs(HED::Model& model, double x, double y) {
	int vertex = model.newVertex(x, y);
	int face = model.newFace();
	int he = model.newHalfedge(vertex);
	int loop = model.newLoop(face);
	
	model.mFaces[face].loopOut = loop;
	model.mLoops[loop].he = he;
	model.mHalfedges[he].next = he;
	model.mHalfedges[he].loop = loop;
	model.setHeOfVtx(vertex, he);
}

void HED::EulerOp::mev(HED::Model& model, int originVtx, double x, double y) {
	std::set<int> nearEdges = model.getNearEdgesFromVtx(originVtx);
	
	if (nearEdges.empty()) {
		int vtx = model.newVertex(x, y);
		int he = model.newHalfedge(vtx);
		model.setHeOfVtx(vtx, he);
		int otherHe = model.getHeFromVtx(originVtx);
		model.newEdge(otherHe, he);
		model.setNextOfHe(he, otherHe);
		model.setNextOfHe(otherHe, he);
		
		int loop = model.getLoopFromHe(otherHe);
		model.setLoopOfHe(he, loop);
		return;
	}
	
	if (nearEdges.size() == 1) {
		int vtx = model.newVertex(x, y);
		int newHe0 = model.newHalfedge(originVtx);
		int newHe1 = model.newHalfedge(vtx);
		model.newEdge(newHe0, newHe1);
		
		int heExistent = model.getHeFromVtx(originVtx);
		int mateExistent = model.mateOfHalfedge(heExistent);
		
		model.setHeOfVtx(vtx, newHe1);
		
		model.setNextOfHe(mateExistent, newHe0);
		model.setNextOfHe(newHe0, newHe1);
		model.setNextOfHe(newHe1, heExistent);
		
		int loop = model.getLoopFromHe(heExistent);
		model.setLoopOfHe(newHe0, loop);
		model.setLoopOfHe(newHe1, loop);
		return;
	}
	
	double origX = model.getPtFromVtx(originVtx).first;
	double origY = model.getPtFromVtx(originVtx).second;
	double angleNewEdge = GeomUtils::pseudoAngle(origX, origY, x, y);
	
	std::vector<std::pair<int, double>> edgesAngles = model.getEdgesAngles(nearEdges, originVtx);
	
	std::sort(edgesAngles.begin(), edgesAngles.end(), [](std::pair<int, double> l, std::pair<int, double> r) {
		return l.second < r.second;
	});
	
	
	for (auto& edgesAngle : edgesAngles) {
		double angleNorm = edgesAngle.second - angleNewEdge;
		if (angleNorm < 0.0)
			angleNorm = 8.0 + angleNorm;
		edgesAngle.second = angleNorm;
	}
	
	std::pair<int, double> prevEdge = edgesAngles.front();
	std::pair<int, double> nextEdge = edgesAngles.front();
	
	for (const auto& edgesAngle : edgesAngles) {
		if (edgesAngle.second < prevEdge.second)
			prevEdge = edgesAngle;
		if (edgesAngle.second > nextEdge.second)
			nextEdge = edgesAngle;
	}
	
	int newVtx = model.newVertex(x, y);
	int newHe0 = model.newHalfedge(originVtx);
	int newHe1 = model.newHalfedge(newVtx);
	
	model.newEdge(newHe0, newHe1);
	
	model.setHeOfVtx(newVtx, newHe1);
	
	int hePrevModified = model.getHeFromEdgeOppositeVtx(prevEdge.first, originVtx);
	int heNextFromOrigin = model.getHeFromEdgeFromVtx(nextEdge.first, originVtx);
	
	model.setNextOfHe(hePrevModified, newHe0);
	model.setNextOfHe(newHe0, newHe1);
	model.setNextOfHe(newHe1, heNextFromOrigin);
	
	int loop = model.getLoopFromHe(hePrevModified);
	
	model.setLoopOfHe(newHe0, loop);
	model.setLoopOfHe(newHe1, loop);
}
