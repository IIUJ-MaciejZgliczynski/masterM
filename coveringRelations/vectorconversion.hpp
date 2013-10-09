#ifndef VECTOROCONVERSION
#define VECTOROCONVERSION

#include "capd/vectalg/Vector.hpp"

using namespace capd::vectalg;
template <class S> 
Vector<S,2> to2D (const Vector<S,0> & y)
{
	return Vector<S,2>(2,y.begin());
}

template <class S>
Vector<S,0> from2D (const Vector<S,2> & y){
	return Vector<S,0>(2,y.begin());
} 

template <class S>
Vector<S,2> R(const Vector<S,2> &y){
	Vector<S,2> v;
	v[0] = -y[0];
	v[1] = y[1];
	return v;
}

#endif
