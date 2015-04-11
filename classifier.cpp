/* 
	Input Specs: 

	' ' = white pixel (background)
	'+' = gray pixel 
	'#' = black pixel (foreground)

	Each digit is of size 28x28 and are concatenated together vertically. 
*/
#include "classifier.h"


void classifier::train(int k)
{
	int datax; 
	int datay = 0; 

	for (int i = 0; i < 10; i++)
	{
		if (likelihood[i] != NULL)
			cout << "LIKELIHOOD IS NOT NULL. SHE'S GONNA BLOW (up), HIT THE DECK!" << endl;
		likelihood[i] = NULL; 
		classCount[i] = 0; 
	}

	for (int i = 0; i < numClass; i++)
	{
		classCount[i] += 1; 

		// check if classifier already has an associated matrix thing
		if (likelihood[trainingLabel[i]] == NULL)
		{
			double ** temp = new double * [28]; 
		
			/* NOTE: I switched x and y around. To me, it's easier to understand.
				A matrix in C++ can be created in any number of ways.
				In this case, temp is an array of rows.
				Each row then creates an array of 28 elements of double (gives us columns).
			*/
			for (int y = 0; y < 28; y++)
			{
				temp[y] = new double[28];
				for (int x = 0; x < 28; x++)
				{
					temp[y][x] = 0; 
				}
			}

			likelihood[trainingLabel[i]] = temp; 
		}

		double ** matrix = likelihood[trainingLabel[i]];
		
		for (datax = 0; datax < 28; datax++)
		{
			int newy = datay + 28; 
			for (; datay < newy; datay++)
			{
				if (trainingData[datay][datax] == ' ')
				{
					matrix[datay][datax] += 0.0; 
				}
				else 
				{
					matrix[datay][datax] += 1.0; 
				}
			}
		}
	}

	/* LAPLACE SMOOTHING */
	for (int c = 0; c < 10; c++)
	{
		double ** matrix = likelihood[trainingLabel[c]];

		// prior calculation
		prior[c] = (double)(classCount[c] / numClass);

		for (int x = 0; x < 28; x++)
		{
			for (int y = 0; y < 28; y++)
			{
				matrix[y][x] = ((double)(matrix[y][x] + k) / (double)(classCount[c] + k * 2)); 
			}
		}
	}
}

void classifier::testing()
{
	int MAP = 0; 
	double maxPosterior = 0.0; 
	//double posterior[10]; 

	for (int i = 0; i < testClass; i++)
	{
		for (int c = 0; c < 10; c++)
		{
			int testx; 
			int testy = 0; 

			int result = log10(prior[c]); 

			for (testx = 0; testx < 28; testx++)
			{
				int tempy = testy + 28; 
				for (; testy < tempy; testy++)
				{
					if (testingData[testx][testy] == '#' || testingData[testx][testy] == '+')
						result += log10( (likelihood[c])[testx][testy] ); 
					else
						result += log10( (1 - (likelihood[c])[testx][testy]) ); 
				}
			}
			//posterior[c] = result; 
			if (result > maxPosterior)
			{
				MAP = c;
				maxPosterior = result;  
			}
		}
		maxPosterior = 0.0; 
		predictedLabels.push_back(MAP);
	}
}

void classifier::evaluation()
{
	double classification_rate[10]; 
	int classification[10]; 
	int tclassCount[10]; 

	for (int x = 0; x < 10; x++)
	{
		classification[x] = 0; 
		classification_rate[x] = 0.0; 
		tclassCount[x] = 0; 
	}

	for (int i = 0; i < testClass; i++)
	{
		tclassCount[testLabel[i]] += 1; 
		if (testLabel[i] == predictedLabels[i])
			classification[testLabel[i]] += 1; 
	}

	for (int x = 0; x < 10; x++)
	{
		classification_rate[x] = classification[x] / tclassCount[x]; 
	}
}

void classifier::load_training_data()
{
	string line;
	ifstream fileData("digitdata/trainingimages.txt");
	ifstream fileLabels("digitdata/traininglabels.txt");

	if (fileData.is_open())
	{
		int y = 0;
		while ( getline(fileData, line) )
		{
			trainingData.push_back( new char[28] );
			for (int x = 0; x < 28; x++)
			{
				trainingData[y][x] = line[x];
			}
			y++;
		}
		fileData.close();
	}
	else cout << "Can't open file." << endl;
}

void classifier::load_test_data()
{

}

classifier::classifier() {};

classifier::~classifier()
{
	for (int c = 0; c < 10; c++)
	{
		if ( likelihood[c] != NULL )
			// deallocate
			delete[] likelihood[c];
	}
	for (int i = 0; i < trainingData.size(); i++)
	{
		// deallocate
		delete[] trainingData[i];
	}
}
