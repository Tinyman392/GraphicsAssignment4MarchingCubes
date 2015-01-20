#ifndef MARCH_H
#define MARCH_H

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <sstream>
using namespace std;

//Coordinate structure
struct coord
{
	float p[3];	
};

//This is used for mapping
//We eventually will take the average of the fn vectors
struct fNorm
{
	vector<coord> fn;
	coord nor;
};

//init our edge and adjacent vertex tables
void initve(float t, int row, int col, int pln);
//compute the location on the table based on the input c
//Then push back the last of values
vector<int> add_cube(vector<int> c);
//computes vertices for drawing
vector<coord> draw(vector<int> bins, vector<float> vals, int r, int c, int p);
//get's the physical vertex based on current location
void getPts(vector<coord> &pts, vector<float> vals, vector<int> eVals, int x, int y, int z, vector<int> bins);
//draws a set of computed vertices
void drawPts(vector<coord> pts, vector<coord> norms, bool nor);
//interpolates between two coordinates
coord interpolate(coord a, coord b);
//interpolates between two numbers
float interpolate(float a, float b);
//get a list of normals by averaging the normals of the shared faces for a 
//given vertex
vector<coord> getNor(vector<coord> points, int row, int col, int pln);
//
vector<coord> getNor(vector<int> bins, vector<float> vals);
//
void getNor(vector<coord> &nors, vector<coord> cubeNor, vector<float> vals, vector<int> eVals, int x, int y, int z);
//convert a float to a string
string to_str(float f);
//cross two vectors
coord cross(coord a, coord b);
//convert coordinate to a string of 3 floats
string to_str(coord c);
//check if a vector is the zero vector
bool isZero(coord c);
//average all the coordinates in the vector fn
coord average(vector<coord> fn);
//subtract a from b
coord subtract(coord a, coord b); //a - b
//add a to b
coord add(coord a, coord b);
#endif