
#include "grid.h"

Grid::Grid() = default;

void Grid::reset() {
	mIsSnapOn = false;
	mGridX = 1.0;
	mGridY = 1.0;
	
}

void Grid::setSnapData(bool isSnapOn, double dx, double dy) {
	mGridX = dx;
	mGridY = dy;
	mIsSnapOn = isSnapOn;
}

bool Grid::getSnapInfo() const {
	return mIsSnapOn;
}

pair<double, double> Grid::getGridSpace() {
	return {mGridX, mGridY};
}

void Grid::snapTo(QPointF& pt) const {
	QPointF pivotF{};
	pivotF.rx() = pt.x() / mGridX;
	pivotF.ry() = pt.y() / mGridY;
	QPoint pivot = pivotF.toPoint();
	pivot.rx() = (int) (pivot.rx() * mGridX);
	pivot.ry() = (int) (pivot.ry() * mGridY);
	QPoint p1 = pivot + QPoint{static_cast<int>(pivot.x() + mGridX), pivot.y()};
	QPoint p2 = pivot + QPoint{static_cast<int>(pivot.x() + mGridX), static_cast<int>(pivot.y() + mGridY)};
	QPoint p3 = pivot + QPoint{pivot.x(), static_cast<int>(pivot.y() + mGridY)};
	
	pt = PointUtils::closestPoint(pt, {pivot, p1, p2, p3});
}
