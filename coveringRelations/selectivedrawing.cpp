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
#include "triplesetmover.hpp"

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
	int stable;
	int unstable;
	public:
		ImageDrawer(int _stable , int _unstable ,int n = 100){
			
			this->n = n;
			stable = _stable;
			unstable = _unstable;
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
			
		    PointDrawer p(stable);
		   
		    hof::foreach(ctop.begin(), ctop.end(), p);
		    hof::foreach(cbottom.begin(), cbottom.end(), p);
		    p = PointDrawer(unstable);
		    hof::foreach(cright.begin(), cright.end(), p);
		    hof::foreach(cleft.begin(), cleft.end(), p);
		    
		    DVector beg = from2D(covering.center + covering.unstable);
			DVector end = from2D(covering.center - covering.unstable);
			p = PointDrawer(ORANGE);
			vector<DVector> horiz;
			getPoints(horiz,beg,end,n);
			vector<DVector> choriz;
			hof::map(horiz.begin(), horiz.end(), pm,choriz);
			hof::foreach(choriz.begin(),choriz.end(), p);
			
		}
	
};

struct FramePointDrawer{
	
	Frame  frame;
	int color;
	void operator ()(DVector & v){
		frame.dot(v[0],v[1],color);
	}
	FramePointDrawer(Frame & _frame, int _color){
		frame = _frame;
		color = _color;
	}
};

struct FrameImageDrawer{
	
	int n ;
	int stable;
	int unstable;
	Frame  frame;
		public :
		FrameImageDrawer(Frame & _frame, int _stable , int _unstable ,int n = 100){
			
			this->n = n;
			stable = _stable;
			unstable = _unstable;
			frame = _frame;
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
			
		    FramePointDrawer p(frame,stable);
		   
		    hof::foreach(ctop.begin(), ctop.end(), p);
		    hof::foreach(cbottom.begin(), cbottom.end(), p);
		    
		    p = FramePointDrawer(frame,unstable);
		    hof::foreach(cright.begin(), cright.end(), p);
		    hof::foreach(cleft.begin(), cleft.end(), p);
		    
		    DVector beg = from2D(covering.center + covering.unstable);
			DVector end = from2D(covering.center - covering.unstable);
			p = FramePointDrawer(frame,ORANGE);
			
			vector<DVector> horiz;
			getPoints(horiz,beg,end,n);
			vector<DVector> choriz;
			hof::map(horiz.begin(), horiz.end(), pm,choriz);
			hof::foreach(choriz.begin(),choriz.end(), p);
			
		}
};

struct Shrinker{
	TripleSetScaler scaler;
	public:
	Shrinker(double strech, double shrink): scaler(strech,shrink){
		
	}
	
	void operator()(capd::covrel::TripleSet & tset){
		tset = scaler.shrinkBothDirections(tset);
	}
};

class FrameDrawer{
	Frame  frame;
	public:
	FrameDrawer(Frame & _frame){
			frame = _frame;
	}
	void operator ()(const TripleSet & tset){
		GraphicsTripleSet gset(tset.center,tset.stable,tset.unstable);
		frame << gset;
	}
};

void enlarge(tset & original, tset & image, int n = 1000){
	  openGW(900,700,WHITE,BLACK);
		rootFrame.Clear();
		Frame frame = Frame(5,135,555,585,WHITE,BLACK);
		
		vector<double> skrajne[2];
		for(int i = 0 ; i < 2 ; ++i){
			skrajne[i].push_back(original.A[i]);
			skrajne[i].push_back(original.B[i]);
			skrajne[i].push_back(original.C[i]);
			skrajne[i].push_back(original.D[i]);
		}
		
		double move = 0.1;
		double minx =  (*std::min_element(skrajne[0].begin(),skrajne[0].end())) - move;
		double miny =  (*std::min_element(skrajne[1].begin(),skrajne[1].end())) - move;
		double maxx =  (*std::max_element(skrajne[0].begin(),skrajne[0].end())) + move;
		double maxy =  (*std::max_element(skrajne[1].begin(),skrajne[1].end())) + move;
		printf("minx : %f \nminy : %f \nmaxx : %f \nmaxy : %f\n",minx,miny,maxx,maxy);
		cout << original << endl;
		frame.setWorldCoord(minx,miny,maxx,maxy);
		//frame << original;
		FrameDrawer fdrawer(frame);
		fdrawer(original);
		
		FrameImageDrawer fimagedrawer(frame,GREEN,RED,n);
		fimagedrawer(image);
   
}

