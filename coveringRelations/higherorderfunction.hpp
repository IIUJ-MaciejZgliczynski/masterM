
#ifndef HIGHERORDERFUNCTION
#define HIGHERORDERFUNCTION

#include <vector>

namespace hof{
template <class InputIterator, class Function, class vec>
void map(InputIterator begin, InputIterator end, Function & f, vec & output){
	for(;begin!=end; ++begin)
		output.push_back(f(*begin));
}

template <class InputIterator, class Function, class vec>
void filter(InputIterator begin, InputIterator end, Function & f, vec & output){
	for(;begin!=end; ++begin)
		if(f(*begin))
			output.push_back(*begin);
	
}

template <class InputIterator, class Function>
void foreach(InputIterator begin, InputIterator end, Function & f){
	for(;begin!=end; ++begin)
		f(*begin);
	
}

template <class InputIterator, class Function, class vec>
void unfilter(InputIterator begin, InputIterator end, Function & f, vec & output){
	for(;begin!=end; ++begin)
		if(!f(*begin))
			output.push_back(*begin);
	
}

template <class InputIterator, class predicat>
bool allGood(InputIterator begin , InputIterator end , predicat p){
	for(;begin!=end; ++begin)
		if(!p(*begin))
			return false;
	return true;
}

template <class InputIterator, class predicat>
InputIterator firstWrong(InputIterator begin , InputIterator  end, predicat){
	for(;begin!=end; ++begin)
		if(!p(*begin))
			break;
	return begin;
}

//template<class T, class U>
/*void map(T & op, std::vector<U> & source, std::vector<U> & output){
	for(typename std::vector<U>::iterator it = source.begin() ; it!= source.end() ; ++it) {
		output.push_back(op(*it));
	}
}*/
template<class T, class U>
void map(T & op, std::vector<U> & source, std::vector<U> & output){
	map(source.begin(), source.end() , op, output);
}

template <class T, class U>
void filter(T & op, std::vector<U> & source, std::vector<U> & output){
	filter(source.begin(), source.end(), op,output);
	/*for(std::vector<U>::iterator it = source.begin() ; it!= source.end() ; ++it) {
		if(op(*it))
			output.push_back(*it);
	}*/
	
	
}



}
#endif
