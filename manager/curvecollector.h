#ifndef DRAWERQTPAINTER_CURVECOLLECTOR_H
#define DRAWERQTPAINTER_CURVECOLLECTOR_H

#include "geom/curve.h"

class CurveCollector {
public:
	
	CurveCollector(CurveCollector& other) = delete;
	
	void operator=(const CurveCollector&) = delete;
	
	static CurveCollector* getInstance();
	
	virtual ~CurveCollector();
	
	void setCurveType(CurveType type);
	
	CurveType getCurveType() { return mCurveType; };
	
	void startCollection();
	
	Curve* getCollectedCurve() const { return mCurve; };
	
	void finishCollection();
	
	bool isActive();
	
	bool isCollecting();
	
	bool hasFinished();
	
	bool insertPoint(QPointF pt, double tol);
	
	bool addTempPoint(QPointF pt);
	
	QVector<QLineF> getDrawPoints();
	
	QVector<QPointF> getPoints();
	
	RectUtils::RectF boundingBox();
	
	void reset();

private:
	CurveCollector();
	
	static CurveCollector* collector;
	
	CurveType mCurveType = CurveType::LINE;
	Curve* mCurve = nullptr;
	QPointF mPrevPt{};
	QPointF mTempPt{};
};

#endif