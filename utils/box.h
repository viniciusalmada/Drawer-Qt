#ifndef DRAWERQTPAINTER_BOX_H
#define DRAWERQTPAINTER_BOX_H

template<typename T>
struct Box {
	Box() = default;
	
	Box(T xmi, T xma, T ymi, T yma) {
		xMin = xmi;
		xMax = xma;
		yMin = ymi;
		yMax = yma;
	}
	
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
};

#endif