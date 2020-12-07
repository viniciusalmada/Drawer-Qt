#ifndef SINGULAR_H
#define SINGULAR_H

#include <vector>
#include <utility>      // std::pair

#include <QPointF>
#include "triangle.h"

using std::pair;


class Singular
{
public:
	QPointF m_pt;
	Triangle m_tri;
	int m_idxPoinc;
	vector<Triangle*> m_triSing;
	vector <QPointF> m_pEdge1;
	vector <QPointF> m_pEdge2;
	vector <QPointF> m_pEdge3;
	vector <QPointF> m_dEdge1;
	vector <QPointF> m_dEdge2;
	vector <QPointF> m_dEdge3;
	vector <double> m_tEdge1;
	vector <double> m_tEdge2;
	vector <double> m_tEdge3;
	vector <QPointF> m_vRpr; 
	vector<QPointF> m_total;
	vector<vector<QPointF>> m_curveStreamlines;
	

public:
	Singular(QPointF pt, Triangle tri, int idxPoinc, vector <QPointF> vRpr, vector<QPointF> total);
	void Initialize();
	void Integrate();
	void BeginIntercession(QPointF pi, QPointF pj, QPointF vi, QPointF vj, vector <QPointF> *m_pEdgei, vector <QPointF> *m_vEdgei, vector <double> *m_tEdgei);
	static int orient2D(QPointF u, QPointF v);
	static QPointF Interpolate(QPointF u, QPointF v, double t);
	static vector <QPointF> CreateCross(QPointF v);
	static double orient2D(QPointF a, QPointF b, QPointF c);
	QPointF findNearestVector(vector<QPointF> cross, QPointF v);
	static double to2PI(double angle);
	pair<double, vector<int>> Intercept(QPointF pi, QPointF vi, vector<int> edgei, Triangle t);
	double norm (QPointF v);
	
	~Singular();
};


#endif