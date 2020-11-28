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
		
		void mvfs(Vertex* vtx, Face* face);
		
		void mev(Edge* edge, Vertex* vtx);
		
		void semv(Edge* edge1, Edge* edge2, Vertex* vertex);
		
		void mef(Edge* edge, Face* face);
		
		void kemr(Edge* edge, Loop*)
	};
}
#endif