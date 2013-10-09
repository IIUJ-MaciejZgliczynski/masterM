#ifndef IOTRIPLESET
#define IOTRIPLESET

//#include <capd/covrel/GraphicsTripleSet.h>
#include "/home/maciek/Informatyka/Capd/capd4/capdDynSys4/src/capd/covrel/TripleSet.cpp"
#include <capd/covrel/TripleSet.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

//using namespace capd::covrel;

std::ostream & operator<< (std::ostream & out , const capd::covrel::TripleSet & set) {
	out << set.center << std::endl;

	out << set.stable << std::endl;
		out << set.unstable << std::endl;
	
	out << set.Icenter << std::endl;
	out << set.Istable << std::endl;
	out << set.Iunstable << std::endl;
	return out;
}

/*
 *  Narazie zakladam ze pracuje na doublach, 
 * 
 */

std::istream& operator>> (std::istream &in, capd::covrel::TripleSet & tset) {
	capd::covrel::FloatVector center, unstable, stable;
	capd::covrel::IntervalVector Icenter, Istable,Iunstable;
	in >> center;
	in >> stable;
	in >> unstable;
	in >> Icenter ;
	in >> Istable;
	in >> Iunstable;
 
	tset = capd::covrel::TripleSet(center,stable,unstable);
	return in;
}

template <typename T>
capd::covrel::TripleSet getScaled(capd::covrel::TripleSet & tset , T stableh, T unstableh){
	// zakladam ze pracuje na doublach
	return capd::covrel::TripleSet(tset.center , tset.stable * stableh, tset.unstable * unstableh);
	
} 

capd::covrel::TripleSet getTripleSet(const char * name){
	const char * triplesetPrefix = "covrel/trisets/tset";
	const char * triplesetSuffix = ".txt";
	
	std::stringstream ss;
	ss << triplesetPrefix << name << triplesetSuffix;
	std::ifstream myfile;
	myfile.open(ss.str().c_str());
	capd::covrel::TripleSet tset ;
	myfile >> tset;
	myfile.close();
	return tset;
}

capd::covrel::TripleSet getHsets(const char * name)
{
	const char * triplesetPrefix = "hsets/";
	const char * triplesetSuffix = ".txt";
	
	std::stringstream ss;
	ss << triplesetPrefix << name << triplesetSuffix;
	std::ifstream myfile;
	myfile.open(ss.str().c_str());
	capd::covrel::TripleSet tset ;
	myfile >> tset;
	myfile.close();
	return tset;
}

capd::covrel::TripleSet getGoodTripleSet(const char * name){
	const char * triplesetPrefix = "covrel/trisets/good/";
	const char * triplesetSuffix = ".txt";
	
	std::stringstream ss;
	ss << triplesetPrefix << name << triplesetSuffix;
	std::ifstream myfile;
	myfile.open(ss.str().c_str());
	capd::covrel::TripleSet tset ;
	myfile >> tset;
	myfile.close();
	return tset;
}



/*capd::covrel::GraphicsTripleSet upgradToGraphical(capd::covrel::TripleSet &tset){
	return capd::covrel::GraphicsTripleSet(tset.center, tset.stable,tset.unstable);
}*/

 
#endif
