
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

Point Grid::snapTo(const Point& pt) {
	Point pivot = pt / Point{mGridX, mGridY};
	pivot = pivot.trim() * Point{mGridX, mGridY};
	Point p1 = pivot + Point{pivot.x + mGridX, pivot.y};
	Point p2 = pivot + Point{pivot.x + mGridX, pivot.y + mGridY};
	Point p3 = pivot + Point{pivot.x, pivot.y + mGridY};
	
	return pt.closestPoint({pivot, p1, p2, p3});
}
