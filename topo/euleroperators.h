#ifndef DRAWER_HED_EULER_H
#define DRAWER_HED_EULER_H

namespace HED {
	
	class Vertex;
	
	class Edge;
	
	class Face;
	
	class Loop;
	
	class HalfEdge;
	
	enum class EOrientation {
		POSITIVE, NEGATIVE
	};
	
	class EulerOperators {
		
		EulerOperators();
		
		virtual ~EulerOperators();
		
		static void mvfs(Vertex* vtx, Face* face);
		
		void mev(Edge* edge, Vertex* vtx);
		
		void semv(Edge* edge1, Edge* edge2, Vertex* vertex);
		
		void mef(Edge* edge, Face* face);
		
		void kemr(Edge* edge, Loop*);
		
		static void loopSetHe(Loop* loop, HalfEdge* he);
		
		static void heSetLoop(HalfEdge* he, Loop* loop);
		
		static void heSetNext(HalfEdge* he, HalfEdge* next);
		
		static void heSetPrev(HalfEdge* he, HalfEdge* prev);
		
		static void heSetVtx(HalfEdge* he, Vertex* vtx);
		
		static void heSetEdge(HalfEdge* he, Edge* edge);
		
		static void vtxSetHe(Vertex* vtx, HalfEdge* he);
	};
}
#endif