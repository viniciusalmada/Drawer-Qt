#include <math/matrix.h>
#include "meshGen.h"
#include "meshMainTools.h"

using namespace meshMainTools;
using std::vector;

void meshGen::Initialize(vector<std::pair<QPointF, bool>> boundaryPts,
                         DelaunayModel meshTriangles,
                         QVector<QPointF> totalPoints) {
	
	vector<QPointF> VFrpr;
	
	VFrpr = createVFrpr(boundaryPts);
	
	int N = totalPoints.size();
	int nB = boundaryPts.size();
	vector<QPointF> u;
	Matrix vbx(nB, 1);
	Matrix vby = vbx;
	Matrix ux(N, 1);
	Matrix uy = ux;
	// Convert vector to matrix
	for (int i = 0; i < nB; i++) {
		vbx[{i, 0}] = VFrpr[i].x();
		vby[{i, 0}] = VFrpr[i].y();
	}
	
	MEF mef(triangles, total, nB);
	ux = mef.solver(vbx);
	uy = mef.solver(vby);
	for (int i = 0; i < ux.rows(); i++) {
		QPointF p{ux[{i, 0}], uy[{i, 0}]};
		u.push_back(p);
		
	}
	
	vector<Singular> ptsS;
	
	
	vector<Triangle*> triSing;
	ptsS = createSingPts(triangles, total, u, &triSing);
	
	// main loop
	for (int i = 0; i < ptsS.size(); i++) {
		ptsS[i].m_triSing = triSing;
		ptsS[i].Initialize();
		ptsS[i].Integrate();
	}
	
}
