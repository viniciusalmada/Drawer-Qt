#ifndef GEOM_UTILS_H
#define GEOM_UTILS_H

#include <QPointF>

#include <cmath>
#include <vector>

namespace GeomUtils {
	static double ceil2(double num, double interval) {
		return num - fmod(num, interval);
	}
	
	static bool edgeContains(const QPointF& pt0, const QPointF& pt1, const QPointF& pt);
	
	static bool polygonContains(const std::vector<QPointF>& pts, QPointF pt);
	
	static std::array<int, 3> triangleBox(const std::vector<QPointF>& pts);
}

#endif