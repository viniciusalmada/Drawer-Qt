#ifndef DRAWER_HED_VERTEX_H
#define DRAWER_HED_VERTEX_H

#include <set>

namespace HED {
	class HalfEdge;
	
	class Face;
	
	class Edge;
	
	class Vertex {
		
		HalfEdge* mHe;
	public:
		Vertex();
		
		virtual ~Vertex();
		
		std::set<Face*> nearFaces();
		
		std::set<Face*> nearEdges();
		
		std::set<Face*> nearVertices();
		
		HalfEdge* he();
		
		void setHe(HalfEdge* he);
	};
}

#endif