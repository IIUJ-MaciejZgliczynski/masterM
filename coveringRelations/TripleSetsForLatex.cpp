#include "vectorconversion.hpp"
#include "graphicstuff.hpp"
#include "filereading.hpp"
#include "imichelsonpoincaremap.hpp"
#include "drawingorbits.hpp"


#include <capd/covrel/GraphicsTripleSet.h>
#include <capd/covrel/TripleSet.h>
#include <cmath>
#include "iotripleset.hpp"
#include "higherorderfunction.hpp"
#include "triplesetcreator.hpp"
#include "checkers.hpp"
#include "coveringmaker.hpp"

using namespace std;

typedef capd::covrel::TripleSet tset;

const string item = "\\item";
const string enumerate = "enumerate";
const int precision = 20;
class Formatter
{
	public :
	
	stringstream in;
	
	Formatter()
	{
		in.precision(precision);
		in << "\\begin{" + enumerate + "}\n";
	}
	
    void Format(int index, tset _tset)
	{	 
		 in << item << " ";
		 in << "$ x_" << index << " = " ;
		 FormatVector(_tset.center) ;
		 in << " $ ";
		 in << "$ u_" << index << " = " ;
		 FormatVector(_tset.unstable) ;
		 in << " $ ";
		 in << "$ s_" << index << " = " ;
		 FormatVector(_tset.stable) ;
		 in << " $ " << endl;
	}
	
	string GetFormatted()
	{
		in << "\\end{" + enumerate + "}\n";
		return in.str();
	}
	
	void FormatVector(const FloatVector & floatVector)
	{
		 in << "(" << floatVector[0] << "," << floatVector[1] << ")";	 
	}
};

int main(int argc, char ** argv)
{
	cout.precision(precision);
	Formatter formater;
	
	 formater.Format(1,getGoodTripleSet("2_0"));
	 formater.Format(2,getGoodTripleSet("3_1"));
	 formater.Format(3,getGoodTripleSet("4_0"));
	 formater.Format(4,getGoodTripleSet("4_2"));
	 formater.Format(5,getGoodTripleSet("4_3"));
	 
	 cout << formater.GetFormatted();
}
