#include "loop.h"
#include "halfedge.h"
#include "face.h"

HED::Loop::Loop(HED::Face* face) :
		mFace(face),
		mHe(nullptr),
		mNext(nullptr),
		mPrev(nullptr) {
	Loop* faceLoop = face->loop();
	if (faceLoop != nullptr) {
		mNext = faceLoop->mNext;
		mPrev = faceLoop;
		faceLoop->setNext(this);
		if (mNext != nullptr)
			mNext->setPrev(this);
	}
}

void HED::Loop::setHe(HalfEdge* he) {
	this->mHe = he;
}

void HED::Loop::setNext(HED::Loop* next) {
	this->mNext = next;
}

void HED::Loop::setPrev(HED::Loop* prev) {
	this->mPrev = prev;
}

HED::Loop::~Loop() {
	if (this->mPrev != nullptr)
		this->mPrev->setNext(this->mNext);
	if (this->mNext != nullptr)
		this->mNext->setPrev(this->mPrev);
	if (this->mFace != nullptr)
		if (this == mFace->loop())
			mFace->setLoop(nullptr);
}
