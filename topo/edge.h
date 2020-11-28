#ifndef DRAWER_HED_EDGE_H
#define DRAWER_HED_EDGE_H

namespace HED {
	class HalfEdge;
	
	class Edge {
		HalfEdge* mHe1;
		HalfEdge* mHe2;
	public:
		Edge();
		
		virtual ~Edge();
	};
}

#endif