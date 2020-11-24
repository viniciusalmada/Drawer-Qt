
#include <geom/line.h>
#include "curvecollector.h"

CurveCollector::CurveCollector() = default;

CurveCollector::~CurveCollector() {
	delete mCurve;
};

void CurveCollector::setCurveType(CurveType type) {
	mCurveType = type;
}

void CurveCollector::startCollection() {
	delete mCurve;
	
	switch (mCurveType) {
		case CurveType::LINE:
			mCurve = new Line();
			break;
		default:
			break;
	}
}

void CurveCollector::finishCollection() {
	mCurve = nullptr;
}

bool CurveCollector::isActive() {
	return mCurve != nullptr;
}

bool CurveCollector::isCollecting() {
	return isActive() && mCurve->numberOfPoints() > 0;
}

bool CurveCollector::hasFinished() {
	return isActive() && mCurve->isComplete();
}

bool CurveCollector::insertPoint(Point pt, double tol) {
	if (isCollecting()) {
		if (pt.dist(mPrevPt) <= tol)
			return false;
	}
	
	mCurve->addPoint(pt);
	mPrevPt = pt;
	return true;
}

bool CurveCollector::addTempPoint(Point pt) {
	mTempPt = pt;
	return true;
}

vector<Point> CurveCollector::getDrawPoints() {
	return mCurve->getPointsToDraw(mTempPt);
}

vector<Point> CurveCollector::getPoints() {
	return mCurve->getPoints();
}

Box CurveCollector::getBoundBox() {
	if (mCurve != nullptr)
		return Box{};
	
	return mCurve->boundingBox();
}

void CurveCollector::reset() {
	delete mCurve;
	mCurve = nullptr;
}




