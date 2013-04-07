
#include "vectorconversion.hpp"
#include "graphicstuff.hpp"
#include "filereading.hpp"
#include "dmichelsonpoincaremap.hpp"
#include "drawingorbits.hpp"


#include <capd/covrel/GraphicsTripleSet.h>
//#include "/home/maciek/Informatyka/Capd/capd/capdDynSys/src/capd/covrel/TripleSet.cpp"
#include <capd/covrel/TripleSet.h>
#include <cmath>
#include "iotripleset.hpp"
#include "higherorderfunction.hpp"
#include "triplesetcreator.hpp"
#include "checkers.hpp"
#include "coveringmaker.hpp"

using namespace std;






template <typename T>
struct Writer{
	void operator() (T t){	
		cout << t << endl;
	}
};

struct Drawer{
	
	public:
	 int color ;
	 Drawer (int _color):color(_color)
	 {}

	void operator () (DVector & v){
		fr.dot(v[0],v[1],color);
	}
};


bool covering(const capd::covrel::TripleSet & covering, const capd::covrel::TripleSet & covered, int n) {
	
	vector<DVector> left, right, cleft, cright,top,bottom, ctop,cbottom;
	
	Writer<DVector> w;
	DMichelsonPoincareMap pm(20,0.1,0.49);
	getPoints(left,from2D(covering.A),from2D(covering.D),n);
	getPoints(right,from2D(covering.B),from2D(covering.C),n);
	getPoints(top,from2D(covering.D),from2D(covering.C),n);
	getPoints(bottom,from2D(covering.A),from2D(covering.B),n);
	
	
	//Drawer d(BLACK);
	//hof::foreach(left.begin(), left.end(), d);
	//hof::foreach(right.begin(), right.end(),d);
	//hof::foreach(top.begin(), top.end() , d);
	//hof::foreach(bottom.begin(), bottom.end(), d);
	
	hof::map(left.begin(), left.end(), pm, cleft);
	hof::map(right.begin(), right.end(), pm, cright);
	hof::map(top.begin(), top.end() , pm, ctop);
	hof::map(bottom.begin(), bottom.end(), pm, cbottom);
	
	Drawer drawer(RED);
	hof::foreach(cleft.begin(), cleft.end(), drawer);
	drawer.color = BLUE;
	hof::foreach(cright.begin(), cright.end() , drawer);
	
	OutsideChecker outsideChecker(covered);
	vector<DVector> wrongleft;
	vector<DVector> wrongright;
	hof::unfilter(cleft.begin(), cleft.end(), outsideChecker,wrongleft);
	hof::unfilter(cright.begin(), cright.end(), outsideChecker,wrongright);
	if (wrongleft.size() == 0){
		cout << "Hurra wrongleft empty" << endl;
	}
	else
		hof::foreach(wrongleft.begin(),wrongleft.end(),w);
		
	if (wrongright.size() == 0){
		cout << "Hurra wrongRight empty" << endl;
	}
	else
		hof::foreach(wrongright.begin(),wrongright.end(), w);
	
	
	
	Drawer drawer2(GREEN);
	hof::foreach(ctop.begin(), ctop.end(), drawer2);
	drawer2.color = ORANGE;
	hof::foreach(cbottom.begin(), cbottom.end(), drawer2);
	
	AcrossChecker acrossChecker(covered);
	vector<DVector> wrongtop;
	vector<DVector> wrongbottom;
	hof::unfilter(ctop.begin(), ctop.end(), acrossChecker,wrongtop);
	hof::unfilter(cbottom.begin(), cbottom.end(), acrossChecker,wrongbottom);
	
	if (wrongtop.size() == 0){
		cout << "Hurra wrongtop empty" << endl;
	}
	else
		hof::foreach(wrongtop.begin(),wrongtop.end(),w);
		
	if (wrongbottom.size() == 0){
		cout << "Hurra wrongbottom empty" << endl;
	}
	else
		hof::foreach(wrongbottom.begin(),wrongbottom.end(), w);
	
	
	return true;
}

capd::covrel::TripleSet createTripleSet(DVector & v, DMichelsonIterationPoincareMap & map){
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
	double h = 1.0;
	for(int i = 0 ; i < 2 ; ++i){
		stable[i] = rVec[i][stableIndex]*h;
		unstable[i] = rVec[i][unstableIndex]*h;
	}
	return capd::covrel::TripleSet(to2D(x), stable,unstable);
	
}

