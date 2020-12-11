#ifndef DRAWERQTPAINTER_REGION_H
#define DRAWERQTPAINTER_REGION_H

#include <QPointF>
#include <QVector>
#include <QPainterPath>
#include "curve.h"

class Region {
public:
	explicit Region(std::vector<std::pair<Curve*, bool>>& curves);
	
	QPainterPath pathToFill() const;
	
	bool isSelected() const { return selected; }

private:
	std::vector<std::pair<Curve*, bool>> mCurves;
	bool selected = false;
};

#endif