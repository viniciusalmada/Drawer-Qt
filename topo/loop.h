#ifndef DRAWER_HED_LOOP_H
#define DRAWER_HED_LOOP_H

namespace HED {
	
	class Face;
	
	class HalfEdge;
	
	class Loop {
		
		Loop* mPrev;
		Loop* mNext;
		HalfEdge* mHe;
		Face* mFace;
	
	public:
		explicit Loop(Face* face);
		
		virtual ~Loop();
		
		void setHe(HalfEdge* he);
		
		void setNext(Loop* next);
		
		void setPrev(Loop* prev);
	};
}

#endif