#ifndef DRAWERQTPAINTER_CUBIC_BEZIER_H
#define DRAWERQTPAINTER_CUBIC_BEZIER_H

#include "curve.h"

class CubicBezier : public Curve {

public:
	CubicBezier();
	
	CurveType type() override { return CurveType::CUBIC_BEZIER; }
	
	void addPoint(QPointF p) override;
	
	QPointF getPoint(double t) override;
	
	QVector<QPointF> getPoints() override;
	
	QVector<QLineF> getPointsToDraw() override;
	
	QVector<QLineF> getPointsToDraw(QPointF p) override;
	
	double closestPoint(QPointF& p) override;
	
	bool isComplete() override { return mNumPts == 4; };
	
	RectUtils::RectF boundingBox() override;
	
	QPointF getPtStart() override { return mPt0; }
	
	QPointF getPtEnd() override { return mPt1; }

private:
	QPointF mPt0;
	QPointF mPt1;
	QPointF mPt0c;
	QPointF mPt1c;
	
	static const int DISCRETE_PTS_TO_DRAW;
	
	static QPointF calculatePt(double t, QPointF p0, QPointF p1, QPointF p0c, QPointF p1c);
	
	static QVector<QLineF>
	calculateMultipleLines(const QPointF& p0, const QPointF& p1, const QPointF& p0c, const QPointF& p1c);
	
	static QVector<QPointF>
	calculateMultiplePts(const QPointF& p0, const QPointF& p1, const QPointF& p0c, const QPointF& p1c);
};


#endif //DRAWERQTPAINTER_LINE_H
