#ifndef DRAWERQTPAINTER_CURVECOLLECTOR_H
#define DRAWERQTPAINTER_CURVECOLLECTOR_H

#include "geom/curve.h"

class CurveCollector {
public:
	CurveCollector();
	
	virtual ~CurveCollector();
	
	void setCurveType(CurveType type);
	
	CurveType getCurveType() { return mCurveType; };
	
	void startCollection();
	
	Curve* getCollectedCurve() const { return mCurve; };
	
	void finishCollection();
	
	bool isActive();
	
	bool isCollecting();
	
	bool hasFinished();
	
	bool insertPoint(Point pt, double tol);
	
	bool addTempPoint(Point pt);
	
	vector<Point> getDrawPoints();
	
	vector<Point> getPoints();
	
	Box getBoundBox();
	
	void reset();
	
	void kill();

private:
	CurveType mCurveType = CurveType::LINE;
	Curve* mCurve = nullptr;
	Point mPrevPt{};
	Point mTempPt{};
};

#endif