#ifndef DRAWERQTPAINTER_CUBIC_BEZIER_H
#define DRAWERQTPAINTER_CUBIC_BEZIER_H

#include "curve.h"

class CubicBezier : public Curve {

public:
	CubicBezier();
	
	void addPoint(QPointF p) override;
	
	QPointF getPoint(double t) const override;
	
	QVector<QPointF> getPoints() const override;
	
	QVector<QLineF> getPointsToDraw() const override;
	
	QVector<QLineF> getPointsToDraw(QPointF p) const override;
	
	double closestPoint(QPointF& p) const override;
	
	bool isComplete() override { return mNumPts == 4; };
	
	RectUtils::RectF boundingBox() const override;
	
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
