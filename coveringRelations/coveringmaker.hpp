#ifndef COVERINGREALATIONSMAKER
#define COVERINGREALATIONSMAKER

#include "triplesetscaler.hpp"
#include "iotripleset.hpp"
#include "higherorderfunction.hpp"
#include "checkers.hpp"
using namespace std;

enum coveringType  {GOOD = 0, TOOSHORTUNSTABLE = 1, TOOSHORTSTABLE =2, TOOSHORTUNSTABLESTABLE = 3};

class CoveringRelationsMaker{

	public:
	int n;
	CoveringRelationsMaker(int _n = 100): n(_n){}
	
	
	coveringType computeCoveringType(const Tset & covering ,const Tset & covered){
		vector<DVector> left, right, cleft, cright,top,bottom, ctop,cbottom;
	
	coveringType result = GOOD;
	DMichelsonPoincareMap pm(20,0.1,0.49);
	
	
	getPoints(left,from2D(covering.A),from2D(covering.D),n);
	getPoints(right,from2D(covering.B),from2D(covering.C),n);
	getPoints(top,from2D(covering.D),from2D(covering.C),n);
	getPoints(bottom,from2D(covering.A),from2D(covering.B),n);
	
	
	hof::map(left.begin(), left.end(), pm, cleft);
	hof::map(right.begin(), right.end(), pm, cright);
	hof::map(top.begin(), top.end() , pm, ctop);
	hof::map(bottom.begin(), bottom.end(), pm, cbottom);
	
	OnTheLeftChecker leftChecker(covered);
	OnTheRightChecker rightChecker(covered);
	
	
	if ((hof::allGood(cleft.begin(), cleft.end(), leftChecker) && hof::allGood(cright.begin(), cright.end() , rightChecker))
			|| (hof::allGood(cleft.begin(), cleft.end(),rightChecker) && hof::allGood(cright.begin(), cright.end() , leftChecker))){
				cout << "Looks like unstable boundry is streched correctly";
	}
	else{
		cout << "unstable boundry is not streched correctly" << endl;
		result = TOOSHORTUNSTABLE;
	}
	
	
	AcrossChecker acrossChecker(covered);
	
	if (hof::allGood(ctop.begin() , ctop.end(), acrossChecker) && hof::allGood(cbottom.begin(), cbottom.end(), acrossChecker)){
		cout << "Looks like stable boundry is nicely across";
	}
	else{
		cout << "stable boundry is not across" << endl;
		if (result == TOOSHORTUNSTABLE)
			result = TOOSHORTUNSTABLESTABLE;
		else 
			result = TOOSHORTSTABLE;
	}
		
	return result;
	}
	
	virtual bool tryCoverLoop(vector<Tset> & tsets, vector<TripleSetScaler> & scalers, int maxIt = 100){
		
		int size = tsets.size();
			for(int p = 0 ; p < size-1;++p){
				coveringType ct = computeCoveringType(tsets[p%size],tsets[(p+1)%size]);
				if (ct!=GOOD)
					return false;
			}
			
		
		return true;
	}
	
	bool coverSequence(vector<Tset> & tsets){
		int size = tsets.size();
			for(int p = 0 ; p < size-1;++p){
				coveringType ct = computeCoveringType(tsets[p%size],tsets[(p+1)%size]);
				if (ct!=GOOD)
					return false;
			}
			
		
		return true;
	}
};

class CoveringRelationsMakerShrinker : public CoveringRelationsMaker{
	public:
	bool tryCoverLoop(vector<Tset> & tsets, vector<TripleSetScaler> & scalers, int maxIt = 100){
		
		int size = tsets.size();
		for(int p = 0 ; p < maxIt; ++p){
			bool change = false;
			for(int i = 0 ; i < size -1;++i){
				cout << "Iteration " << p << " index " << i << endl;
				coveringType ct = computeCoveringType(tsets[i%size],tsets[(i+1)%size]);
				cout << ct << endl;
				while(ct==TOOSHORTUNSTABLESTABLE){
						int index = (i+1)%n;
						tsets[index] = scalers[index].shrinkUnStableDirection(tsets[index]);
						index = i%n;
						tsets[index] = scalers[index].shrinkStableDirection(tsets[index]);
						change = true;
						ct = computeCoveringType(tsets[i%size],tsets[(i+1)%size]);
						cout << endl << ct << endl;
				}
			   while(ct==TOOSHORTUNSTABLE){
						int index = (i+1)%n;
						tsets[index] = scalers[index].shrinkUnStableDirection(tsets[index]);
						change = true;
					    ct = computeCoveringType(tsets[i%size],tsets[(i+1)%size]);
				}
				while(ct==TOOSHORTSTABLE){
						int index = i%n;
						tsets[index] = scalers[index].shrinkStableDirection(tsets[index]);
						change = true;
						ct = computeCoveringType(tsets[i%size],tsets[(i+1)%size]);
				}
				
			}
			if(!change)
			return true;
			
		}
		return false;
	}
};

class CoveringRelationsMakerStrecher : public CoveringRelationsMaker{
	public:
	bool tryCoverLoop(vector<Tset> & tsets, vector<TripleSetScaler> & scalers, int maxIt = 100){
		
		int size = tsets.size();
		for(int p = 0 ; p < maxIt; ++p){
			bool change = false;
			for(int i = 0 ; i < size-1;++i){
				coveringType ct = computeCoveringType(tsets[i%size],tsets[(i+1)%size]);
				cout << ct << endl;
				cout << i << endl;
				while(ct==TOOSHORTUNSTABLESTABLE){
						int index = i%n;
						tsets[index] = scalers[index].strechUnStableDirection(tsets[index]);
						index = (i+1)%n;
						tsets[index] = scalers[index].strechStableDirection(tsets[index]);
						change = true;
					    ct = computeCoveringType(tsets[i%size],tsets[(i+1)%size]);
					    cout << endl << ct << endl;
				}
				while(ct==TOOSHORTUNSTABLE){
						int index = (i+1)%n;
						tsets[index] = scalers[index].strechUnStableDirection(tsets[index]);
						change = true;
						ct = computeCoveringType(tsets[i%size],tsets[(i+1)%size]);
						cout << endl << ct << endl;
				}
				while(ct==TOOSHORTSTABLE){
						int index = (i+1)%n;
						tsets[index] = scalers[index].strechStableDirection(tsets[index]);
						change = true;
						ct = computeCoveringType(tsets[i%size],tsets[(i+1)%size]);
						cout << endl << ct << endl;
				}
					
			}
			if(!change)
				return true;
		}
		return false;
	}
	
};


#endif
