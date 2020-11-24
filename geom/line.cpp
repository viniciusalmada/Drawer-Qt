#include <QtCore/QVector>
#include "line.h"

Line::Line() = default;

Line::Line(Point p0, Point p1) : mPt0(p0), mPt1(p1) {

}

void Line::addPoint(Point p) {
	if (mNumPts == 0)
		mPt0 = p;
	else if (mNumPts == 1)
		mPt1 = p;
	mNumPts++;
}

Point Line::getPoint(double t) {
	if (t <= 0.0)
		return mPt0;
	else if (t >= 1.0)
		return mPt1;
	else
		return mPt0 + (mPt1 - mPt0) * t;
}

vector<Point> Line::getPoints() {
	if (mNumPts == 1)
		return vector<Point>{mPt0};
	
	return vector<Point>{mPt0, mPt1};
}

QVector<QPointF> Line::getPointsF() {
	if (mNumPts == 1) {
		return QVector<QPointF>{mPt0.toQF()};
	}
	
	return {mPt0.toQF(), mPt1.toQF()};
}

QVector<QPointF> Line::getPointsToDraw() {
	return {QPointF{mPt0.x, mPt0.y}, QPointF{mPt1.x, mPt1.y},};
}

QVector<QPointF> Line::getPointsToDraw(QPointF p) {
	if (mNumPts == 1) {
		return {mPt0.toQF(), p};
	}
	return getPointsF();
}

double Line::closestPoint(Point& p) {
	double dot = ((mPt1 - mPt0).dot(p - mPt0));
	double t = dot / (mPt1 - mPt0).norm();
	Point closest = getPoint(t);
	double dist = closest.dist(p);
	p = closest;
	return dist;
}

Box<double> Line::boundingBox() {
	double xMin = (mPt0.x < mPt1.x) ? mPt0.x : mPt1.x;
	double xMax = (mPt0.x > mPt1.x) ? mPt0.x : mPt1.x;
	double yMin = (mPt0.y < mPt1.y) ? mPt0.y : mPt1.y;
	double yMax = (mPt0.y > mPt1.y) ? mPt0.y : mPt1.y;
	return {xMin, xMax, yMin, yMax};
}