class Enlarger{
	public :
	Tset original;
	Tset image;
	char  originalTag[10];
	char  imageTag[10];
	Frame frame;
	Frame txt;
	Enlarger(Tset & _original , Tset & _image , const char * _orignalTag, const char * _imageTag){
		original = _original;
		image = _image;
		
		strcpy(originalTag,_orignalTag);
		strcpy(imageTag,_imageTag);
	}
	
	void operator ()(){
		enlarge(original,image);
		CoveringRelationsMaker crm(1000);
		txt = Frame(0,0,595,130);
		txt << "Coverig : " << imageTag << "\n";
		txt << "Covered : " << originalTag << "\n";
		txt << "Result : " << crm.computeCoveringType(image,original);
		waitBt();
	}
	
	void enlarge(tset & original, tset & image, int n = 1000){
	  openGW(900,700,WHITE,BLACK);
		rootFrame.Clear();
	    frame = Frame(5,135,555,585,WHITE,BLACK);
		
		vector<double> skrajne[2];
		for(int i = 0 ; i < 2 ; ++i){
			skrajne[i].push_back(original.A[i]);
			skrajne[i].push_back(original.B[i]);
			skrajne[i].push_back(original.C[i]);
			skrajne[i].push_back(original.D[i]);
		}
		
		double move = 0.1;
		double minx =  (*std::min_element(skrajne[0].begin(),skrajne[0].end())) - move;
		double miny =  (*std::min_element(skrajne[1].begin(),skrajne[1].end())) - move;
		double maxx =  (*std::max_element(skrajne[0].begin(),skrajne[0].end())) + move;
		double maxy =  (*std::max_element(skrajne[1].begin(),skrajne[1].end())) + move;
		printf("minx : %f \nminy : %f \nmaxx : %f \nmaxy : %f\n",minx,miny,maxx,maxy);
		cout << original << endl;
		frame.setWorldCoord(minx,miny,maxx,maxy);
		//frame << original;
		FrameDrawer fdrawer(frame);
		fdrawer(original);
		
		FrameImageDrawer fimagedrawer(frame,GREEN,RED,n);
		fimagedrawer(image);
   
	}
	
	Enlarger(){}
};



capd::covrel::TripleSet bounce(const capd::covrel::TripleSet & tset){
	capd::covrel::TripleSet result(R(tset.center),R(tset.unstable),R(tset.stable));
	return result;
}

int main(int argc , char * argv[]){
	
    initGraphics();
   try
   {
		
		
		vector<TripleSet> ts;
		
		capd::covrel::TripleSet t4_3 = getTripleSet("4_3");
		capd::covrel::TripleSet t4_0 = getTripleSet("4_0");
		TripleSetMover mover(0.15);
		t4_3 = mover.moveAlongStableDirection(t4_3);

		TripleSetScaler scaler(1.2,0.8);
		//t4_0= scaler.shrinkBothDirections(t4_0);
		t4_0 = scaler.strechBothDirections(t4_0);
		
		scaler = TripleSetScaler(1.45,0.95);
		t4_3 = scaler.strechUnStableDirection(t4_3);
		scaler = TripleSetScaler(1.,0.55);
		//t4_3 = scaler.shrinkStableDirection(t4_3);
		cout << "Scaling t4_3" << endl;
		//t4_3 = scaler.shrinkBothDirections(t4_3);
		t4_3 = scaler.shrinkStableDirection(t4_3);
		
		ts.push_back(getTripleSet("2_0")); // 0
		ts.push_back(getTripleSet("3_1")); // 1
		ts.push_back(t4_3); // 2 
		ts.push_back(t4_0); // 3 
		ts.push_back(bounce(t4_3)); // 4, to jest 4_2
		Shrinker shrinker(2.0,0.45);
		hof::foreach(ts.begin(), ts.end(),shrinker);
		MyDrawer dr;
		hof::foreach(ts.begin(), ts.end(),dr);
		ImageDrawer id(GREEN,RED);
		hof::foreach(ts.begin(), ts.end(),id);
		
		CoveringRelationsMaker crm(1000);
		cout << "Covers " << crm.coverSequence(ts) << endl;
		
		vector<Enlarger> ec;
		ec.push_back(Enlarger(ts[0],ts[1],"2_0","3_1"));
		ec.push_back(Enlarger(ts[1],ts[0],"3_1","2_0"));
		ec.push_back(Enlarger(ts[2],ts[1],"4_3","3_1"));
		ec.push_back(Enlarger(ts[3],ts[2],"4_0","4_3"));
		ec.push_back(Enlarger(ts[4],ts[3],"4_2","4_0"));
		ec.push_back(Enlarger(ts[1],ts[4],"3_1","4_2"));
		for(vector<Enlarger>::iterator it = ec.begin() ; it!=ec.end() ; ++it){
			(*it)();
			
		}
		
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
