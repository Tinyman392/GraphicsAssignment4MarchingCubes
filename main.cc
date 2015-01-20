#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "march.h"
using namespace std;

int checkArgs(int argc, 
			  char * argv[], 
			  string &file, 
			  bool &nor, 
			  float &thresh);
void paramError();
void readVector(vector<float> v2);
void readVector(vector<int> v2);
vector<float> readFile(string file, int &row, int &col, int &pln);
vector<int> posNeg(vector<float> v2, float thresh);

int main(int argc, char * argv[])
{
	//filename to read from
	string file;
	//are we doing normals?
	bool nor;
	float thresh;
	int row, col, pln;
	
	//check the arguments
	int err = checkArgs(argc, argv, file, nor, thresh);
	//if there is error, return
	if(err < 0)
		return -1;
	
	//read in the file into pts (holds values)
	vector<float> pts = readFile(file, row, col, pln);
	//init the values for the marching cubes
	initve(thresh, row, col, pln);
	//compute the binary representation for the list of values
	vector<int> bins = posNeg(pts, thresh);
	//compute the actual points to plot
	vector<coord> points = draw(bins, pts, row, col, pln);
	//normal vector place holder
	vector<coord> norms;
	//if we are computing normals compute normals
	if(nor)
	{
		//norms = getNor(points, row, col, pln);
		norms = getNor(bins, pts);
	}
	
	//draw the points and output to standard out
	drawPts(points, norms, nor);
	
	//we succeeded!
	return 0;
}

//check arguments and set file, normal boolean, and thresh hold
int checkArgs(int argc, 
			  char * argv[], 
			  string &file, 
			  bool &nor, 
			  float &thresh)
{
	//set nor to false
	nor = false;
	
	//if we have 4 args, we could have a normal flag
	if(argc == 4)
	{
		//set file
		file = "";
		file.append(argv[1]);
		//set thresh hold
		thresh = atof(argv[2]);
		//set normal flag
		string n = "";
		n.append(argv[3]);
		
		//if we have a normal flag, set it to true
		if(n == "-n")
		{
			nor = true;
		}
	}
	//if 3 args, we don't have normal flag
	else if(argc == 3)
	{
		file = "";
		file.append(argv[1]);
		string n = "";
		thresh = atof(argv[2]);
		
		nor = false;
	}
	//if not, we have error!
	else
	{
		paramError();
		return -1;
	}
	
	return 0;
}

//output usage if we have a parameter error
void paramError()
{
	cerr << "march <file> <thresh> [-n]\n";
	cerr << "\t<file> is the file to read from.\n";
	cerr << "\t<thresh> is the threshold to use.\n";
	cerr << "\t[-n] is an optional argument for calculating and using ";
	cerr << "normals\n";
	
	return;
}

//output values of a vector
void readVector(vector<float> v2)
{
	int count = 0;
	while(v2.size() > 0)
	{
		//if(count++ % 10 == 0)
		//	cout << endl;
		if(v2[0] >= 0)
			cout << v2[0] << "\t";
		v2.erase(v2.begin());
	}
	
	cout << endl;
}

void readVector(vector<int> v2)
{
	int count = 0;
	while(v2.size() > 0)
	{
		//if(count++ % 10 == 0)
		//	cerr << endl;
		if(v2[0] >= 0)
			cout << v2[0] << "\t";
		v2.erase(v2.begin());
	}
	
	cout << endl;
}

//read in a file and return the vector
//set row, col, and plane (pln)
vector<float> readFile(string file, int &row, int &col, int &pln)
{
	//vector to be returned
	vector<float> v2;
	//setup a file stream
	ifstream ifile(file.c_str(), ios::in | ios::binary);
	
	//setup buffer
	char buff[128];
	//read in row col and pln
	ifile >> row;
	ifile >> col;
	ifile >> pln;
	
	//skip exactly 1 bit of space
	ifile.seekg((int)ifile.tellg()+1);

	while(!ifile.eof())
	{
		//float to push back into vector
		float f;
		//read in float
		ifile.read((char*) &f, sizeof(float));
		v2.push_back(f);
	}
	
	//close the file
	ifile.close();
	
	//return the vector
	return v2;
}

//compare each value in v2 to the thresh hold, set appropriately
vector<int> posNeg(vector<float> v2, float thresh)
{
	//posneg vector to be returned
	vector<int> pn;
	
	//compare each value in v2 to thresh
	//if it's > thresh, push back a 1
	//if it's < thresh, push back a 0
	//if it's = thresh, push back a -1
	for(int i = 0; i < v2.size(); i++)
	{
		if(v2[i] > thresh)
			pn.push_back(1);
		else if(v2[i] < thresh)
			pn.push_back(0);
		else
		{
			pn.push_back(-1);
		}
	}
	
	//return the vector
	return pn;
}