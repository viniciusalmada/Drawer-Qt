#ifndef EULER_OPERATOR_H
#define EULER_OPERATOR_H

#include "hed.h"

namespace HED {
	class EulerOp {
	public:
		
		static void mev(HED::Model& model, int originVtx, double x, double y);
		
		static int addHe(HED::Model& model, int edge, int vtx, int heWhere);
		
		static void lmev(HED::Model& model, int he1, int he2, double x, double y);
		
		static void lmef(HED::Model& model, int he1, int he2, double f);
		
		static void mvfs(HED::Model& model, double x, double y);
	};
}

#endif