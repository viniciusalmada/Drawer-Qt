#ifndef DRAWERQTPAINTER_MODEL_H
#define DRAWERQTPAINTER_MODEL_H

#include "geom/curve.h"

class Model {
public:
	Model();
	
	~Model();
	
	vector<Curve*> curves() { return mCurves; }
	
	Box boundingBox();
	
	void selectPick(Point pt, double tol, bool shiftKey = false);
	
	void selectFence(Box box, bool shiftKey = false);
	
	void insertCurve(Curve* curve);
	
	void delCurve(int curveId);
	
	void delSelectedCurves();
	
	bool snapToCurve(Point& pt, double tol);

protected:
	vector<Curve*> mCurves;
	
	void delAllCurves();
};


#endif //DRAWERQTPAINTER_MODEL_H
