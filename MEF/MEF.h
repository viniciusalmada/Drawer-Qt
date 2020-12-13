#ifndef MEF_H
#define MEF_H

#include <QPointF>
#include "matrix.h"
#include "smatrix.h"
#include <vector>
#include "mesh/delaunaymodel.h"

using std::vector;

class MEF {
public:
	DelaunayModel m_model;
	QVector<QPolygonF> m_triangles;
	vector<QPointF> m_points;
	int m_T, m_N, m_nB;
	SMatrix m_SKG{m_N + m_nB, m_N + m_nB};


public:
	MEF(DelaunayModel& model, const vector<QPointF>& total, int nB);
	
	Matrix solver(Matrix b);
	
	double area2D(QPointF a, QPointF b, QPointF c);
	
	~MEF();
	
	
};

#endif