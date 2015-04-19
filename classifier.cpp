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

		/**********************************
			FEATURE 1     
		***********************************/

		// prior calculation
		prior[c] = (double)classCount[c] / (double)numClass;

		for (int x = 0; x < 28; x++)
		{
			for (int y = 0; y < 28; y++)
			{
				/**********************************
					FEATURE 2    
				***********************************/
				matrix[y][x] = ((double)(matrix[y][x] + k) / (double)(classCount[c] + k * 2)); 
			}
		}
		cout << matrix[14][14] << endl;
	}
}

void classifier::test()
{
	int MAP = 0; 
	int ML = 0;
	double maxPosterior = -1.0/0.0; 
	double maxLikelihood = -1.0/0.0;

	for (int c = 0; c < 10; c++)
	{
		testexample[c] = 0.0; 
	}

	int newy = 0;
	for (int i = 0; i < testClass; i++)
	{
		int testx; 
		int testy = newy; 
		int oldy = testy;
		newy += 28;

		for (int c = 0; c < 10; c++)
		{
			double result = 0;

			for (testx = 0; testx < 28; testx++)
			{
					//cout << "testx " << testx << " newy " << newy << " c " << c << " i " << i << endl;
				for (; testy < newy; testy++)
				{
					if (testingData[testy][testx] != ' ')
					{
						result += log10( (likelihood[c])[testy%28][testx] ); 
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
			//cout << "result " << result << " " << "maxPosterior" << " " << maxPosterior << " c " << c;
			/* We say "less than" because result and maxPosterior are LOGARITHMIC */

		/**********************************
			FEATURE 3     
		***********************************/	

			if (result > maxLikelihood)
			{
				ML = c;
				maxLikelihood = result;  
			}

			result += log10(prior[c]);
			if (result > maxPosterior)
			{
				MAP = c;
				maxPosterior = result;  
			}

		}
		//cout << " MAP " << MAP << endl;

		if (maxPosterior < testexample[MAP])
		{
			testexample[MAP] = maxPosterior;
			idx_example[MAP] = i;  
		}

		maxPosterior = -1.0/0.0; 
		maxLikelihood = -1.0/0.0; 
		predictedLabelsML.push_back(ML);
		predictedLabelsMAP.push_back(MAP);
	}

	for (int i = 0; i < 10; i++)
	{
		cout << "best example for class" << i << ": " << idx_example[i] << endl; 
	}

	//for (int henry = 0; henry < predictedLabels.size(); henry++)
	//	cout << predictedLabels[henry] << " ";
	//cout << endl;
}

void classifier::evaluation()
{
	/* This is apparently needed to prevent a seg fault caused from a for loop that is totally void of memory operations */
	confusion = NULL;

	int classificationMAP[10]; 
	int classificationML[10]; 

	for (int x = 0; x < 10; x++)
	{
		classificationMAP[x] = 0; 
		classificationML[x] = 0; 
		classification_rateMAP[x] = 0.0; 
		classification_rateML[x] = 0.0; 
		tclassCount[x] = 0; 
	}

	//cout << "init to zero" << endl;

	correct = 0;
	for (int i = 0; i < testClass; i++)
	{
		//cout << testingLabel[i] << " " << predictedLabels[i] << endl;
		tclassCount[testingLabel[i]] += 1; 
		if (testingLabel[i] == predictedLabelsMAP[i])
		{
			correct++;
			classificationMAP[testingLabel[i]] += 1; 
		}
		if (testingLabel[i] == predictedLabelsML[i])
			classificationML[testingLabel[i]] += 1; 
	}

	for (int x = 0; x < 10; x++)
	{
		//cout << "classification " << classification[x] << "	tclassCount " << tclassCount[x] << endl;
		classification_rateMAP[x] = (double)classificationMAP[x] / (double)tclassCount[x]; 
		classification_rateML[x] = (double)classificationML[x] / (double)tclassCount[x]; 
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

void classifier::confusionMatrix()
{
	if (confusion != NULL)
	{
		cout << "You must be confused. You already called this function." << endl;
		return;
	}

	double ** confusion = new double * [10]; 
	for (int x = 0; x < 10; x++)
	{
		confusion[x] = new double [10]; 
		for (int y = 0; y < 10; y++)
		{
			confusion[x][y] = 0; 
		}
	}

	for (int it = 0; it < testClass; it++)
	{
		confusion[predictedLabelsMAP[it]][testingLabel[it]] += (1.0 / tclassCount[testingLabel[it]]); 
	}

	//cout.setf(ios::fixed, ios::floatfield);
	//cout.precision(2); 
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			// I made it prettier :) -Brandon
			ostringstream ss;
			ss.setf(ios::fixed, ios::floatfield);
			ss.precision(2);
			ss << (confusion[y][x] * 100);
			string s = ss.str();
			s += "%";
			s.resize(6, ' ');
			//cout << (confusion[y][x] * 100) << "%" << " "; 
			cout << s << " ";
		}
		cout << "\n";
	}

	//cout << confusion << endl;
	//get_most_confused(confusion);
}

/* THIS IS A PRIVATE FUNCTION */
void classifier::get_most_confused(double ** cnf)
{
	/* the code below stores the indices corresponding to the highest confusion rates
		say we calculate indices of highest rates, where (r1, c1) corresponds to indices of most confused
		and (r2, c2) corresponds to second most confused, and so on. The array would look like this:
		[ r1, c1, r2, c2, r3, c3, r4, c4 ]
		*/

	for (int i = 0; i < 8; i++)
		mc[i] = 0; //init

	double max1, max2, max3, max4; // the highest values encountered
	max1 = max2 = max3 = max4 = 0.0;

	int col_start = 2; // start at 2nd column for first row
	for (int y = 0; y < 10; y++)
	{
		for (int x = col_start; x < 10; x++)
		{
			if (x == y) continue; // not considering same class, obviously

			// gina is our candidate. Say hi to her!
			double gina = cnf[y][x];
			if (gina > max1)
			{
				max1 = gina;
				mc[0] = y;
				mc[1] = x;
			}
			else if (gina > max2)
			{
				max2 = gina;
				mc[2] = y;
				mc[3] = x;
			}
			else if (gina > max3)
			{
				max3 = gina;
				mc[4] = y;
				mc[5] = x;
			}
			else if (gina > max4)
			{
				max4 = gina;
				mc[6] = y;
				mc[7] = x;
			}
		}
		col_start = 0; // from now on, we'll iterate through all columns
	}
}

void classifier::info_for_matlab(int k)
{
	ostringstream ss;
	ss << k;
	string filename = "matlab/matlab_info.m";
	
	ofstream f;
	f.open(filename.c_str());

			// if class c is in most confused is 
	vector <int> mc2;
	mc2.push_back(4);
	mc2.push_back(8);
	mc2.push_back(9);
	mc2.push_back(4);
	mc2.push_back(9);
	mc2.push_back(7);
	mc2.push_back(9);
	mc2.push_back(8);
	for (int c = 0; c < 8; c++)
	{
		f << "likelihood" << mc2[c] << "=[";
		for (int y = 0; y < 28; y++)
		{
			for (int x = 0; x < 28; x++)
			{
				f << likelihood[mc2[c]][y][x];
				if (x != 27)
					f << " ";
			}
			if (y != 27)
				f << ";\n";
			else
				f << "];\n\n";
		}
	}

	f << "indices = ["; 
	for (int m = 0; m < 7; m+=2)
	{
		f << "[" << mc[m] << "," << mc[m+1] << "] ";
	}
	f << "];";
	
	f.close();
}

classifier::classifier() {};

classifier::~classifier()
{
	// basically frees all allocated memory

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

	if (confusion != NULL)
	{
		for (int c1 = 0; c1 < 10; c1++)
		{
			if (confusion[c1] != NULL)
			{
				delete[] confusion[c1];
			}
		}
	}
}
