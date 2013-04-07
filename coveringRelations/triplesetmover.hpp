#ifndef TRIPLESETMOVER
#define TRIPLESETMOVER

#include <capd/covrel/TripleSet.h>

typedef capd::covrel::TripleSet tset;
class TripleSetMover{
	public :
	double h;
	
	tset moveAlongStableDirection(const tset & original){
		return tset(original.center + original.stable * h, original.stable,original.unstable);
	}
	
	tset moveAlognUnstableDirection(const tset & original){
		return tset(original.center + original.unstable*h, original.stable,original.unstable);
	}
	
	TripleSetMover(double _h = 0.1):h(_h){};
	
	
};

#endif
