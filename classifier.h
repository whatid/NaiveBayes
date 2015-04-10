#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

using namespace std; 

class classifier
{

	public:

		vector<vector<int>>  trainingData ;
		vector<int> trainingLabel;  
		vector<int> classCount; 

		map < int, int ** > values; 

};
#endif