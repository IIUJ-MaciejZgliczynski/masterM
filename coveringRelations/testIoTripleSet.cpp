#include "iotripleset.hpp"
#include "vectorconversion.hpp"
#include "graphicstuff.hpp"
#include "filereading.hpp"
#include "dmichelsonpoincaremap.hpp"
#include "drawingorbits.hpp"



#include <cmath>



void testWrite(const DVector & v, int iteration, double c = 0.49){
	
	DMichelsonIterationPoincareMap map(20,0.1,c,iteration);
	DMatrix der(2,2);
	DVector x = map.derivative(v,der);
	cout.precision(20);
	DVector rV(2), iV(2);         // real and imaginary parts of eigenvalues
	DMatrix rVec(2,2), iVec(2,2); // real and imaginary parts of eigenvectors
	capd::alglib::computeEigenvaluesAndEigenvectors(der,rV,iV,rVec,iVec);
	
	capd::covrel::FloatVector stable,unstable;
	int unstableIndex = 0;
	if ( abs(rV[1]) > 1)
		unstableIndex = 1;
	int stableIndex = 1 - unstableIndex;
	double h = 0.1;
	for(int i = 0 ; i < 2 ; ++i){
		stable[i] = rVec[i][stableIndex]*h;
		unstable[i] = rVec[i][unstableIndex]*h;
	}
	
	x[0] = 0;
	capd::covrel::TripleSet set(to2D(x), unstable,stable);
	
	
	cout << set << endl;

	
	//cout << nset;
}

void testRead(){
	capd::covrel::TripleSet s;
	
	cin >> s;
	cout << s;
}

int main(int argc, char * argv[]){
	cout.precision(20);
		/*const char * points2orbit = "covrel/orbit2.txt";
		const char * points4orbit = "covrel/orbit4.txt";
		
		vector<vector<double> > orbit2;
		vector<vector<double> > orbit4;
		getValuesFromFile<double>(points2orbit,orbit2);
		getValuesFromFile<double>(points4orbit,orbit4);
	
		vector<DVector> v2;
		vector<DVector> v4;
		
		getDVectorsFromRows(orbit2,v2);
		getDVectorsFromRows(orbit4,v4);		
		
		testWrite(*(v2.begin()),2);*/
		testRead();
		
}
