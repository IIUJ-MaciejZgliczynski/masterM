#ifndef checkers
#define checkers

#include "iotripleset.hpp"

void getPoints(vector<DVector> & output,const  DVector & p,const DVector & q, int n){
	double step = 1./n;
	DVector d = q - p;
	double s = 0;
	for (int i = 0; i <= n ; ++i, s+=step){
		output.push_back(p + s*d);
	}
}


struct OutsideChecker{
	public :
		capd::covrel::TripleSet ts;
		OutsideChecker(const capd::covrel::TripleSet & _ts): ts(_ts){}
		bool operator ()(const DVector & v){
			return ts.outside(to2D(v));
		}
};

struct AcrossChecker{
	public :
		capd::covrel::TripleSet ts;
		AcrossChecker(const capd::covrel::TripleSet & _ts): ts(_ts){}
		bool operator() (const DVector & v){
			return ts.across(to2D(v));
		}
};

struct OnTheLeftChecker{
	public :
		capd::covrel::TripleSet ts;
		OnTheLeftChecker(const capd::covrel::TripleSet & _ts): ts(_ts){}
		bool operator()(const DVector & v){
			return ts.onLeft(to2D(v));
		}
};

struct OnTheRightChecker{
	public :
		capd::covrel::TripleSet ts;
		OnTheRightChecker(const capd::covrel::TripleSet & _ts): ts(_ts){}
		bool operator()(const DVector & v){
			return ts.onRight(to2D(v));
		}
};


#endif
