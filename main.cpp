#include "classifier.h"

#include <iostream>

using namespace std;

int main()
{
	classifier digitClass;
	digitClass.load_training_data();
	digitClass.load_testing_data();
	digitClass.train(1);
	cout << "done with training" << endl;

	for (int c = 0; c < 10; c++)
		cout << digitClass.prior[c] << " ";
	cout << endl;

	digitClass.test();
	cout << "done with testing" << endl;

	digitClass.evaluation();
	cout << "done with evaluation" << endl;

	for (int c = 0; c < 10; c++)
		cout << digitClass.classification_rate[c] << " ";
	cout << endl;

}
