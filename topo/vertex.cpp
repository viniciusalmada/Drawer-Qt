#include "vertex.h"
#include "halfedge.h"

void HED::Vertex::setHe(HalfEdge* he) {
	this->mHe = he;
}
