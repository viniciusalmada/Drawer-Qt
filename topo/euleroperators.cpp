#include "euleroperators.h"
#include "loop.h"
#include "halfedge.h"
#include "vertex.h"

void HED::EulerOperators::mvfs(Vertex* vtx, Face* face) {
	Loop* outLoop = new Loop(face);
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
