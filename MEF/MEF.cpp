#include <vector>
#include "MEF.h"
#include <stdlib.h>     /* abs */
#include <math/matrix.h>
#include <math/smatrix.h>

MEF::MEF(DelaunayModel& model, const vector<QPointF>& total, int nB) :
		m_model(model),
		m_triangles(model.finish()),
		m_points(total),
		m_T(m_triangles.size()),
		m_N(total.size()),
		m_nB(nB) {
	//Initialize
	vector<int> nodes;
	Matrix K(m_N, m_N);
	Matrix Pe(3, 3);
	Matrix C(3, 3);
	Matrix G(2, 3);
	Matrix Ke(3, 3);
	SMatrix SKG(m_N + m_nB, m_N + m_nB);
	double area;
	
	for (int i = 0; i < this->m_T; i++) {
		for (QPolygonF tri : m_triangles) {
			nodes = this->m_triangles.mModel.verticesOfTriangles(i).nodes;
			for (int j = 0; j < 3; j++) {
				Pe[{j, 0}] = 1;
				Pe[{j, 1}] = this->m_points[nodes[j]].x;
				Pe[{j, 2}] = this->m_points[nodes[j]].y;
			}
			area = area2D(m_points[nodes[0]], m_points[nodes[1]], m_points[nodes[2]]);
			C = (Pe).inverse();
			G = C.getSubMatrix(2, 3, 1, 3);
			Ke = (G.transpose()) * G * area;
			for (int l = 0; l < 3; l++) {
				for (int k = 0; k < 3; k++) {
					//K[{nodes[k], nodes[l]}] = K[{nodes[k], nodes[l]}] + Ke[{k, l}];
					double value = SKG.get({nodes[k], nodes[l]}) + Ke[{k, l}];
					
					SKG.set({nodes[k], nodes[l]}, value);
				}
				
			}
			
		}
		// Set Identity for Lagrange Multiplyers
		int m = 0;
		int n = m_N;
		for (int k = 0; k < m_nB; k++) {
			SKG.set({m, n}, 1);
			SKG.set({n, m}, 1);
			m++;
			n++;
		}
		m_SKG = SKG;
		
	}
	
	MEF::~MEF()
	{
	
	}
	
	double MEF::area2D(QPointF a, QPointF b, QPointF c) {
		return abs((a.x() * b.y() + b.x() * c.y() + c.x() * a.y) - (a.x() * c.y() + b.x() * a.y() + c.x() * b.y)) * 0.5;
		
	}
	
	Matrix MEF::solver(Matrix b) {
		SMatrix SF(m_N + m_nB, 1);
		SMatrix u = SF;
		Matrix uf(m_N, 1);
		
		int m = 0;
		int n = m_N;
		for (int k = 0; k < m_nB; k++) {
			
			SF.set({n, 0}, b[{m, 0}]);
			m++;
			n++;
		}
		
		u = SMatrix::solveLinearSystemCG(m_SKG, SF);
		
		for (int k = 0; k < m_N; k++) {
			uf[{k, 0}] = u.get({k, 0});
		}
		
		return uf;
	}
