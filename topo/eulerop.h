#ifndef EULER_OPERATOR_H
#define EULER_OPERATOR_H

#include "hed.h"

namespace HED {
	class EulerOp {
	public:
		
		static void mev(HED::Model& model, int originVtx, double x, double y);
		
		static void mvfs(HED::Model& model, double x, double y);
	};
}

#endif