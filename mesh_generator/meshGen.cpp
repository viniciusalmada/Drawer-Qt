#include "meshGen.h"
#include "meshMainTools.h"
using namespace meshMainTools
meshGen::meshGen()
{
}
void meshGen::Initialize(vector<QPointF> pol , vector<bool> vBool){

    vector <QPointF> VFrpr, total;
	vector <Triangle> triangles;


	VFrpr = createVFrpr(pol, vBool);
	total = createInside(pol,ng);

	int N = total.size();
	int nB = pol.size();
	vector<QPointF> u;
	Matrix vbx(nB, 1);
	Matrix vby = vbx;
	Matrix ux(N, 1);
	Matrix uy = ux;
	// Convert vector to matrix
	for (int i = 0; i < nB; i++)
	{
		vbx[{i, 0}] = VFrpr[i].x;
		vby[{i, 0}] = VFrpr[i].y;
	}
	
	MEF mef(triangles, total, nB);
	ux = mef.solver(vbx);
	uy = mef.solver(vby);
	for (int i = 0; i < ux.rows(); i++)
	{
		QPointF p{ux[{i, 0}], uy[{i, 0}] };
		u.push_back(p);

	}

	vector <Singular> ptsS;


	vector <Triangle*> triSing;
	ptsS = createSingPts(triangles,total,u, &triSing);

	// main loop
	for (int i = 0; i < ptsS.size(); i++)
	{
		ptsS[i].m_triSing = triSing;
		ptsS[i].Initialize();
		ptsS[i].Integrate();
	}

}

meshGen::~meshGen()
{
}
