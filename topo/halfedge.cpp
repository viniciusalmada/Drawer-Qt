#include "halfedge.h"
#include "loop.h"

void HED::HalfEdge::setLoop(Loop* loop) {
	this->mLoop = loop;
}

void HED::HalfEdge::setNext(HED::HalfEdge* next) {
	this->mNext = next;
}

void HED::HalfEdge::setPrev(HED::HalfEdge* prev) {
	this->mPrevious = prev;
}

void HED::HalfEdge::setVtx(HED::Vertex* vtx) {
	this->mVtx = vtx;
}

void HED::HalfEdge::setEdge(HED::Edge* edge) {
	this->mEdge = edge;
}
