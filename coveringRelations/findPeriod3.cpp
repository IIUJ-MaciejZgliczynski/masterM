#include<iostream>
using namespace std;
#include "capd/poincare/BasicPoincareMap.hpp"
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

struct MyWriter{
	public:
		void operator()(const TripleSet & Tset){
			cout << Tset << endl;
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

struct AllGood{
	bool operator()(const DVector & v){
		return true;
	}
};

struct OnlyClose{
	
	
	
	bool operator() (const DVector &v){
		double frac = 0.5;
		return v[0] > minx*frac && v[0]*frac < maxx*frac && v[1] > miny*frac && v[1] < maxy*frac;
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
	
		void operator()(const TripleSet & covering, int iterations = 1){
			DMichelsonPoincareMap pm(20,0.1,0.49);
			vector<DVector> left, right, cleft, cright,top,bottom, ctop,cbottom;
			vector<DVector> tmpleft,tmpright,tmptop,tmpbottom;
	
			getPoints(left,from2D(covering.A),from2D(covering.D),n);
			getPoints(right,from2D(covering.B),from2D(covering.C),n);
			getPoints(top,from2D(covering.D),from2D(covering.C),n);
			getPoints(bottom,from2D(covering.A),from2D(covering.B),n);
			OnlyClose onlyClose;
			for (int i = 0 ; i < iterations ; ++i){
			
				tmpleft.clear(); tmpright.clear(); tmptop.clear(); tmpbottom.clear();
				if(i%2 == 0){
					
					cleft.clear(); cright.clear(); ctop.clear(); cbottom.clear();
					hof::map(left.begin(), left.end(), pm, tmpleft);
					hof::map(right.begin(), right.end(), pm, tmpright);
					hof::map(top.begin(), top.end() , pm, tmptop);
					hof::map(bottom.begin(), bottom.end(), pm, tmpbottom);
					
					hof::filter(tmpleft.begin(), tmpleft.end(), onlyClose, cleft);
					hof::filter(tmpright.begin(), tmpright.end(),onlyClose, cright);
					hof::filter(tmptop.begin(), tmptop.end(), onlyClose, ctop);
					hof::filter(tmpbottom.begin(), tmpbottom.end(), onlyClose, cbottom);
	
				}
				else{
					left.clear(); right.clear(); top.clear(); bottom.clear();
					hof::map(cleft.begin(), cleft.end(), pm, tmpleft);
					hof::map(cright.begin(), cright.end(), pm, tmpright);
					hof::map(ctop.begin(), ctop.end() , pm, tmptop);
					hof::map(cbottom.begin(), cbottom.end(), pm, tmpbottom);
					
					hof::filter(tmpleft.begin(), tmpleft.end(), onlyClose, left);
					hof::filter(tmpright.begin(), tmpright.end(),onlyClose, right);
					hof::filter(tmptop.begin(), tmptop.end(), onlyClose, top);
					hof::filter(tmpbottom.begin(), tmpbottom.end(), onlyClose, bottom);
				}
			}
			
		    FramePointDrawer p(frame,stable);
		    if(iterations %2 == 0){
				cleft.clear(); cright.clear(); ctop.clear(); cbottom.clear();
				AllGood ag;
				hof::filter(left.begin(), left.end(), ag, cleft);
				hof::filter(right.begin(), right.end(), ag, cright);
				hof::filter(top.begin(), top.end(),ag,ctop);
				hof::filter(bottom.begin(), bottom.end(), ag,cbottom);
			}
			
			cout << "cleft.size() = " << cleft.size() << endl;
			cout << "cright.size() = " << cright.size() << endl;
			cout << "ctop.size() = " << ctop.size() << endl;
			cout << "cbottom.size() = " << cbottom.size() << endl;
		    
		   
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

void enlarge(tset & original, tset & image,int iteration = 1, int n = 1000){
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
		fimagedrawer(image,iteration);
   
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
	
	void operator ()(int iteration = 1){
		enlarge(original,image, iteration);
		CoveringRelationsMaker crm(1000);
		txt = Frame(0,0,595,130);
		txt << "Coverig : " << imageTag << "\n";
		txt << "Covered : " << originalTag << "\n";
		txt << "Result : " << crm.computeCoveringType(image,original);
		waitBt();
	}
	
	void enlarge(tset & original, tset & image,int iteration=1, int n = 1000){
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
		fimagedrawer(image,iteration);
   
	}
	
	Enlarger(){}
};



capd::covrel::TripleSet bounce(const capd::covrel::TripleSet & tset){
	capd::covrel::TripleSet result(R(tset.center),R(tset.unstable),R(tset.stable));
	return result;
}

void saveTripleSets(vector<tset> & tsets, vector<string> & tags){
	if (tags.size() != tsets.size()){
		cout << "differnet sizes!" << endl;
		return;
	}
	else{
		int n = tsets.size();
		const char * triplesetPrefix = "covrel/trisets/good/";
		const char * triplesetSuffix = ".txt";
		for(int i = 0 ; i < n ; ++i){
			stringstream ss;
			ss << triplesetPrefix;
			ss << tags[i]  << triplesetSuffix;
			string str(ss.str());
			ofstream myfile;
			myfile.open(str.c_str ());	
			myfile.precision(20);
			myfile << tsets[i];
			myfile.close();
		}
	}
	
}

void zeroutXDoublecoordinate(capd::covrel::TripleSet & tset, double error = 1e-10){
	 capd::covrel::FloatVector x = tset.center;
	 if(abs(x[0]) < error ){
		x[0] = 0.0;
		
		tset = capd::covrel::TripleSet(x,tset.stable,R(tset.stable));
	 }
}

void addPeriod3(vector<capd::covrel::TripleSet> & tsets, vector<string> & tags){
	for(int i = 0 ; i <= 6; ++i){
		stringstream tag ;
		tag << "6_" << i;
		string rTag = tag.str();
		tsets.push_back(getTripleSet(rTag.c_str()));
		tags.push_back(rTag);
	}
}

void iteration(vector<DVector> &input, vector<DVector> &output,DMichelsonPoincareMap &pm){
		hof::map(input.begin(), input.end(), pm,output);
}

void drawIteration(vector<DVector> &toDraw){
	PointDrawer pd(BLACK);
		hof::foreach(toDraw.begin(), toDraw.end(),pd); 
}

void clearIterations(vector<DVector> v[], int n = 4){
	for(int i = 0 ; i < n ; ++i)
		v[i].clear();
}



void drawIterations(tset & tripleSet, int iterations, int n = 100){
	
	vector<DVector> v;
	vector<DVector> u;
	vector<DVector> tmp;
	
	getPoints(v,from2D(tripleSet.A),from2D(tripleSet.D),n);
	getPoints(v,from2D(tripleSet.B),from2D(tripleSet.C),n);
	getPoints(v,from2D(tripleSet.D),from2D(tripleSet.C),n);
	getPoints(v,from2D(tripleSet.A),from2D(tripleSet.B),n);
	
	DMichelsonPoincareMap pm(20,0.1,0.49);
	OnlyClose onlyClose;
	for(int i = 0 ; i < iterations; ++i){
		cout << "Begin Iteration " << (i+1) << endl;
		if(i % 2 == 0){
			u.clear();
			tmp.clear();
			iteration(v,tmp,pm);
			drawIteration(tmp);
			hof::filter(tmp.begin(),tmp.end(),onlyClose,u);
			
			
		}
		else{
			v.clear();
			tmp.clear();
			iteration(u,tmp,pm);
			drawIteration(tmp);
			hof::filter(tmp.begin(),tmp.end(),onlyClose,v);
		}
		cout << "Iteration " << (i+1) << " finished " << endl;
	}
	
}


int main(int argc , char * argv[]){
	
   initGraphics();
   try
   {
		cout.precision(20);
		
	
	
		cout << "In da TRY BLOCk" << endl;
		vector<TripleSet> ts;
		vector<string> tags;
		
	/*	ts.push_back(getGoodTripleSet("2_0")); tags.push_back("2_0");
		cout << "Added 2_0" <<endl;
	
		ts.push_back(getGoodTripleSet("3_1"));tags.push_back("3_1");
		ts.push_back(getGoodTripleSet("4_0"));tags.push_back("4_0");
		ts.push_back(getGoodTripleSet("4_3"));tags.push_back("4_3");
		ts.push_back(getGoodTripleSet("4_2"));tags.push_back("4_2");*/
		//ts.push_back(getGoodTripleSet("4_3"));tags.push_back("4_3");
		
		tset t4_3 = getTripleSet("4_3");
		Shrinker shrinker(2.0,0.45);
		cout << "added all the folders " << endl;
		shrinker(t4_3);
		//ts.push_back(t4_3); tags.push_back("4_3");
		vector<TripleSet> period3;
		vector<string> period3tags;
		
		addPeriod3(period3,period3tags);
		tset t3 = getTripleSet("6_3");
		tset t4_1 = getTripleSet("4_1");
		TripleSetMover mover(2.0);
		t3 = mover.moveOnY(t3,0.0);
		zeroutXDoublecoordinate(t3);
		zeroutXDoublecoordinate(t4_1);
		shrinker(t3);
		for(vector<TripleSet>::iterator it = period3.begin() ; it!=period3.end() ; ++it){
			zeroutXDoublecoordinate(*it);
		}
		hof::foreach(period3.begin(),period3.end(), shrinker);
		tset t6_1 = getTripleSet("6_1");
		shrinker(t6_1);
		
		
		//shrinker(t4_1);
		Shrinker mt(2.0,0.45);
		mt(t4_1);
		//ts.push_back(t4_1); tags.push_back("4_1");
		//ts.push_back(t3); tags.push_back("6_3");
		//ts.push_back(t6_1); tags.push_back("6_1");
		MyDrawer dr;
		hof::foreach(ts.begin(), ts.end(),dr);
		hof::foreach(period3.begin(),period3.end(),dr);
		ImageDrawer id(GREEN,RED);
		hof::foreach(period3.begin(), period3.end(),id);
		id(t4_1);
		MyWriter mwriter;
		hof::foreach(period3.begin(), period3.end(), mwriter);
		
		
		//dr(t3);
		//drawIterations(t4_1,1);
		/*drawIterations(t3,5);
		vector<Enlarger> ec;
		ec.push_back(Enlarger(ts[1],t3,"3_1","6_3"));
		ec[0](5);*/
		
			
		
		
		
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
