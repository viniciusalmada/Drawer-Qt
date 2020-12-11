#ifndef DRAWERQTPAINTER_LINE_H
#define DRAWERQTPAINTER_LINE_H

#include "curve.h"

class Line : public Curve {

public:
	Line();
	
	Line(QPointF p0, QPointF p1);
	
	CurveType type() override { return CurveType::LINE; }
	
	void addPoint(QPointF p) override;
	
	QPointF getPoint(double t) const override;
	
	QVector<QPointF> getPoints() const override;
	
	QVector<QLineF> getPointsToDraw() const override;
	
	QVector<QLineF> getPointsToDraw(QPointF p) const override;
	
	double closestPoint(QPointF& p) const override;
	
	bool isComplete() override { return mNumPts == 2; };
	
	RectUtils::RectF boundingBox() const override;
	
	QPointF getPtStart() override { return mPt0; }
	
	QPointF getPtEnd() override { return mPt1; }

private:
	QPointF mPt0;
	QPointF mPt1;
};


#endif //DRAWERQTPAINTER_LINE_H
