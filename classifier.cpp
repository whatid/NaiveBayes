/* 
	Input Specs: 

	' ' = white pixel (background)
	'+' = gray pixel 
	'#' = black pixel (foreground)

	Each digit is of size 28x28 and are concatenated together vertically. 
*/
#include "classifier.h"


void classifier::train(vector<int> label, int ** data, int k)
{
	int datax; 
	int datay = 0; 

	for (int i = 0; i < 10; i++)
	{
		likelihood[i] = NULL; 
		classCount[i] = 0; 
	}

	for (int i = 0; i < numClass; i++)
	{

		classCount[i] += 1; 

		if (likelihood[label[i]] == NULL)
		{
			double ** temp = new double[28][28]; 
		
			for (int x = 0; x < 28; x++)
			{
				for (int y = 0; y < 28; y++)
				{
					temp[x][y] = 0; 
				}
			}

			likelihood[label[i]] = temp; 
		}

		double ** matrix = likelihood[label[i]];
		
		for (datax = 0; datax < 28; datax++)
		{
			int newy = datay + 28; 
			for (; datay < newy; datay++)
			{
				if (data[datax][datay] == ' ')
				{
					matrix[datax][datay] += 0.0; 
				}
				else 
				{
					matrix[datax][datay] += 1.0; 
				}
			}
		}
	}

	for (int c = 0; c < 10; c++)
	{
		double ** matrix = likelihood[label[c]];

		// prior calculation
		prior[c] = classCount[c] / numClass; 

		for (int x = 0; x < 28; x++)
		{
			for (int y = 0; y < 28; y++)
			{
				matrix[x][y] = ((matrix[x][y] + k) / (classCount[c] + k * 2); 
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
		if (testLabel[i] == predicatedLabels[i])
			classification[testLabel[i]] += 1; 
	}

	for (int x = 0; x < 10; x++)
	{
		classification_rate[x] = classification[x] / tclassCount[x]; 
	}
}



