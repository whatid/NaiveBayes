/* 
	Input Specs: 

	' ' = white pixel (background)
	'+' = gray pixel 
	'#' = black pixel (foreground)

	Each digit is of size 28x28 and are concatenated together vertically. 
*/

int main(int argc, char *argv[])
{
	ifstream file (argv[1]);

	if (!file) {
		cout << "Input arguments: labels, images" << endl; 
		return 0; 
	}

	vector<int> traininglabels;  
	int input; 
	while (file >> input)
		traininglabels.push_back(input); 

	

}


int train()
{


	
}