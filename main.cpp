#include "classifier.h"

#include <iostream>

using namespace std;

int main()
{
	classifier digitClass;

	digitClass.load_training_data();

	digitClass.train(1);
}
