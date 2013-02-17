#ifndef IMICHELSONPOINCAREMAP
#define IMICHELSONPOINCAREMAP


class IMichelsonPoincareMap{
 public :
	IFunction section;
	IMap vectorField;
	ITaylor solver;
	IPoincareMap pm;
	IMichelsonPoincareMap(int order , int step, interval c) : 
		section("var:x,y,z,d;fun:z;"),
		vectorField("par:c;var:x,y,z;fun:y,z,c^2-y-0.5*x*x;"),
		solver(vectorField,order,step),
		pm(solver,section)
	{
		vectorField.setParameter("c", c);
	}
	
	IVector operator () (const DVector &v){
		IVector x(3);
		x[0] = v[0];
		x[1] = v[1];
		x[2] = interval(0.0);
	
		IVector res = pm(x);
		IVector result(2);
		result[0] = res[0];
		result[1] = res[1];
		return result;
	}
	
	IVector iterate(const IVector &v,int iterations){
		IVector x(2);
		for(int i = 0 ; i < 2 ; ++i)
			x[i] = v[i];
		for(int i = 0 ; i < iterations ; ++i)
			x = (*this)(x);
		return x;
		
	}
};
#endif
