#ifndef IOTRIPLESET
#define IOTRIPLESET

//#include "/home/maciek/Informatyka/Capd/capd/capdDynSys/src/capd/covrel/TripleSet.cpp"
//#include "/home/maciek/Informatyka/Capd/capd4/capdDynSys4/src/capd/covrel/TripleSet.cpp"
#include <capd/covrel/TripleSet.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace capd;
using namespace capd::covrel;

std::ostream & operator<< (std::ostream & out , const TripleSet & set) {
	out << set.center << std::endl;
	out << set.stable << std::endl;
	out << set.unstable << std::endl;
	
	out << set.Icenter << std::endl;
	out << set.Istable << std::endl;
	out << set.Iunstable << std::endl;
	return out;
}

std::istream& operator>> (std::istream &in, TripleSet & tset) 
{
	TripleSet::FloatVector center, unstable, stable;
	TripleSet::IntervalVector Icenter, Istable,Iunstable;
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
TripleSet getScaled(TripleSet & tset , T stableh, T unstableh)
{
	// zakladam ze pracuje na doublach
	return TripleSet(tset.center , tset.stable * stableh, tset.unstable * unstableh);
} 

TripleSet getTripleSet(const char * name)
{
	const char * triplesetPrefix = "HSets/";
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
#endif
