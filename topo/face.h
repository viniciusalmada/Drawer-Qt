#ifndef DRAWER_HED_FACE_H
#define DRAWER_HED_FACE_H

namespace HED {
	
	class Loop;
	
	class Face {
		
		Loop* mLoop;
	
	public:
		Face();
		
		virtual ~Face();
		
		Loop* loop();
		
		void setLoop(Loop* loop);
	};
}
#endif