#ifndef DRAWERQTPAINTER_MODEL_H
#define DRAWERQTPAINTER_MODEL_H

#include <vector>
#include <QtCore/QRectF>
#include <geom/region.h>
#include <mesh/delaunaymodel.h>
#include "geom/curve.h"

class Model {
public:
	Model();
	
	~Model();
	
	const std::vector<Curve*>& curves() { return mCurves; }
	
	const std::vector<Region>& regions() { return mRegions; }
	
	RectUtils::RectF boundingBox();
	
	void selectPick(QPointF pt, double tol, bool shiftKey = false);
	
	void selectFence(const RectUtils::RectF& box, bool shiftKey = false);
	
	void insertCurve(Curve* curve);
	
	void delCurve(int curveId);
	
	void delRegion(int id);
	
	void delSelectedCurves();
	
	void delSelectedRegions();
	
	bool snapToCurve(QPointF& pt, double tol);
	
	bool isEmpty();
	
	void createRegion();
	
	void genDelaunay(int sub);
	
	void startPartitionDomain();

protected:
	std::vector<Curve*> mCurves;
	std::vector<Region> mRegions;
	
	void delAllCurves();
};


#endif //DRAWERQTPAINTER_MODEL_H
