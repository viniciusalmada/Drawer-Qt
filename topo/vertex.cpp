#include "vertex.h"
#include "halfedge.h"
#include "edge.h"

void HED::Vertex::setHe(HalfEdge* he) {
	this->mHe = he;
}

std::set<HED::Edge*> HED::Vertex::nearEdges() {
	std::set<Edge*> edges;
	HalfEdge* he = this->mHe;
	HalfEdge* heBegin = he;
	
	if (he->edge() == nullptr)
		return edges;
	
	do {
		edges.insert(he->edge());
	} while ((he = he->mate()->next()) != heBegin);
	return edges;
}
