#include "euleroperators.h"
#include "loop.h"
#include "halfedge.h"
#include "vertex.h"

void HED::EulerOperators::mvfs(Vertex* vtx, Face* face) {
	Loop* newLoop = new Loop(face);
	HalfEdge* he = new HalfEdge();
	
	loopSetHe(newLoop, he);
	heSetLoop(he, newLoop);
	heSetNext(he, he);
	heSetPrev(he, he);
	heSetVtx(he, vtx);
	heSetEdge(he, nullptr);
	vtxSetHe(vtx, he);
}

void HED::EulerOperators::mev(HED::Edge* edge, HED::Vertex* vtxBegin, HED::Vertex* vtxNext) {
	HalfEdge* he1 = new HalfEdge{};
	HalfEdge* he2 = new HalfEdge{};
	
	heSetVtx(he1, vtxBegin);
	heSetVtx(he2, vtxNext);
	heSetEdge(he1, edge);
	heSetEdge(he2, edge);
	vtxSetHe(vtxNext, he2);
	
	std::set<Edge*> nearEdges = vtxGetEdges(vtxBegin)
}

void HED::EulerOperators::loopSetHe(HED::Loop* loop, HED::HalfEdge* he) {
	loop->setHe(he);
}

void HED::EulerOperators::heSetLoop(HED::HalfEdge* he, HED::Loop* loop) {
	he->setLoop(loop);
}

void HED::EulerOperators::heSetNext(HED::HalfEdge* he, HED::HalfEdge* next) {
	he->setNext(next);
}

void HED::EulerOperators::heSetPrev(HED::HalfEdge* he, HED::HalfEdge* prev) {
	he->setPrev(prev);
}

void HED::EulerOperators::heSetVtx(HED::HalfEdge* he, HED::Vertex* vtx) {
	he->setVtx(vtx);
}

void HED::EulerOperators::heSetEdge(HED::HalfEdge* he, HED::Edge* edge) {
	he->setEdge(edge);
}

void HED::EulerOperators::vtxSetHe(HED::Vertex* vtx, HED::HalfEdge* he) {
	vtx->setHe(he);
}

std::set<HED::Edge*> HED::EulerOperators::vtxGetEdges(HED::Vertex* vtx) {
	return vtx->nearEdges();
}
