#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <cmath>

using namespace std; 

class classifier
{

	public:

		int ** trainingData ;
		vector<int> trainingLabel;  

		int ** testingData; 

		vector<int> testLabel; 
		// size of trainingLabel vector 
		int numClass; 

		// size of testingLabel vector
		int testClass; 

		// class to 2D array mapping. 
		map < int, double ** > likelihood; 

		// number of times each "number" appears in the training data.
		map <int, int> classCount; 

		//map <int, double> posterior;

		map <int, double> prior;  

		// train method for classifer
		void train(vector<int> label, int ** data);

		void testing();

		void evaluation();

		vector<int> predictedLabels; 



};
#endif