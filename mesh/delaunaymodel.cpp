#include "delaunaymodel.h"
#include <geom/geomutils.h>

DelaunayModel::DelaunayModel(const std::vector<QPointF>& points) {
	std::array<int, 3> triangleBox = GeomUtils::triangleBox(points);
	
}
