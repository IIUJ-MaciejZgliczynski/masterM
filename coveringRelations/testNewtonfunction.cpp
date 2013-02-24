#include "newtonfunction.hpp"
using namespace std;

class RootNewtonFunction : public NewtonFucntion{
	
	private :
		double a ;
	public :
	
	RootNewtonFunction(double _a) : a(_a){}
	
	double eval(double point , double & derivative){
		   derivative = 2*point;
		   return point*point - a;
	}
};




int main(int argc, char * argv[]){
	
	double a = 4;
	RootNewtonFunction rnf(a);
	cout << newtonMethod(rnf,1,1e-10,20) << endl;
}
