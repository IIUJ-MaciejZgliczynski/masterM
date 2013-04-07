#include<iostream>
using namespace std;

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

using namespace capd::covrel;

class MyDrawer{
	public:
	void operator ()(const TripleSet & tset){
		GraphicsTripleSet gset(tset.center,tset.stable,tset.unstable);
		fr << gset;
	}
};

struct PointDrawer{
	
	public:
	 int color ;
	 PointDrawer (int _color):color(_color)
	 {}

	void operator () (DVector & v){
		fr.dot(v[0],v[1],color);
	}
};
class ImageDrawer{
 
	int n ;
	
	int color;
	public:
		ImageDrawer(int _color,int n = 100){
			
			this->n = n;
			color = _color;
		}
		
		void operator()(const TripleSet & covering){
			DMichelsonPoincareMap pm(20,0.1,0.49);
			vector<DVector> left, right, cleft, cright,top,bottom, ctop,cbottom;
	
			getPoints(left,from2D(covering.A),from2D(covering.D),n);
			getPoints(right,from2D(covering.B),from2D(covering.C),n);
			getPoints(top,from2D(covering.D),from2D(covering.C),n);
			getPoints(bottom,from2D(covering.A),from2D(covering.B),n);

	
			hof::map(left.begin(), left.end(), pm, cleft);
			hof::map(right.begin(), right.end(), pm, cright);
			hof::map(top.begin(), top.end() , pm, ctop);
			hof::map(bottom.begin(), bottom.end(), pm, cbottom);
			
		    PointDrawer p(color);
		    hof::foreach(cright.begin(), cright.end(), p);
		    hof::foreach(ctop.begin(), ctop.end(), p);
		    hof::foreach(cbottom.begin(), cbottom.end(), p);
		    hof::foreach(cleft.begin(), cleft.end(), p);
		}
	
};

int main(int argc , char * argv[]){
	
    initGraphics();
   try
   {
		
		
		vector<TripleSet> ts;
		//ts.push_back(getTripleSet("2_0"));
		//ts.push_back(getTripleSet("2_1"));
		
		ts.push_back(getTripleSet("3_1"));
		ts.push_back(getTripleSet("4_3"));
		ts.push_back(getTripleSet("4_0"));
		
		vector<TripleSetScaler> scalers;
		double shrink = 0.8;
		double strech = 1.2;
		scalers.push_back(SymetricTripleSetScaler(strech,shrink));
		scalers.push_back(TripleSetScaler(strech,shrink));
		scalers.push_back(SymetricTripleSetScaler(strech,shrink));
		
		CoveringRelationsMakerStrecher cms;
		cms.n = 100;
		cout << cms.tryCoverLoop(ts,scalers, 5) << endl;
		MyDrawer dr;
		hof::foreach(ts.begin(), ts.end(),dr);
		ImageDrawer id(GREEN);
		hof::foreach(ts.begin(), ts.end(),id);
		
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
