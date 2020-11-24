#ifndef DRAWERQTPAINTER_LINE_H
#define DRAWERQTPAINTER_LINE_H

#include "curve.h"

class Line : public Curve {

public:
	Line();
	
	Line(Point p0, Point p1);
	
	CurveType type() override { return CurveType::LINE; }
	
	void addPoint(Point p) override;
	
	Point getPoint(double t) override;
	
	vector<Point> getPoints() override;
	
	QVector<QPointF> getPointsF() override;
	
	QVector<QPointF> getPointsToDraw() override;
	
	QVector<QPointF> getPointsToDraw(QPointF p) override;
	
	double closestPoint(Point& p) override;
	
	bool isComplete() override { return mNumPts == 2; };
	
	Box<double> boundingBox() override;
	
	Point getPtStart() override { return mPt0; }
	
	Point getPtEnd() override { return mPt1; }

private:
	Point mPt0;
	Point mPt1;
};


#endif //DRAWERQTPAINTER_LINE_H
