#include "vectorconversion.hpp"
#include <capd/covrel/GraphicsTripleSet.h>
#include <capd/covrel/TripleSet.h>
#include <cmath>
#include "higherorderfunction.hpp"
#include "iotripleset.hpp"
#include "checkers.hpp"
#include "imichelsonpoincaremap.hpp"
//#include "triplesetcreator.hpp"
/*#include "checkers.hpp"
#include "coveringmaker.hpp"*/

using namespace capd;
using namespace std;

typedef capd::covrel::TripleSet tset;
typedef capd::covrel::GridSet<IMatrix> GridSet;

#define NOPOSITIVEMESSAGE

bool Icovering(tset & covering , tset & covered, int N = 99){
	
	//IMichelsonPoincareMap pm(20,0.1,0.49);
	IMichelsonPoincareMapAndGridWrapper pm(20,0.1,0.49);
	
	/*
	 * Tworzenie gridu zbioru i brzegow
	 */
	GridSet leftEdge,rightEdge,topEdge,bottomEdge;
	GridSet gridSet;
	covering.gridSet(gridSet,10,10);
	covering.gridLeftEdge(leftEdge,N); // niestabilny
	covering.gridRightEdge(rightEdge,N); // niestabilny
	covering.gridBottomEdge(bottomEdge,N); // stabilny
	covering.gridTopEdge(topEdge,N); // stabilny
	//cout << "Created grids" << endl;
	/*
	 * Pierwszy etap sprawdzania nakrywania, czy odzworowanie jest w ogole okreslone
	 * Jesli nie rzuci wyjatek to sie wszystko to odzworawanie jest okreslone i moge kontynuowac
	 */
	pm.setGridSet(gridSet);
	hof::foreach(gridSet.begin(), gridSet.end(),pm);
	#ifndef NOPOSITIVEMESSAGE
	cout << "Didn't blow up so is defined" << endl;
	#endif
	
	/*
	 * Obliczam obrazy brzegow
	 */
	vector<IVector> leftEdgeImage, rightEdgeImage,bottomEdgeImage,topEdgeImage;
	pm.setGridSet(leftEdge);
	hof::map(leftEdge.begin(), leftEdge.end(), pm,leftEdgeImage);
	pm.setGridSet(rightEdge);
	hof::map(rightEdge.begin(), rightEdge.end(), pm, rightEdgeImage);
	pm.setGridSet(bottomEdge);
	hof::map(bottomEdge.begin(), bottomEdge.end(), pm, bottomEdgeImage);
	pm.setGridSet(topEdge);
	hof::map(topEdge.begin(), topEdge.end(), pm, topEdgeImage);
	
	/*
	 * Sprawdzam czy brzeg ktory mial sie splaszczyc sie rzeczywscie splaszczyl
	 */
	AcrossChecker acrossChecker(covered);
	if (hof::forall(bottomEdgeImage.begin(), bottomEdgeImage.end(), acrossChecker)){
			#ifndef NOPOSITIVEMESSAGE
			cout << "Bottom edge is good " << endl;
			#endif
	}
	else{
		cout << "Bottome edge is not good " << endl;
		return false;
	}
	
	if (hof::forall(topEdgeImage.begin(), topEdgeImage.end(), acrossChecker)){
		#ifndef NOPOSITIVEMESSAGE
		    cout << "Top edge is good " << endl;
		    #endif
	}
	else{
		cout << "Top edge is not good" << endl;
		return false;
	}
	OnTheLeftChecker onTheLeftChecker(covered);
	OnTheRightChecker onTheRightChecker(covered);
	
	/*
	 *  Tu sprawdzam efekt rosciagania, odwzorowanie moze miec dwie orientacje wiec albo 
	 *  obraz lewego po lewej i obraz prawego po prawej lub na odwrot
	 */
	if ((hof::forall(leftEdgeImage.begin(), leftEdgeImage.end(),onTheLeftChecker) &&
		hof::forall(rightEdgeImage.begin(), rightEdgeImage.end(), onTheRightChecker)) ||
		(hof::forall(leftEdgeImage.begin(), leftEdgeImage.end(),onTheRightChecker) &&
		hof::forall(rightEdgeImage.begin(), rightEdgeImage.end(), onTheLeftChecker))){
			#ifndef NOPOSITIVEMESSAGE
			cout << "Unstable edges also good";
			#endif
		}
	else{
		cout << "Unstable edges not well" << endl;
		return false;
	}
		
		
	/*
	 *  Doszlo do tego momentu to wszystkie warunki pokrywania sa spelnione wiec mozemy zwrocic tak
	 */
	return true;
}

const char * toString(bool cond){
	return cond ? "true" : "false";
}
int main(){
	tset ts2_0 = getTripleSet("N1");
	tset ts3_1 = getTripleSet("N2");
	tset ts4_3 = getTripleSet("N3");
	tset ts4_0 = getTripleSet("N4");
	tset ts4_2 = getTripleSet("N5");
	cout.precision(20);
	
	cout << "first loop " << endl;
	cout << "\n2_0 covers 3_1 " << toString(Icovering(ts2_0,ts3_1)) << endl;
	cout << "\n3_1 covers 2_0 " << toString(Icovering(ts3_1,ts2_0)) << endl;
	
	cout << "second loop " << endl;
	cout << "\n3_1 covers 4_3 " << toString(Icovering(ts3_1,ts4_3)) << endl;
	cout << "\n4_3 covers 4_0 " << toString(Icovering(ts4_3,ts4_0)) << endl;
	cout << "\n4_0 covers 4_2 " << toString(Icovering(ts4_0,ts4_2)) << endl;
	cout << "\n4_2 covers 3_1 " << toString(Icovering(ts4_2,ts3_1)) << endl;
	
	

}
