#ifndef DRAWERQTPAINTER_CURVE_H
#define DRAWERQTPAINTER_CURVE_H

#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <geom/box.h>

enum class CurveType {
	LINE, QUAD_BEZIER, CUBIC_BEZIER
};

class Curve {
public:
	Curve();
	
	virtual ~Curve();
	
	virtual CurveType type() = 0;
	
	virtual void addPoint(QPointF p) = 0;
	
	virtual QPointF getPoint(double t) = 0;
	
	virtual QVector<QPointF> getPoints() = 0;
	
	virtual QVector<QPointF> getPointsToDraw() = 0;
	
	virtual QVector<QPointF> getPointsToDraw(QPointF p) = 0;
	
	virtual bool isComplete() = 0;
	
	virtual double closestPoint(QPointF& p) = 0;
	
	virtual RectUtils::RectF boundingBox() = 0;
	
	virtual QPointF getPtStart() = 0;
	
	virtual QPointF getPtEnd() = 0;
	
	int numberOfPoints() const { return mNumPts; }
	
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
