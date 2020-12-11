#include "cubicbezier.h"
#include <QtCore/QVector>
#include <QtCore/QRectF>
#include "point.h"
#include <cfloat>

const int CubicBezier::DISCRETE_PTS_TO_DRAW = 35;

CubicBezier::CubicBezier() = default;

void CubicBezier::addPoint(QPointF p) {
	if (mNumPts == 0)
		mPt0 = p;
	else if (mNumPts == 1)
		mPt1 = p;
	else if (mNumPts == 2)
		mPt1c = mPt0c = p;
	else if (mNumPts == 3)
		mPt0c = p;
	mNumPts++;
}

QPointF CubicBezier::getPoint(double t) const {
	if (t <= 0.0)
		return mPt0;
	else if (t >= 1.0)
		return mPt1;
	else {
		return calculatePt(t, mPt0, mPt1, mPt0c, mPt1c);
	}
}

QVector<QPointF> CubicBezier::getPoints() const {
	if (mNumPts == 1)
		return QVector<QPointF>{mPt0};
	if (mNumPts == 2)
		return QVector<QPointF>{mPt0, mPt1};
	if (mNumPts == 3)
		return QVector<QPointF>{mPt0, mPt1, mPt1c};
	
	return QVector<QPointF>{mPt0, mPt0c, mPt1c, mPt1};
}

QVector<QLineF> CubicBezier::getPointsToDraw() const {
	return calculateMultipleLines(mPt0, mPt1, mPt0c, mPt1c);
}

QVector<QLineF> CubicBezier::getPointsToDraw(QPointF p) const {
	if (mNumPts == 1) {
		return {QLineF{mPt0, p}};
	} else if (mNumPts == 2) {
		return calculateMultipleLines(mPt0, mPt1, p, p);
	} else if (mNumPts == 3) {
		return calculateMultipleLines(mPt0, mPt1, p, mPt1c);
	}
	return getPointsToDraw();
}

double CubicBezier::closestPoint(QPointF& p) const {
	QVector<QPointF> pts = calculateMultiplePts(mPt0, mPt1, mPt0c, mPt1c);
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

RectUtils::RectF CubicBezier::boundingBox() const {
	QVector<QPointF> pts = calculateMultiplePts(mPt0, mPt1, mPt0c, mPt1c);
	auto compX = [](const QPointF& l, const QPointF& r) { return l.x() < r.x(); };
	auto compY = [](const QPointF& l, const QPointF& r) { return l.y() < r.y(); };
	double xMin = std::min_element(pts.begin(), pts.end(), compX)->x();
	double yMin = std::min_element(pts.begin(), pts.end(), compY)->y();
	double xMax = std::max_element(pts.begin(), pts.end(), compX)->x();
	double yMax = std::max_element(pts.begin(), pts.end(), compY)->y();
	return {xMin, yMin, xMax - xMin, yMax - yMin};
}

QPointF CubicBezier::calculatePt(double t, QPointF p0, QPointF p1, QPointF p0c, QPointF p1c) {
	double b0 = (1.0 - t) * (1.0 - t) * (1.0 - t);
	double b0c = 3.0 * (1.0 - t) * (1.0 - t) * t;
	double b1c = 3 * (1.0 - t) * t * t;
	double b1 = t * t * t;
	
	return p0 * b0 + p0c * b0c + p1c * b1c + p1 * b1;
}

QVector<QLineF>
CubicBezier::calculateMultipleLines(const QPointF& p0, const QPointF& p1, const QPointF& p0c, const QPointF& p1c) {
	QVector<QLineF> lines{DISCRETE_PTS_TO_DRAW};
	for (int i = 0; i < DISCRETE_PTS_TO_DRAW; i++) {
		double t0 = (double) i / (double) DISCRETE_PTS_TO_DRAW;
		double t1 = (double) (i + 1) / (double) DISCRETE_PTS_TO_DRAW;
		QPointF pt0 = calculatePt(t0, p0, p1, p0c, p1c);
		QPointF pt1 = calculatePt(t1, p0, p1, p0c, p1c);
		lines[i] = QLineF{pt0, pt1};
	}
	return lines;
}

QVector<QPointF>
CubicBezier::calculateMultiplePts(const QPointF& p0, const QPointF& p1, const QPointF& p0c, const QPointF& p1c) {
	QVector<QPointF> pts{DISCRETE_PTS_TO_DRAW};
	for (int i = 0; i < DISCRETE_PTS_TO_DRAW; i++) {
		double t = (double) i / (double) DISCRETE_PTS_TO_DRAW;
		QPointF pt0 = calculatePt(t, p0, p1, p0c, p1c);
		pts[i] = pt0;
	}
	return pts;
}
