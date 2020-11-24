#ifndef DRAWERQTPAINTER_LINE_H
#define DRAWERQTPAINTER_LINE_H

#include "curve.h"

class Line : public Curve {

public:
	Line();
	
	Line(QPointF p0, QPointF p1);
	
	CurveType type() override { return CurveType::LINE; }
	
	void addPoint(QPointF p) override;
	
	QPointF getPoint(double t) override;
	
	QVector<QPointF> getPoints() override;
	
	QVector<QPointF> getPointsToDraw() override;
	
	QVector<QPointF> getPointsToDraw(QPointF p) override;
	
	double closestPoint(QPointF& p) override;
	
	bool isComplete() override { return mNumPts == 2; };
	
	QRectF boundingBox() override;
	
	QPointF getPtStart() override { return mPt0; }
	
	QPointF getPtEnd() override { return mPt1; }

private:
	QPointF mPt0;
	QPointF mPt1;
};


#endif //DRAWERQTPAINTER_LINE_H
