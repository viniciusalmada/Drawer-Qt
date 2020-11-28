#ifndef DRAWER_HED_HALFEDGE_H
#define DRAWER_HED_HALFEDGE_H

namespace HED {
	class Vertex;
	
	class Edge;
	
	class Loop;
	
	class Face;
	
	class HalfEdge {
		HalfEdge();
		
		virtual ~HalfEdge();
		
		HalfEdge* mPrevious;
		HalfEdge* mNext;
		Edge* mEdge;
		Vertex* mVtx;
		Loop* mLoop;
	};
}

#endif