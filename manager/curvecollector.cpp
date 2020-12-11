
#include <geom/line.h>
#include "curvecollector.h"
#include <QVector>
#include <geom/point.h>
#include <geom/cubicbezier.h>

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
		case CurveType::CUBIC_BEZIER:
			mCurve = new CubicBezier();
			break;
		default:
			break;
	}
}

void CurveCollector::finishCollection() {
	mPrevPt = QPointF{};
	mTempPt = QPointF{};
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

bool CurveCollector::insertPoint(QPointF pt, double tol) {
	if (isCollecting()) {
		if (PointUtils::dist(pt, mPrevPt) <= tol)
			return false;
	}
	
	mCurve->addPoint(pt);
	mPrevPt = pt;
	return true;
}

bool CurveCollector::addTempPoint(QPointF pt) {
	mTempPt = pt;
	return true;
}

QVector<QLineF> CurveCollector::getDrawPoints() {
	return mCurve->getPointsToDraw(mTempPt);
}

QVector<QPointF> CurveCollector::getPoints() {
	return mCurve->getPoints();
}

RectUtils::RectF CurveCollector::boundingBox() {
	if (mCurve != nullptr)
		return RectUtils::RectF{};
	
	return mCurve->boundingBox();
}

void CurveCollector::reset() {
	delete mCurve;
	mPrevPt = QPointF{};
	mTempPt = QPointF{};
	mCurve = nullptr;
}

CurveCollector* CurveCollector::collector = nullptr;

CurveCollector* CurveCollector::getInstance() {
	if (collector == nullptr)
		collector = new CurveCollector();
	return collector;
}




