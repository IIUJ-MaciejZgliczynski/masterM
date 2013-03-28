#ifndef TRIPLESETSCALER
#define TRIPLESETSCALER


#inlcude "iotripleset.hpp"

class TripleSetScaler{
	typedef capd::covrel::TripleSet Tset;
	protected :
    virtual Tset changeStableDirection(const Tset & original, double d){
		return Tset(original.center,original.stable * d, original.unstable);
	}
	
    virtual Tset changeUnStableDirection(const Tset & original , double d){
		return Tset(original.center,original.stable , original.unstable);
	}
	
    Tset changeBothDirections(const Tset & original , double d){
		return Tset(original.center, original.stable * d , original.unstable * d);
	}
	
	
	double strech; // strech > 1
	double shrink; // shrink < 1 
	
	public :
		
	TripleSetScaler(double _h) : strech(_h), shrink(1/_h) {}
	TripleSetScaler(double _strech, _shrink) : strech(_strech), shrink(_shrink){}
	
    Tset strechStableDirection(const Tset & original){
			return changeStableDirection(original,strech);
	}
	
	 Tset strechUnStableDirection(const Tset & original){
			return changeUnStableDirection(original,strech);
	}
	
	 Tset shrinkStableDirection(const Tset & original){
			return changeStableDirection(original, shrink);
	}	
	
    Tset shrinkUnStableDirection(const Tset & original){
			return changeUnStableDirection(original,shrink);
	}
	
	 Tset strechBothDirections(const Tset & original){
			return changeBothDirections(original, strech);
	}
	
    Tset shrinkBothDirections(const Tset & original){
			return changeBothDirections(const Tset & orignal,shrink);
	}
	
};

class SymetricTripleSetScaler : public TripleSetScaler{
	protected:
	virtual Tset changeStableDirection(const Tset & original, double d){
		//return Tset(original.center,original.stable * d, original.unstable);
		return changeBothDirections(original,d);
	}
	
    virtual Tset changeUnStableDirection(const Tset & original , double d){
		//return Tset(original.center,original.stable , original.unstable);
		return changeBothDirections(original,d);
	}
};

#endif
