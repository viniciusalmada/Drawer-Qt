#include <QtCore/QVector>
#include <QtCore/QRectF>
#include <array>
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

QPointF Line::getPoint(double t) const {
	if (t <= 0.0)
		return mPt0;
	else if (t >= 1.0)
		return mPt1;
	else
		return mPt0 + (mPt1 - mPt0) * t;
}

QVector<QPointF> Line::getPoints() const {
	if (mNumPts == 1)
		return QVector<QPointF>{mPt0};
	
	return QVector<QPointF>{mPt0, mPt1};
}

QVector<QLineF> Line::getPointsToDraw() const {
	QVector<QPointF> pts = getPoints();
	return {QLineF{pts[0], pts[1]}};
}

QVector<QLineF> Line::getPointsToDraw(QPointF p) const {
	if (mNumPts == 1)
		return {QLineF{mPt0, p}};
	
	return getPointsToDraw();
}

double Line::closestPoint(QPointF& p) const {
	double dot = QPointF::dotProduct(mPt1 - mPt0, p - mPt0);
	double t = dot / pow(PointUtils::norm(mPt1 - mPt0), 2);
	QPointF closest = getPoint(t);
	double dist = PointUtils::dist(closest, p);
	p = closest;
	
	std::vector<QPointF> nmm{{1.0, 2.3}};
	std::array<std::vector<QPointF>, 3> f{{nmm, nmm, nmm}};
	
	return dist;
}

RectUtils::RectF Line::boundingBox() const {
	double xMin = (mPt0.x() < mPt1.x()) ? mPt0.x() : mPt1.x();
	double xMax = (mPt0.x() > mPt1.x()) ? mPt0.x() : mPt1.x();
	double yMin = (mPt0.y() < mPt1.y()) ? mPt0.y() : mPt1.y();
	double yMax = (mPt0.y() > mPt1.y()) ? mPt0.y() : mPt1.y();
	return {xMin, yMin, xMax - xMin, yMax - yMin};
}
