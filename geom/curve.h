#ifndef DRAWERQTPAINTER_CURVE_H
#define DRAWERQTPAINTER_CURVE_H

#include "point.h"

#include <vector>
#include <utils/box.h>

using std::vector;

enum class CurveType {
	LINE, QUAD_BEZIER, CUBIC_BEZIER
};

class Curve {
public:
	Curve();
	
	virtual ~Curve();
	
	virtual CurveType type() = 0;
	
	virtual void addPoint(Point p) = 0;
	
	virtual Point getPoint(double t) = 0;
	
	virtual vector<Point> getPoints() = 0;
	
	virtual vector<Point> getPointsToDraw() = 0;
	
	virtual vector<Point> getPointsToDraw(Point p) = 0;
	
	virtual double closestPoint(Point& p) = 0;
	
	virtual Box boundingBox() = 0;
	
	virtual Point getPtStart() = 0;
	
	virtual Point getPtEnd() = 0;
	
	int getNumberOfPoints() const { return mNumPts; }
	
	void selected(bool select) { mSelected = select; }
	
	bool isSelected() const { return mSelected; }
	
	void toggleSelection();
	
	void unselect();
	
	void select();

protected:
	int mNumPts;
	int mSelected;
};


#endif //DRAWERQTPAINTER_CURVE_H
