#ifndef GEOM_UTILS_H
#define GEOM_UTILS_H

#include <cmath>

namespace GeomUtils {
	static double ceil2(double num, double interval) {
		return num - fmod(num, interval);
	}
}

#endif