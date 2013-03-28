
#include "graphicstuff.hpp"
#include "filereading.hpp"
#include "dmichelsonpoincaremap.hpp"
#include "drawingorbits.hpp"

using namespace std;




int main(int argc , char * argv[]){
	
    initGraphics();
   try
   {
		//vector<int> vec;
		//vec.push_back(2);
		//drawCircleFromFiles(vec);
		
		const char * filename = "poss/test2_3.txt";
		const char * filename2 = "poss/test1_1.txt";
		set<double> drawn;
		drawIterationsFromFile(filename,drawn,20,0.1,0.49,4);
		//drawIterationsFromFile(filename2,drawn,20,0.1,0.49,2);
		//drawIterationsFromFiles(vec);
   }  
   catch(std::exception& e)
   {
      rootFrame << "\n" << e.what();
      waitBt();
   }

   closeGW();
   return 0;
}
