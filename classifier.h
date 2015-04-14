#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <cmath>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std; 

class classifier
{
	private:
		// finds 4 highest confusion rates and puts them into the "mc" array (below)
		void get_most_confused(double ** cnf);
	public:

		int TX;
		int TY;

		vector<char*> trainingData ;
		vector<int> trainingLabel;  

		vector<char*> testingData; 
		vector<int> testingLabel; 

		// size of trainingLabel vector 
		int numClass; 

		// size of testingLabel vector
		int testClass; 

		// class to 2D array mapping. 
		map < int, double ** > likelihood; 

		// confusion matrix
		double ** confusion;

		// stands for most confused, which contains indices of highest confusion rates
		int mc[8];

		// number of times each "number" appears in the training data.
		map <int, int> classCount; 

		//map <int, double> posterior;

		// map of class to its prior
		map <int, double> prior;  

		// the labels predicted from the test data after testing()
		vector<int> predictedLabelsMAP; 
		vector<int> predictedLabelsML; 

		
		int tclassCount[10]; 
		double classification_rateMAP[10];
		double classification_rateML[10];

		/********************************************************
			METHODS
		 ********************************************************/

		classifier();
		~classifier();

		// train method for classifer
		// calculates prior and likehoods for each class
		// k is constant for Laplace smoothing
		void train(int k);

		// test method
		// uses the prior and likehoods for each class to compute the posterior,
		// then makes a decision using MAP (predictedLabels contains decisions)
		void test();

		// evaluates the accurate of the testing based on the true labels
		void evaluation();

		void load_training_data();

		void load_testing_data();

		// make the confusion matrix
		void confusionMatrix();
		void info_for_matlab(int k);
};
#endif
