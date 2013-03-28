#include "dmichelsonpoincaremap.hpp"
#include <iostream>
using namespace std;

int main(int argc , char * argv[]){
	DVector v(2);
	v[0] = 0;
	v[1] =  -0.2171079054114740392;
	int order = 20;
	int step = 0.1;
	double c = 0.49;
	DMichelsonPoincareMap pm(order,step,c);
	cout.precision(20);
	cout << "start : " << v << endl;
	for(int i = 0 ; i < 4 ; ++i){
		v = pm(v);
		cout << v << endl;
	}
}
