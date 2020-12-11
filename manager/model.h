#ifndef DRAWERQTPAINTER_MODEL_H
#define DRAWERQTPAINTER_MODEL_H

#include <vector>
#include <QtCore/QRectF>
#include "geom/curve.h"

class Model {
public:
	Model();
	
	~Model();
	
	const std::vector<Curve*>& curves() { return mCurves; }
	
	RectUtils::RectF boundingBox();
	
	void selectPick(QPointF pt, double tol, bool shiftKey = false);
	
	void selectFence(RectUtils::RectF box, bool shiftKey = false);
	
	void insertCurve(Curve* curve);
	
	void delCurve(int curveId);
	
	void delSelectedCurves();
	
	bool snapToCurve(QPointF& pt, double tol);
	
	bool isEmpty();

protected:
	std::vector<Curve*> mCurves;
	
	void delAllCurves();
};


#endif //DRAWERQTPAINTER_MODEL_H
