#include "curve.h"


Curve::Curve() : mNumPts(0), mSelected(0) {

}

void Curve::toggleSelection() {
	mSelected = !mSelected;
}

void Curve::unselect() {
	mSelected = false;
	
}

void Curve::select() {
	mSelected = true;
}

Curve::~Curve() = default;
