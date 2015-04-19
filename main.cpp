#include "classifier.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define K_MAX 2

using namespace std;

int to_file = 1;

int main(int argc, char ** argv)
{
	int kmin = atoi(argv[1]);
	int kmax = atoi(argv[2]);

	int kBest = 0;
	double maxAccuracy = 0;
	for (int k = 1; k < K_MAX; k++)
	{
		cout << "k = " << k << endl;
		classifier digitClass;
		digitClass.load_training_data();
		digitClass.load_testing_data();
		digitClass.train(k);
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

		double minMAP = 1;
		double minML = 1;
		for (int c = 0; c < 10; c++)
		{
			double cr = digitClass.classification_rateMAP[c];
			if (minMAP > cr)
				minMAP = cr;
			cr = digitClass.classification_rateML[c];
			if (minML > cr)
				minML = cr;

			//cout << digitClass.classification_rate[c] << " ";
		}
		cout << "Minimum classification rate (MAP): " << minMAP << endl;
		cout << "Minimum classification rate (ML): " << minML << endl;

		digitClass.confusionMatrix();

		digitClass.info_for_matlab(k);

		double acc = (double)digitClass.correct/(double)digitClass.testClass;
		if (maxAccuracy < acc)
		{
			maxAccuracy = acc;
			kBest = k;
		}

		if (to_file == 1)
		{
			ofstream f;
			f.open("predict.txt");
			for (int i = 0; i < digitClass.predictedLabelsMAP.size(); i++)
			{
				ostringstream ss;
				ss << digitClass.predictedLabelsMAP[i];
				f << ss.str() << endl;
			}
			f.close();
		}
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
					if ( sB4.length() != 7 )
					{
						sB4.resize(7, ' ');
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
	}

	cout << "max accuracy " << maxAccuracy << " k = " << kBest << endl;
}
