#include "classifier.h"

#include <iostream>

using namespace std;

int main()
{
	classifier digitClass;
	digitClass.load_training_data();
	digitClass.load_testing_data();
	digitClass.train(5);
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


	ofstream f;
	f.open("like.txt");
	for (int c = 0; c < 10; c++)
	{
		string s = "";
		for (int y = 0; y < 28; y++)
		{
			for (int x = 0; x < 28; x++)
			{
				ostringstream ss;
				ss << digitClass.likelihood[c][y][x];
				s += ss.str();
				if (x != 27) s += " ";
			}
			if (y != 27) s += "\n";
		}
		f << s;
		f << endl << endl;
	}
	f.close();

}
