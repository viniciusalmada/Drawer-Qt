#include <QtCore/QVector>
#include <QtCore/QRectF>
#include "line.h"
#include "point.h"

Line::Line() = default;

Line::Line(QPointF p0, QPointF p1) : mPt0(p0), mPt1(p1) {

}

void Line::addPoint(QPointF p) {
	if (mNumPts == 0)
		mPt0 = p;
	else if (mNumPts == 1)
		mPt1 = p;
	mNumPts++;
}

QPointF Line::getPoint(double t) {
	if (t <= 0.0)
		return mPt0;
	else if (t >= 1.0)
		return mPt1;
	else
		return mPt0 + (mPt1 - mPt0) * t;
}

QVector<QPointF> Line::getPoints() {
	if (mNumPts == 1)
		return QVector<QPointF>{mPt0};
	
	return QVector<QPointF>{mPt0, mPt1};
}

QVector<QPointF> Line::getPointsToDraw() {
	return getPoints();
}

QVector<QPointF> Line::getPointsToDraw(QPointF p) {
	if (mNumPts == 1) {
		return {mPt0, p};
	}
	return getPoints();
}

double Line::closestPoint(QPointF& p) {
	double dot = QPointF::dotProduct(mPt1 - mPt0, p - mPt0);
	double t = dot / PointUtils::norm(mPt1 - mPt0);
	QPointF closest = getPoint(t);
	double dist = PointUtils::dist(closest, p);
	p = closest;
	return dist;
}

RectUtils::RectF Line::boundingBox() {
	double xMin = (mPt0.x() < mPt1.x()) ? mPt0.x() : mPt1.x();
	double xMax = (mPt0.x() > mPt1.x()) ? mPt0.x() : mPt1.x();
	double yMin = (mPt0.y() < mPt1.y()) ? mPt0.y() : mPt1.y();
	double yMax = (mPt0.y() > mPt1.y()) ? mPt0.y() : mPt1.y();
	return {xMin, yMax, xMax - xMin, yMax - yMin};
}
