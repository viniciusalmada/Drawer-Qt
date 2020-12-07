#ifndef MEF_H
#define MEF_H

#include "triangle.h"
#include <QPointF>
#include "matrix.h"
#include "smatrix.h"


class MEF
{
public:
	vector<Triangle> m_triangles;
	vector<QPointF> m_points;
	int m_T, m_N, m_nB;
	SMatrix m_SKG{m_N + m_nB, m_N + m_nB};


public:
	MEF(vector<Triangle> triangle, vector<QPointF> total, int nB);
	Matrix solver(Matrix b);
	double area2D(QPointF a, QPointF b, QPointF c);
	~MEF();


};

#endif