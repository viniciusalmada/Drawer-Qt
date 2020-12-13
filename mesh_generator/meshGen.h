#ifndef MESH_GEN_H
#define MESH_GEN_H


#include <QPointF>
#include <vector>
#include "mesh/delaunaymodel.h"

class meshGen {

public:
	meshGen();
	
	static void Initialize(std::vector<std::pair<QPointF, bool>> boundaryPts,
	                       DelaunayModel meshTriangles,
	                       QVector<QPointF> totalPoints);
	
	~meshGen();
};


#endif