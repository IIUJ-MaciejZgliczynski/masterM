#ifndef DMICHELSONNEWTONFUNCTION
#define DMICHELSONNEWTONFUNCTION

#include "newtonfunction.hpp"

class DMichelsonSymetricOrbitFinder : public NewtonFucntion {
	protected :
	 DMichelsonPoincareMap pm;
	public :
	 DMichelsonSymetricOrbitFinder(int order, double step , double c) : pm(order,step,c){ 
	 }
	 
	 virtual double eval(double point, double & der){
		 DVector v(2);
		 v[0] = 0; v[1] = point;
		 DMatrix derivative(2,2);
		 v = pm.derivative(v,derivative);
		 
		 der = derivative[0][1];
		 return v[0];
	 }
};

class DMichelsonIterationSymetricOrbitFinder : public NewtonFucntion {
	
	protected :
	  DMichelsonIterationPoincareMap pm;
	public :
		DMichelsonIterationSymetricOrbitFinder(int order, double step, double c , int _iteration) : 
			pm(order,step,c,_iteration)
		{
			
		}
	
		double eval(double point, double & der)
		{
			
		DVector v(2);
		 v[0] = 0; v[1] = point;
		 DMatrix derivative(2,2);
		 DMatrix _der(2,2);
		 
		 v = pm.derivative(v,derivative);
		 
		 der = derivative[0][1];
		 return v[0];
		}
};

#endif
