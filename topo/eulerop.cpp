#include <set>
#include <geom/geomutils.h>
#include "eulerop.h"

void HED::EulerOp::mvfs(HED::Model& model, double x, double y) {
	int vertex = model.newVertex(x, y);
	int face = model.newFace();
	int he = model.newHalfedge(vertex);
	int loop = model.newLoop(face);
	
	model.mFaces[face].loopIn = loop;
	model.mLoops[loop].he = he;
	model.mHalfedges[he].next = he;
	model.mHalfedges[he].loop = loop;
	model.setHeOfVtx(vertex, he);
}

void HED::EulerOp::mev(HED::Model& model, int originVtx, double x, double y) {
	std::set<int> nearEdges = model.getNearEdgesFromVtx(originVtx);
	
	if (nearEdges.empty()) {
		mev0(model, originVtx, x, y);
		return;
	}
	
	if (nearEdges.size() == 1) {
		mev1(model, originVtx, x, y);
		return;
	}
	
	std::pair<int, int> prevAndNextEdge = model.getPrevAndNext(originVtx, x, y);
	int newVtx = model.newVertex(x, y);
	int newHe0 = model.newHalfedge(originVtx);
	int newHe1 = model.newHalfedge(newVtx);
	
	model.newEdge(newHe0, newHe1);
	
	model.setHeOfVtx(newVtx, newHe1);
	
	int hePrevModified = model.getHeFromEdgeOppositeVtx(prevAndNextEdge.first, originVtx);
	int heNextFromOrigin = model.getHeFromEdgeFromVtx(prevAndNextEdge.second, originVtx);
	
	model.setNextOfHe(hePrevModified, newHe0);
	model.setNextOfHe(newHe0, newHe1);
	model.setNextOfHe(newHe1, heNextFromOrigin);
	
	int loop = model.getLoopFromHe(hePrevModified);
	
	model.setLoopOfHe(newHe0, loop);
	model.setLoopOfHe(newHe1, loop);
}

void HED::EulerOp::mev1(HED::Model& model, int originVtx, double x, double y) {
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
}

void HED::EulerOp::mev0(HED::Model& model, int originVtx, double x, double y) {
	int vtx = model.newVertex(x, y);
	int he = model.newHalfedge(vtx);
	model.setHeOfVtx(vtx, he);
	int otherHe = model.getHeFromVtx(originVtx);
	model.newEdge(otherHe, he);
	model.setNextOfHe(he, otherHe);
	model.setNextOfHe(otherHe, he);
	
	int loop = model.getLoopFromHe(otherHe);
	model.setLoopOfHe(he, loop);
}

void HED::EulerOp::mef(HED::Model& model, int vtx0, int vtx1) {
	int newHe0 = model.newHalfedge(vtx0);
	int newHe1 = model.newHalfedge(vtx1);
	
	model.newEdge(newHe0, newHe1);
	
	int face = model.newFace();
	int loop = model.newLoop(face);
	
	
}
