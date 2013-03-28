
#include "vectorconversion.hpp"
#include "graphicstuff.hpp"
#include "filereading.hpp"
#include "dmichelsonpoincaremap.hpp"
#include "drawingorbits.hpp"
#include <cmath>
#include <capd/covrel/GraphicsTripleSet.h>
#include "iotripleset.hpp"

//#include "/home/maciek/Informatyka/Capd/capd/capdDynSys/src/capd/covrel/TripleSet.cpp"
using namespace std;


//using namespace capd::covrel;

int main(int argc , char * argv[]){
	
    initGraphics();
   try
   {
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
		
		for(vector<DVector>::iterator it = v6.begin() ; it!=v6.end() ; ++it){
			drawStableUnStableDirections(*it,6);
		}
		
		//draw_orbit((*v2.begin())[1],true,20,0.1,0.49,2);
		//draw_orbit((*v4.begin())[1],true,10,0.1,0.49,4);

		
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
