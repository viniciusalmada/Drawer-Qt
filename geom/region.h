#ifndef DRAWERQTPAINTER_REGION_H
#define DRAWERQTPAINTER_REGION_H

#include <QPointF>
#include <QVector>
#include <QPainterPath>
#include "curve.h"
#include "mesh/delaunaymodel.h"
#include "geom/point.h"

class Region {

public:
	explicit Region(std::vector<std::pair<Curve*, bool>>& curves);
	
	~Region();
	
	QPainterPath pathToFill() const;
	
	bool isSelected() const { return selected; }
	
	void toggleSelection() { selected = !selected; }
	
	void unselect() { selected = false; }
	
	RectUtils::RectF boundingBox() const;
	
	DelaunayModel genDelaunayMesh(int subGrid, int subBoundary);
	
	bool contains(QPointF pt) const;
	
	QVector<QPolygonF>* getMesh() const { return mMesh; };
	
	std::vector<std::pair<QPointF, bool>> getBoundary(int subDiv);
	
	void partitionDomain();

private:
	std::vector<std::pair<Curve*, bool>> mCurves;
	bool selected = false;
	QVector<QPolygonF>* mMesh = nullptr;
};

#endif