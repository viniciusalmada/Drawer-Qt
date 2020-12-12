#ifndef CIRCLEARC_H
#define CIRCLEARC_H

#include "curve.h"

class CircleArc : public Curve {
public:
	CircleArc();
	
	// virtual methods implemented
	CurveType type() override { return CurveType::ARC; }
	
	void addPoint(QPointF p) override;
	
	QPointF getPoint(double t) const override;
	
	QVector<QPointF> getPoints() const override;
	
	QVector<QLineF> getPointsToDraw() const override;
	
	QVector<QLineF> getPointsToDraw(QPointF p) const override;
	
	double closestPoint(QPointF& p) const override;
	
	bool isComplete() override { return mNumPts == 3; };
	
	RectUtils::RectF boundingBox() const override;
	
	QPointF getPtStart() override { return mPtBegin; }
	
	QPointF getPtEnd() override { return mPtEnd; }
	
	double getPhase() const { return mPhase; }
	
	double getRadius() const { return mRadius; }
	
	double getRelativeAngle() const { return mRelativeAngle; }

private:
	QPointF mPtCenter;
	QPointF mPtBegin;
	QPointF mPtEnd;
	
	static const int STEPS;
	double mRadius{};
	double mRelativeAngle{};
	double mPhase{};
	
	static QPointF calculatePt(double t, QPointF center, double rad, double relAngle, double phase);
	
	static QVector<QLineF> calculateLines(QPointF center, QPointF begin, QPointF end, double radius, double phase);
	
	static QVector<QPointF> calculatePoints(QPointF center, QPointF begin, QPointF end, double radius, double phase);
};

#endif // CIRCLEARC_H
