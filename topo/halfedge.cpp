#include "halfedge.h"
#include "edge.h"
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

HED::Edge* HED::HalfEdge::edge() {
	return this->mEdge;
}

HED::HalfEdge* HED::HalfEdge::mate() {
	return this->edge()->he1() == this ? this->edge()->he2() : this->edge()->he1();
}

HED::HalfEdge* HED::HalfEdge::next() {
	return this->mNext;
}

HED::HalfEdge::~HalfEdge() = default;

HED::HalfEdge::HalfEdge() = default;
