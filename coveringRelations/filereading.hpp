#ifndef FILEREADING
#define FILEREADING

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <capd/vectalg/Vector.h>

using namespace std;


template <typename Value>
void getValuesFromFile(const char * filename, vector<vector<Value> > & result, int numberOfLinesIgnored = 0, bool debug = false)
{
	ifstream file(filename);
	if(debug)
		std::cout << "READING FILE " << filename << std::endl;
	if(file.is_open()){
		string line;
		while(numberOfLinesIgnored--) 
		{
			getline(file,line);
		}
		while(file.good()) 
		{
			getline(file,line);
			stringstream ss(line);
			Value t ;
			vector<Value > temp;
			while(ss >> t){
				temp.push_back(t);
				if(debug)
					std::cout << t << " ";
			}
			if(debug)
			{
				std::cout << std::endl;
			}
			if(temp.size() > 0)
				result.push_back(temp);
		}
	}
	
	if(debug)
		std::cout << "FINISHED READING " << std::endl;
	file.close();
}


template <typename Value>
void getColumnFromFile(const char * filename, vector<Value> & result, unsigned int columnIndex = 0, int numberOfLinesIgnored = 0 , bool debug = false){
	vector<vector<Value> > temp;
	getValuesFromFile<Value>(filename,temp,numberOfLinesIgnored,debug);

	for(unsigned int i = 0 ; i < temp.size() ; ++i)
	{
		if( temp[i].size() > columnIndex)
		{
			result.push_back(temp[i][columnIndex]);
		}
	}
	
	if(debug) 
	{
		std::cout << "RESULT BEGING \n";
		for(unsigned int i = 0 ; i < result.size() ; ++i)
			std::cout << result[i] << std::endl;
		std::cout << "RESULT ENDING \n";
	}
}

void getDVectorsFromRows(vector<vector<double > > & input , vector<capd::vectalg::Vector<double,0> > & output){

	for(vector<vector<double> >::iterator it = input.begin() ; it!= input.end() ; ++it){
		capd::vectalg::Vector<double,0>  v(it->size());
		for(unsigned int i = 0 ; i < it->size() ; ++i){
			v[i] = (*it)[i];
		}
		output.push_back(v);
	}
}

#endif
