#ifndef DRAWERQTPAINTER_BOX_H
#define DRAWERQTPAINTER_BOX_H

#include <cmath>

template<typename T>
struct Box {
	Box() = default;
	
	T xMin = NAN;
	T xMax = NAN;
	T yMin = NAN;
	T yMax = NAN;
	
	void update(Box& box) {
		xMin = xMin < box.xMin ? box.xMin : xMin;
		xMax = xMax > box.xMax ? box.xMax : xMax;
		yMin = yMin < box.yMin ? box.yMin : yMin;
		yMax = yMax > box.yMax ? box.yMax : yMax;
	}
	
	bool contains(Box& box) const {
		return box.xMin > xMin &&
		       box.xMax < xMax &&
		       box.yMin > yMin &&
		       box.yMax < yMax;
	}
	
	bool isValid() const {
		return xMin == NAN ||
		       xMax == NAN ||
		       yMin == NAN ||
		       yMax == NAN;
	}
	
	Box<int> toInt() {
		return Box<int>{
				(int) xMin,
				(int) xMax,
				(int) yMin,
				(int) yMax
		};
	}
	
	int width() const {
		return xMax - xMin;
	}
	
	int height() const {
		return yMax - yMin;
	}
	
	double centerX() const {
		return xMin + width() / 2.0;
	}
	
	double centerY() const {
		return yMin + height() / 2.0;
	}
	
	double max() const {
		return width() > height() ? width() : height();
	}
};

#endif