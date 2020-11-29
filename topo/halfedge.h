#ifndef DRAWER_HED_HALFEDGE_H
#define DRAWER_HED_HALFEDGE_H

namespace HED {
	class Vertex;
	
	class Edge;
	
	class Loop;
	
	class Face;
	
	class HalfEdge {
		HalfEdge* mPrevious;
		HalfEdge* mNext;
		Edge* mEdge;
		Vertex* mVtx;
		Loop* mLoop;
	
	public:
		
		HalfEdge();
		
		virtual ~HalfEdge();
		
		void setLoop(Loop* loop);
		
		void setNext(HalfEdge* next);
		
		void setPrev(HalfEdge* prev);
		
		void setVtx(Vertex* vtx);
		
		void setEdge(Edge* edge);
	};
}

#endif