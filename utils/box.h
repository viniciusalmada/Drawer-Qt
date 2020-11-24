#ifndef DRAWERQTPAINTER_BOX_H
#define DRAWERQTPAINTER_BOX_H

struct Box {
	Box() = default;
	
	Box(double xmi, double xma, double ymi, double yma) {
		xMin = xmi;
		xMax = xma;
		yMin = ymi;
		yMax = yma;
	}
	
	double xMin = NAN;
	double xMax = NAN;
	double yMin = NAN;
	double yMax = NAN;
	
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