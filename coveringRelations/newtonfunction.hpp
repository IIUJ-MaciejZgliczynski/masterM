#ifndef NEWTONFUNCTION
#define NEWTONFUNCTION

#include <cmath>
#include <iostream>

class NewtonFucntion{
	public :
		virtual double eval(double point, double &derivative) = 0; 
};

#define DEBUGNEWTON

double newtonMethod(NewtonFucntion & nf, double startpoint, double precission , int maxIterations){
	double x = startpoint;
	for(int i = 0 ; i < maxIterations && std::abs(x) > precission ; ++i){
		double point; double der;
		point = nf.eval(x,der);
		#ifdef DEBUGNEWTON
			std::cout << i + 1 << " "  << x <<  " " << point << " " << der << std::endl;
		#endif
		if( der == 0){
			std::cout << "We have a problem. " << std::endl;
			return point;
		}
		else{
			double nx = x - point/der;
			/*if(std::abs(nx-x) < precission){
				x = nx;
				break;
			}*/
			x = nx;
		}
	}
	return x;
}
#endif
