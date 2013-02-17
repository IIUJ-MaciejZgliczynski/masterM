#ifndef DMICHELSONPOINCAREMAP
#define DMICHELSONPOINCAREMAP


#include "capd/poincare/PoincareMap.hpp"
class DMichelsonPoincareMap{
 public :
	DFunction section;
	DMap vectorField;
	DTaylor solver;
	DPoincareMap pm;
	DMichelsonPoincareMap(int order , int step, double c) : 
		section("var:x,y,z,d;fun:z;"),
		vectorField("par:c;var:x,y,z;fun:y,z,c^2-y-0.5*x*x;"),
		solver(vectorField,order,step),
		pm(solver,section)
	{
		vectorField.setParameter("c", c);
	}
	
	DVector operator () (const DVector &v){
		DVector x(3);
		x[0] = v[0];
		x[1] = v[1];
		x[2] = 0.0;
		
		DVector res = pm(x);
		DVector result(2);
		result[0] = res[0];
		result[1] = res[1];
		return result;
	}
	
	DVector iterate(const DVector &v,int iterations){
		DVector x(2);
		for(int i = 0 ; i < 2 ; ++i)
			x[i] = v[i];
		for(int i = 0 ; i < iterations ; ++i)
			x = (*this)(x);
		return x;
		
	}
};

#endif
