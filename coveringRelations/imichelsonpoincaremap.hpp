#ifndef IMICHELSONPOINCAREMAP
#define IMICHELSONPOINCAREMAP

#include "capd/capdlib.h"
#include "capd/poincare/PoincareMapJet.h"
#include "higherorderfunction.hpp"
#include <capd/covrel/TripleSet.h>
#include <capd/covrel/GridSet.h>
#include "vectorconversion.hpp"



typedef capd::poincare::PoincareMapJet<ITaylor> PoincareMap;
class IMichelsonPoincareMap{
 public :
	IFunction section;
	IMap vectorField;
	ITaylor solver;
	IPoincareMap pm;
	//PoincareMap pm;
	IMichelsonPoincareMap(int order , double step, double c) : 
		section("var:x,y,z;fun:z;"),
		vectorField("par:c;var:x,y,z;fun:y,z,c^2-y-0.5*x*x;"),
		solver(vectorField,order,step),
		pm(solver,section)
	{
		vectorField.setParameter("c", c);
	}

  
  IVector operator()(const IVector& u, int period=1)
  {
    // u is assumed to be on the section
    // so it is 2-dim, represented by coordinates (y,z).
    // We simply add 0 as the first coordinate i.e. we embed the vector
    // into the full 3d-space
    IVector x(3);
    x[0] = u[0];
    x[1] = u[1];
    x[2] = 0.0;

    // we define a doubleton representation of the set
    C0Rect2Set s(x);
    for(int i=0;i<period;++i) // and compute period-iterations
      x = pm(s);
      //x = pm(x);
    // here we project the image 'x' onto 2-dimensional section
    return IVector(2,x.begin());
  }
	IMatrix dx(const IVector& u, int period=1)
  {
    // Again u is two dimensional, so embed it.
    IVector x(3);
    x[0] = u[0];
    x[1] = u[1];
    x[2] = 0.0;

    // for computing of derivative of PM we need an instance of logarithmic norm
    IEuclLNorm N;

    // We define a doubleton representation of the set and its derivative
    // constructor sets initial condition for variational equation to Identity
    C1Rect2Set s(x,N);

    // the matrix monodromyMatrix will store derivatives of the FLOW not Poincare map
    IMatrix monodromyMatrix(3,3);
    for(int i=0;i<period;++i)
      x = pm(s,monodromyMatrix);

    // This member function recomputes derivatives of the flow into derivatives of Poincare map
    IMatrix DP = pm.computeDP(x,monodromyMatrix);

    // as before, we extract from 3x3 matrix a 2x2 slice
    IMatrix result(2,2);
   /* result(1,1) = DP(2,2);
    result(1,2) = DP(2,3);
    result(2,1) = DP(3,2);
    result(2,2) = DP(3,3);*/
    for(int i = 1 ; i <=2 ; ++i)
		for(int j = 1 ; j <= 2 ; ++j)
			result(i,j) = DP(i,j);
    
    
    return result;
  }
  
  typedef capd::covrel::TripleSet tset;
  
 /*void operator()(const tset & set, int N = 3, int M = 3){
	  GridSet grid;
	  set.gridSet(grid,N,M);
	  //hof::foreach(grid.begin(), grid.end(),pm);
  }*/
  
  IVector operator ()(C0Rect2Set  & x){
		return IVector(2, pm(x).begin());
  }
  
};
typedef capd::covrel::GridSet<IMatrix> GridSet;
class IMichelsonPoincareMapAndGridWrapper : public IMichelsonPoincareMap{
	public:
		GridSet gridset;
		IMichelsonPoincareMapAndGridWrapper(int order, double step, double c) : IMichelsonPoincareMap(order,step,c){
		}
		IMichelsonPoincareMapAndGridWrapper(int order, double step, double c, GridSet & _gridset) : IMichelsonPoincareMap(order,step,c){
			gridset = _gridset;
		}
		IVector operator()(const IVector & v){
			//return IVector(2,pm(C0Rect2Set(v,gridset.coordinateSystem(),gridset.box())).begin());
			//return IVector(2,IMichelsonPoincareMap::operator()(v + gridset.coordinateSystem()*gridset.box()).begin());
			C0Rect2Set s(v,gridset.coordinateSystem(),gridset.box());
			return IVector(2,IMichelsonPoincareMap::operator()(s).begin());
			
		}
		
		void setGridSet(GridSet _gridset){
			gridset = _gridset;
		}
		
};

#endif
