#include "classifier.h"

#include <iostream>

using namespace std;

int main()
{
	for (int k = 0; k < 50; k++)
	{
		cout << "k = " << k << endl;
		classifier digitClass;
		digitClass.load_training_data();
		digitClass.load_testing_data();
		digitClass.train(5);
		cout << "done with training" << endl;

		double priorTot = 0;
		for (int c = 0; c < 10; c++)
		{
			cout << digitClass.prior[c] << " ";
			priorTot += digitClass.prior[c];
		}
		cout << endl;
		cout << "Total: " << priorTot << endl;

		digitClass.test();
		cout << "done with testing" << endl;

		digitClass.evaluation();
		cout << "done with evaluation" << endl;

		for (int c = 0; c < 10; c++)
			cout << digitClass.classification_rate[c] << " ";
		cout << endl;
	}


/*
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
				string sB4 = ss.str();
				if ( sB4.length() != 4 )
				{
					sB4.resize(4, ' ');
				}
				s += sB4;
				if (x != 27) s += " ";
			}
			if (y != 27) s += "\n";
		}
		f << s;
		f << endl << endl;
	}
	f.close();
	*/

}
