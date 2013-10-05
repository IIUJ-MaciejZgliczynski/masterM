#ifndef DMICHELSONPOINCAREMAP
#define DMICHELSONPOINCAREMAP

#include <stdexcept>
#include "capd/poincare/PoincareMap.hpp"
#include "capd/poincare/BasicPoincareMapJet.hpp"
#include "capd/capdlib.h"
#include "capd/krak/krak.h"
#include <iostream>
using namespace capd;

class DMichelsonPoincareMap{
 public :
	DFunction section;
	DMap vectorField;
	DTaylor solver;
	DPoincareMap pm;
	DMichelsonPoincareMap(int order , double step, double c) : 
		section("var:x,y,z;fun:z;"),
		vectorField("par:c;var:x,y,z;fun:y,z,c^2-y-0.5*x*x;"),
		solver(vectorField,order,step),
		pm(solver,section)
	{
		vectorField.setParameter("c", c);
	}
	
	virtual DVector operator () (const DVector &v)
	{
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
	
    DVector iterate(const DVector &v,int iterations)
	{
		DVector x(2);
		for(int i = 0 ; i < 2 ; ++i)
			x[i] = v[i];
		for(int i = 0 ; i < iterations ; ++i)
			x = operator ()(x);
		return x;
		
	}
	
	virtual DVector derivative(const DVector &v, DMatrix &der)
	{
		DMatrix dp(3,3);
		DVector x(3);
		x[0] = v[0];
		x[1] = v[1];
		x[2] = 0.0;
		
		DVector res = pm(x,dp);
		dp = pm.computeDP(res,dp);
		
		// wydobyc dwa na dwa, 
		// pamietac od wyobywaniu jedynyki
		for(int i = 0 ; i < 2 ; ++i)
			for(int j = 0 ; j < 2 ; ++j)
				der[i][j] = dp[i][j];
		DVector result(2);
		result[0] = res[0];
		result[1] = res[1];
		return result;
	}
	
};

class DMichelsonIterationPoincareMap : public DMichelsonPoincareMap {
		protected:
			int iteration;
		public :
			DMichelsonIterationPoincareMap(int order,double step,double c,int _iteration)
				: DMichelsonPoincareMap(order,step,c), iteration(_iteration)
			{
			}
		
		virtual DVector operator () (const DVector &v)
		{	
			DVector x(2);
			for(int i = 0 ; i < 2 ; ++i)
				x[i] = v[i];
			for(int i = 0 ; i < iteration ; ++i)
				x = DMichelsonPoincareMap::operator()(x);
			return x;
		}
		
	virtual DVector derivative(const DVector &v, DMatrix &der)
	{
		
		DMatrix _der(2,2);
		for(int i = 0 ; i < 2 ; ++i)
			for(int j = 0 ; j < 2 ; ++j){
				if(i == j)
					der[i][j] = 1.0;
				else
					der[i][j] = 0.0;
			}
		
		 // identycznosc
		DVector x(2);
		for(int i = 0; i<2;++i) x[i] = v[i];
		for(int i = 0 ; i < iteration ; ++i){
			x = DMichelsonPoincareMap::derivative(x,_der);
			der = _der * der;
		}
	
		return x;
	}
};

#endif
