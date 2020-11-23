//
// Created by vini on 11/23/20.
//

#include "line.h"

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

vector<Point> Line::getPointsToDraw() {
	return getPoints();
}

vector<Point> Line::getPointsToDraw(Point p) {
	if (mNumPts == 1)
		return vector<Point>{mPt0, p};
	return getPoints();
}

double Line::closestPoint(Point& p) {
	double dot = ((mPt1 - mPt0).dot(p - mPt0));
	double t = dot / (mPt1 - mPt0).norm();
	Point closest = getPoint(t);
	double dist = closest.dist(p);
	p = closest;
	return dist;
}

void Line::getBoundBox(double& xMin, double& xMax, double& yMin, double& yMax) {
	xMin = (mPt0.x < mPt1.x) ? mPt0.x : mPt1.x;
	xMax = (mPt0.x > mPt1.x) ? mPt0.x : mPt1.x;
	yMin = (mPt0.y < mPt1.y) ? mPt0.y : mPt1.y;
	yMax = (mPt0.y > mPt1.y) ? mPt0.y : mPt1.y;
}