#ifndef DRAWERQTPAINTER_GRID_H
#define DRAWERQTPAINTER_GRID_H

#include <geom/point.h>
#include <utility>

using std::pair;

class Grid {
public:
	Grid();
	
	void setSnapData(bool isSnapOn, double dx, double dy);
	
	bool getSnapInfo() const;
	
	pair<double, double> getGridSpace();
	
	void snapTo(QPointF& pt) const;

protected:
	bool mIsSnapOn = false;
	double mGridX = 1.0;
	double mGridY = 1.0;
};

#endif