#ifndef TRIPLESETMOVER
#define TRIPLESETMOVER

#include <capd/covrel/TripleSet.h>

typedef capd::covrel::TripleSet tset;
typedef capd::covrel::FloatVector floatVector;
class TripleSetMover{
	public :
	double h;
	
	tset moveAlongStableDirection(const tset & original){
		return tset(original.center + original.stable * h, original.stable,original.unstable);
	}
	
	tset moveAlognUnstableDirection(const tset & original){
		return tset(original.center + original.unstable*h, original.stable,original.unstable);
	}
	
	tset moveOnX(const tset & original , double dx){
		floatVector ncenter = original.center;
		ncenter[0] += dx;
		
		return tset(ncenter,original.stable,original.unstable);
	}
	
	tset moveOnY(const tset & original , double dy){
		floatVector ncenter = original.center;
		ncenter[1] += dy;
		
		return tset(ncenter,original.stable,original.unstable);
	}
	
	TripleSetMover(double _h = 0.1):h(_h){};
	
	
};

#endif
