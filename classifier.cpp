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

	for (int c = 0; c < 10; c++)
	{
		if (likelihood[c] != NULL)
			cout << "LIKELIHOOD IS NOT NULL. SHE'S GONNA BLOW (up), HIT THE DECK!" << endl;
		likelihood[c] = NULL; 
		classCount[c] = 0; 
	}

	for (int i = 0; i < numClass; i++)
	{
		classCount[trainingLabel[i]] += 1;

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
		
		int newy = datay + 28; 
		for (datax = 0; datax < 28; datax++)
		{
			int oldy = datay;
			for (; datay < newy; datay++)
			{
				if (trainingData[datay][datax] == ' ')
				{
					matrix[datay%28][datax] += 0.0; 
				}
				else 
				{
					matrix[datay%28][datax] += 1.0; 
				}
			}
			if (datax != 27) datay = oldy;
		}
	}

	/* LAPLACE SMOOTHING */
	for (int c = 0; c < 10; c++)
	{
		double ** matrix = likelihood[c];

		// prior calculation
		prior[c] = (double)classCount[c] / (double)numClass;

		for (int x = 0; x < 28; x++)
		{
			for (int y = 0; y < 28; y++)
			{
				matrix[y][x] = ((double)(matrix[y][x] + k) / (double)(classCount[c] + k * 2)); 
			}
		}
	}
}

void classifier::test()
{
	int MAP = 0; 
	double maxPosterior = 0.0; 

	int newy = 0;
	for (int i = 0; i < testClass; i++)
	{
		int testx; 
		int testy = newy; 
		int oldy = testy;
		newy += 28;

		for (int c = 0; c < 10; c++)
		{
			double result = log10(prior[c]); 

			for (testx = 0; testx < 28; testx++)
			{
					//cout << "testx " << testx << " newy " << newy << " c " << c << " i " << i << endl;
				for (; testy < newy; testy++)
				{
					if (testingData[testy][testx] != ' ')
					{
						result += log10( (likelihood[c])[testy%28][testx] ); 
						//if (testy == TY and testx == TX)
						//	cout << (likelihood[c])[testy][testx] << endl;
					}
					else
					{
						result += log10( (1 - (likelihood[c])[testy%28][testx]) ); 
					}
				}
				testy = oldy; // reset for next class
			}

			/* convert result "back" to a probability between 0 and 1 */
			//result = pow(10.0, result);
			//cout << "result " << result << " " << "maxPosterior" << " " << maxPosterior << " c " << c << endl;
			/* We say "less than" because result and maxPosterior are LOGARITHMIC */
			if (result < maxPosterior)
			{
				MAP = c;
				maxPosterior = result;  
			}
		}
		//cout << MAP << endl;
		maxPosterior = 0.0; 
		predictedLabels.push_back(MAP);
	}

	for (int henry = 0; henry < predictedLabels.size(); henry++)
		cout << predictedLabels[henry] << " ";
	cout << endl;
}

void classifier::evaluation()
{
	int classification[10]; 
	int tclassCount[10]; 

	for (int x = 0; x < 10; x++)
	{
		classification[x] = 0; 
		classification_rate[x] = 0.0; 
		tclassCount[x] = 0; 
	}

	//cout << "init to zero" << endl;

	for (int i = 0; i < testClass; i++)
	{
		//cout << testingLabel[i] << " " << predictedLabels[i] << endl;
		tclassCount[testingLabel[i]] += 1; 
		if (testingLabel[i] == predictedLabels[i])
			classification[testingLabel[i]] += 1; 
	}

	for (int x = 0; x < 10; x++)
	{
		//cout << classification[x] << " " << tclassCount[x] << endl;
		classification_rate[x] = (double)classification[x] / (double)tclassCount[x]; 
	}
}

void classifier::load_training_data()
{
	string line;
	ifstream fileData("digitdata/trainingimages.txt");
	ifstream fileLabels("digitdata/traininglabels.txt");

	numClass = 0;

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

		numClass = y / 28;

		fileData.close();
	}
	else cout << "Can't open file for data." << endl;

	if (fileLabels.is_open())
	{
		int y = 0;
		while ( getline(fileLabels, line) )
		{
			trainingLabel.push_back(atoi(line.c_str()));
		}
		fileLabels.close();
	}
	else cout << "Can't open file for labels." << endl;
}

void classifier::load_testing_data()
{
	string line;
	ifstream fileData("digitdata/testimages.txt");
	ifstream fileLabels("digitdata/testlabels.txt");

	testClass = 0;

	if (fileData.is_open())
	{
		int y = 0;
		while ( getline(fileData, line) )
		{
			testingData.push_back( new char[28] );
			// increment number of training classes
			for (int x = 0; x < 28; x++)
			{
				testingData[y][x] = line[x];
				if (testingData[y][x] != ' ')
				{
					TX = x;
					TY = y;
				}
			}
			y++;
		}

	}
	else cout << "Can't open file for data." << endl;

	if (fileLabels.is_open())
	{
		while ( getline(fileLabels, line) )
		{
			testingLabel.push_back(atoi(line.c_str()));
			// Increment number of test classes
			testClass++;
		}
		fileLabels.close();
	}
	else cout << "Can't open file for labels." << endl;
}

classifier::classifier() {};

classifier::~classifier()
{
	for (int c = 0; c < 10; c++)
	{
		if ( likelihood[c] != NULL )
		{
			// deallocate
			for (int y = 0; y < 28; y++)
				delete[] (likelihood[c])[y];
			delete[] likelihood[c];
		}
	}

	for (int i = 0; i < trainingData.size(); i++)
	{
		// deallocate
		if ( trainingData[i] != NULL )
			delete[] trainingData[i];
	}

	for (int i = 0; i < testingData.size(); i++)
	{
		// deallocate
		if ( testingData[i] != NULL )
			delete[] testingData[i];
	}
	
}
