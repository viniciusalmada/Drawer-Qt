#include "circlearc.h"
#include "point.h"

#include <cmath>
#include <cfloat>

const int CircleArc::STEPS = 50;

//-------------------------------------------------------------------------
CircleArc::CircleArc() = default;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void CircleArc::addPoint(QPointF pt) {
	if (mNumPts == 0) {
		mPtCenter = pt;
	} else if (mNumPts == 1) {
		mPtBegin = pt;
		mRadius = PointUtils::dist(mPtCenter, pt);
		double dot = QPointF::dotProduct(PointUtils::normalize(mPtBegin - mPtCenter), {1, 0});
		double cross = PointUtils::crossProd({1, 0}, {mPtBegin - mPtCenter});
		mPhase = acos(dot);
		if (cross <= 0.0) {
			mPhase = acos(-dot);
			mPhase += M_PI;
		}
	} else if (mNumPts == 2) {
		mPtEnd = pt;
		double dot = QPointF::dotProduct(PointUtils::normalize(mPtBegin - mPtCenter),
		                                 PointUtils::normalize(pt - mPtCenter));
		double cross = PointUtils::crossProd(mPtBegin - mPtCenter, pt - mPtCenter);
		mRelativeAngle = acos(dot);
		if (cross <= 0.0) {
			mRelativeAngle = acos(-dot);
			mRelativeAngle += M_PI;
		}
	}
	mNumPts++;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QPointF CircleArc::getPoint(double t) const {
	return calculatePt(t, mPtCenter, mRadius, mRelativeAngle, mPhase);
}

QPointF CircleArc::calculatePt(double t, QPointF center, double rad, double relAngle, double phase) {
	double x = rad * cos(relAngle * t + phase);
	double y = rad * sin(relAngle * t + phase);
	return center + QPointF{x, y};
}
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
QVector<QPointF> CircleArc::getPoints() const {
	return {mPtCenter, mPtBegin, mPtEnd};
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QVector<QLineF> CircleArc::getPointsToDraw() const {
	return calculateLines(mPtCenter, mPtBegin, mPtEnd, mRadius, mPhase);
}

QVector<QLineF> CircleArc::calculateLines(QPointF center, QPointF begin, QPointF end, double radius, double phase) {
	QVector<QLineF> lines{};
	double dot = QPointF::dotProduct(PointUtils::normalize(begin - center), PointUtils::normalize(end - center));
	double cross = PointUtils::crossProd(begin - center, end - center);
	double relAngle = acos(dot);
	
	if (cross <= 0.0) {
		relAngle = acos(-dot);
		relAngle = M_PI + relAngle;
	}
	
	for (int i = 0; i < STEPS; i++) {
		double t0 = phase + i * (relAngle) / STEPS;
		double t1 = phase + (i + 1) * (relAngle) / STEPS;
		QPointF pt0{radius * cos(t0), radius * sin(t0)};
		QPointF pt1{radius * cos(t1), radius * sin(t1)};
		lines.push_back({pt0 + center, pt1 + center});
	}
	
	return lines;
}

QVector<QPointF> CircleArc::calculatePoints(QPointF center, QPointF begin, QPointF end, double radius, double phase) {
	QVector<QPointF> pts;
	double dot = QPointF::dotProduct(PointUtils::normalize(begin - center), PointUtils::normalize(end - center));
	double cross = PointUtils::crossProd(begin - center, end - center);
	double relAngle = acos(dot);
	
	if (cross <= 0.0) {
		relAngle = acos(-dot);
		relAngle = M_PI + relAngle;
	}
	
	pts.push_back(begin);
	for (int i = 0; i < STEPS; i++) {
		double t = phase + (i + 1) * (relAngle) / STEPS;
		QPointF pt{radius * cos(t), radius * sin(t)};
		pts.push_back(pt + center);
	}
	pts.push_back(end);
	
	return pts;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QVector<QLineF> CircleArc::getPointsToDraw(QPointF pt) const {
	if (mNumPts == 1) {
		return {QLineF{mPtCenter, pt}};
	} else if (mNumPts == 2) {
		return calculateLines(mPtCenter, mPtBegin, pt, mRadius, mPhase);
	}
	return {};
}

double CircleArc::closestPoint(QPointF& p) const {
	QVector<QPointF> pts = calculatePoints(mPtCenter, mPtBegin, mPtEnd, mRadius, mPhase);
	double dist = DBL_MAX;
	QPointF tempPt = p;
	for (const QPointF& pt : pts) {
		double d = PointUtils::dist(pt, p);
		if (d < dist) {
			dist = d;
			tempPt = pt;
		}
	}
	
	p = tempPt;
	
	return dist;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
RectUtils::RectF CircleArc::boundingBox() const {
	QVector<QPointF> pts = calculatePoints(mPtCenter, mPtBegin, mPtEnd, mRadius, mPhase);;
	auto compX = [](const QPointF& l, const QPointF& r) { return l.x() < r.x(); };
	auto compY = [](const QPointF& l, const QPointF& r) { return l.y() < r.y(); };
	double xMin = std::min_element(pts.begin(), pts.end(), compX)->x();
	double yMin = std::min_element(pts.begin(), pts.end(), compY)->y();
	double xMax = std::max_element(pts.begin(), pts.end(), compX)->x();
	double yMax = std::max_element(pts.begin(), pts.end(), compY)->y();
	return {xMin, yMin, xMax - xMin, yMax - yMin};
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
