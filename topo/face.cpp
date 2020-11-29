#include "face.h"
#include "loop.h"

HED::Loop* HED::Face::loop() {
	return mLoop;
}

void HED::Face::setLoop(HED::Loop* loop) {
	this->mLoop = loop;
}
