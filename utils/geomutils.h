#ifndef GEOMUTILS_H
#define GEOMUTILS_H

#include <cmath>

namespace GeomUtils {
	static double ceil2(double num, double interval) {
		return num - fmod(num, interval);
	}
}

#endif