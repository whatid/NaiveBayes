#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <cmath>
#include <string>
#include <vector>

using namespace std; 

class classifier
{

	public:

		vector<char*> trainingData ;
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

		// map of class to its prior
		map <int, double> prior;  

		// the labels predicted from the test data after testing()
		vector<int> predictedLabels; 


		/********************************************************
			METHODS
		 ********************************************************/

		classifier();
		~classifier();

		// train method for classifer
		// calculates prior and likehoods for each class
		// k is constant for Laplace smoothing
		void train(int k);

		// testing method
		// uses the prior and likehoods for each class to compute the posterior,
		// then makes a decision using MAP (predictedLabels contains decisions)
		void testing();

		// evaluates the accurate of the testing based on the true labels
		void evaluation();

		void load_training_data();

		void load_test_data();
};
#endif