void createTripleSets(vector<DVector> & v, vector<capd::covrel::TripleSet> & output ,int iteration, double c = 0.49){
	DMichelsonIterationPoincareMap map(20,0.1,c,iteration);
	for(vector<DVector>::iterator it = v.begin() ; it!=v.end() ; ++it){
		cout << *it << endl;
		output.push_back(createTripleSet(*it, map));
	}
}

void zeroutXDoublecoordinate(capd::covrel::TripleSet & tset){
	 capd::covrel::FloatVector x = tset.center;
	 x[0] = 0.0;
	 tset = capd::covrel::TripleSet(x,tset.stable,tset.unstable);
}
// Ugly stuff
// Nie wiem jak zmieniac koncowki
void makeSidesEqualLength(capd::covrel::TripleSet & tset){
	for(int i = 0 ; i < 2 ; ++i)
		if(tset.stable[i] == tset.unstable[i]){
			capd::covrel::FloatVector x = tset.stable;
			x[(i+1)%2] = -x[(i+1)%2];
			tset = capd::covrel::TripleSet(tset.center,tset.stable,x);
			return;
		}
}


void prooveCoveringRelationsOrbit2(){
	const char * points2orbit = "covrel/orbit2.txt";
	vector<vector<double> > orbit2;
	getValuesFromFile<double>(points2orbit,orbit2);
	
	cout << "Read file " << endl;
	vector<DVector> v2;
	getDVectorsFromRows(orbit2,v2);
	cout << "Gotten DVectors" << endl;
	vector<capd::covrel::TripleSet> t2;
	createTripleSets(v2,t2,2);
	cout << "Created triplests " << endl;
	
	for(vector<capd::covrel::TripleSet>::iterator it = t2.begin() ; it!=t2.end() ; ++it)
		zeroutXDoublecoordinate(*it);
	
	for(vector<capd::covrel::TripleSet>::iterator it = t2.begin() ; it!= t2.end() ; ++it){
		makeSidesEqualLength(*it);
	}
	double h1 = 0.2,h2 = 0.2;
	cout << "Zero out the first coordinate" << endl;
	for(vector<capd::covrel::TripleSet>::iterator it = t2.begin() ; it!=t2.end() ; ++it){
		*it = getScaled(*it,h1,h2);
	}
	
	
	
	cout << "Scaled them " << endl;
	for(vector<capd::covrel::TripleSet>::iterator it = t2.begin() ; it!=t2.end() ; ++it)
		cout << *it << endl;
	
	covering(t2[0],t2[1],100);
	covering(t2[1],t2[0],100);
	
	CoveringRelationsMaker crm;
	crm.n = 100;
	cout << crm.computeCoveringType(t2[0],t2[1]) << endl;
	cout << crm.computeCoveringType(t2[1],t2[0]) << endl;
	
}


void poincareMain(){
	    const char * points2orbit = "covrel/orbit2.txt";
		const char * points4orbit = "covrel/orbit4.txt";
		const char * points6orbit = "covrel/orbit6.txt";
		
		vector<vector<double> > orbit2;
		vector<vector<double> > orbit4;
		vector<vector<double> > orbit6;
		getValuesFromFile<double>(points2orbit,orbit2);
		getValuesFromFile<double>(points4orbit,orbit4);
		getValuesFromFile<double>(points6orbit,orbit6);
	
		vector<DVector> v2;
		vector<DVector> v4;
		vector<DVector> v6;
		
		getDVectorsFromRows(orbit2,v2);
		getDVectorsFromRows(orbit4,v4);		
		getDVectorsFromRows(orbit6,v6);
			
		for(vector<DVector>::iterator it = v2.begin() ; it!= v2.end() ; ++it){
			drawStableUnStableDirections(*it,2);
		}
		
		cout << " ORBITY 4 " << endl;
		for(vector<DVector>::iterator it = v4.begin() ; it!=v4.end() ; ++it){
			drawStableUnStableDirections(*it,4);
		}
		
		/*for(vector<DVector>::iterator it = v6.begin() ; it!=v6.end() ; ++it){
			drawStableUnStableDirections(*it,6);
		}*/
		
		//draw_orbit((*v2.begin())[1],true,20,0.1,0.49,2);
		//draw_orbit((*v4.begin())[1],true,10,0.1,0.49,4);
}


int main(int argc , char * argv[]){
	
    initGraphics();
   try
   {
		poincareMain();
		//waitBt();
		prooveCoveringRelationsOrbit2();
		waitBt();
   }  
   catch(std::exception& e)
   {
      rootFrame << "\n" << e.what();
      waitBt();
   }

   closeGW();
   return 0;
}
