#ifndef EULER_OPERATOR_H
#define EULER_OPERATOR_H

#include "hed.h"

namespace HED {
	class EulerOp {
	public:
		
		static void mvfs(HED::Model& model, double x, double y);
		
		static void mev(HED::Model& model, int originVtx, double x, double y);
		
		static void mev0(HED::Model& model, int originVtx, double x, double y);
		
		static void mev1(HED::Model& model, int originVtx, double x, double y);
		
		static void mef(HED::Model& model, int vtx0, int vtx1);
	};
}

#endif