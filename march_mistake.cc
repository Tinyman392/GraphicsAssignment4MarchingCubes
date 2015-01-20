#include "march.h"

//Edge table lookup
vector<coord> e;
//Edge adjacency table lookup
vector<coord> ea;
//Vertex table lookup
vector<coord> v;
//Thresh hold
float thresh;
//Rows, Columns, and Planes
int row, col, pln;

/*
	Lookup table!  Note!  This Lookup table was taken from:
		http://paulbourke.net/geometry/polygonise/table2.txt
	
	Yeah, I'm not crazy enough to make my own 256 x 12 table...
	So I'm using someone else's
*/
static int aCases[256][13]=
{
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 8, 3, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 9, 0, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 8, 3, 1, 8, 1, 9,-1,-1,-1,-1,-1,-1,-1},
    {10, 1, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 8, 3, 0, 1, 2,10,-1,-1,-1,-1,-1,-1,-1},
    { 9, 0, 2, 9, 2,10,-1,-1,-1,-1,-1,-1,-1},
    { 3, 2, 8, 2,10, 8, 8,10, 9,-1,-1,-1,-1},
    {11, 2, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {11, 2, 0,11, 0, 8,-1,-1,-1,-1,-1,-1,-1},
    {11, 2, 3, 0, 1, 9,-1,-1,-1,-1,-1,-1,-1},
    { 2, 1,11, 1, 9,11,11, 9, 8,-1,-1,-1,-1},
    {10, 1, 3,10, 3,11,-1,-1,-1,-1,-1,-1,-1},
    { 1, 0,10, 0, 8,10,10, 8,11,-1,-1,-1,-1},
    { 0, 3, 9, 3,11, 9, 9,11,10,-1,-1,-1,-1},
    { 8,10, 9, 8,11,10,-1,-1,-1,-1,-1,-1,-1},
    { 8, 4, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 3, 0, 4, 3, 4, 7,-1,-1,-1,-1,-1,-1,-1},
    { 1, 9, 0, 8, 4, 7,-1,-1,-1,-1,-1,-1,-1},
    { 9, 4, 1, 4, 7, 1, 1, 7, 3,-1,-1,-1,-1},
    {10, 1, 2, 8, 4, 7,-1,-1,-1,-1,-1,-1,-1},
    { 2,10, 1, 0, 4, 7, 0, 7, 3,-1,-1,-1,-1},
    { 4, 7, 8, 0, 2,10, 0,10, 9,-1,-1,-1,-1},
    { 2, 7, 3, 2, 9, 7, 7, 9, 4, 2,10, 9,-1},
    { 2, 3,11, 7, 8, 4,-1,-1,-1,-1,-1,-1,-1},
    { 7,11, 4,11, 2, 4, 4, 2, 0,-1,-1,-1,-1},
    { 3,11, 2, 4, 7, 8, 9, 0, 1,-1,-1,-1,-1},
    { 2, 7,11, 2, 1, 7, 1, 4, 7, 1, 9, 4,-1},
    { 8, 4, 7,11,10, 1,11, 1, 3,-1,-1,-1,-1},
    {11, 4, 7, 1, 4,11, 1,11,10, 1, 0, 4,-1},
    { 3, 8, 0, 7,11, 4,11, 9, 4,11,10, 9,-1},
    { 7,11, 4, 4,11, 9,11,10, 9,-1,-1,-1,-1},
    { 9, 5, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 3, 0, 8, 4, 9, 5,-1,-1,-1,-1,-1,-1,-1},
    { 5, 4, 0, 5, 0, 1,-1,-1,-1,-1,-1,-1,-1},
    { 4, 8, 5, 8, 3, 5, 5, 3, 1,-1,-1,-1,-1},
    { 2,10, 1, 9, 5, 4,-1,-1,-1,-1,-1,-1,-1},
    { 0, 8, 3, 5, 4, 9,10, 1, 2,-1,-1,-1,-1},
    {10, 5, 2, 5, 4, 2, 2, 4, 0,-1,-1,-1,-1},
    { 3, 4, 8, 3, 2, 4, 2, 5, 4, 2,10, 5,-1},
    {11, 2, 3, 9, 5, 4,-1,-1,-1,-1,-1,-1,-1},
    { 9, 5, 4, 8,11, 2, 8, 2, 0,-1,-1,-1,-1},
    { 3,11, 2, 1, 5, 4, 1, 4, 0,-1,-1,-1,-1},
    { 8, 5, 4, 2, 5, 8, 2, 8,11, 2, 1, 5,-1},
    { 5, 4, 9, 1, 3,11, 1,11,10,-1,-1,-1,-1},
    { 0, 9, 1, 4, 8, 5, 8,10, 5, 8,11,10,-1},
    { 3, 4, 0, 3,10, 4, 4,10, 5, 3,11,10,-1},
    { 4, 8, 5, 5, 8,10, 8,11,10,-1,-1,-1,-1},
    { 9, 5, 7, 9, 7, 8,-1,-1,-1,-1,-1,-1,-1},
    { 0, 9, 3, 9, 5, 3, 3, 5, 7,-1,-1,-1,-1},
    { 8, 0, 7, 0, 1, 7, 7, 1, 5,-1,-1,-1,-1},
    { 1, 7, 3, 1, 5, 7,-1,-1,-1,-1,-1,-1,-1},
    { 1, 2,10, 5, 7, 8, 5, 8, 9,-1,-1,-1,-1},
    { 9, 1, 0,10, 5, 2, 5, 3, 2, 5, 7, 3,-1},
    { 5, 2,10, 8, 2, 5, 8, 5, 7, 8, 0, 2,-1},
    {10, 5, 2, 2, 5, 3, 5, 7, 3,-1,-1,-1,-1},
    {11, 2, 3, 8, 9, 5, 8, 5, 7,-1,-1,-1,-1},
    { 9, 2, 0, 9, 7, 2, 2, 7,11, 9, 5, 7,-1},
    { 0, 3, 8, 2, 1,11, 1, 7,11, 1, 5, 7,-1},
    { 2, 1,11,11, 1, 7, 1, 5, 7,-1,-1,-1,-1},
    { 3, 9, 1, 3, 8, 9, 7,11,10, 7,10, 5,-1},
    { 9, 1, 0,10, 7,11,10, 5, 7,-1,-1,-1,-1},
    { 3, 8, 0, 7,10, 5, 7,11,10,-1,-1,-1,-1},
    {11, 5, 7,11,10, 5,-1,-1,-1,-1,-1,-1,-1},
    {10, 6, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 8, 3, 0,10, 6, 5,-1,-1,-1,-1,-1,-1,-1},
    { 0, 1, 9, 5,10, 6,-1,-1,-1,-1,-1,-1,-1},
    {10, 6, 5, 9, 8, 3, 9, 3, 1,-1,-1,-1,-1},
    { 1, 2, 6, 1, 6, 5,-1,-1,-1,-1,-1,-1,-1},
    { 0, 8, 3, 2, 6, 5, 2, 5, 1,-1,-1,-1,-1},
    { 5, 9, 6, 9, 0, 6, 6, 0, 2,-1,-1,-1,-1},
    { 9, 6, 5, 3, 6, 9, 3, 9, 8, 3, 2, 6,-1},
    { 3,11, 2,10, 6, 5,-1,-1,-1,-1,-1,-1,-1},
    { 6, 5,10, 2, 0, 8, 2, 8,11,-1,-1,-1,-1},
    { 1, 9, 0, 6, 5,10,11, 2, 3,-1,-1,-1,-1},
    { 1,10, 2, 5, 9, 6, 9,11, 6, 9, 8,11,-1},
    {11, 6, 3, 6, 5, 3, 3, 5, 1,-1,-1,-1,-1},
    { 0, 5, 1, 0,11, 5, 5,11, 6, 0, 8,11,-1},
    { 0, 5, 9, 0, 3, 5, 3, 6, 5, 3,11, 6,-1},
    { 5, 9, 6, 6, 9,11, 9, 8,11,-1,-1,-1,-1},
    {10, 6, 5, 4, 7, 8,-1,-1,-1,-1,-1,-1,-1},
    { 5,10, 6, 7, 3, 0, 7, 0, 4,-1,-1,-1,-1},
    { 5,10, 6, 0, 1, 9, 8, 4, 7,-1,-1,-1,-1},
    { 4, 5, 9, 6, 7,10, 7, 1,10, 7, 3, 1,-1},
    { 7, 8, 4, 5, 1, 2, 5, 2, 6,-1,-1,-1,-1},
    { 4, 1, 0, 4, 5, 1, 6, 7, 3, 6, 3, 2,-1},
    { 9, 4, 5, 8, 0, 7, 0, 6, 7, 0, 2, 6,-1},
    { 4, 5, 9, 6, 3, 2, 6, 7, 3,-1,-1,-1,-1},
    { 7, 8, 4, 2, 3,11,10, 6, 5,-1,-1,-1,-1},
    {11, 6, 7,10, 2, 5, 2, 4, 5, 2, 0, 4,-1},
    {11, 6, 7, 8, 0, 3, 1,10, 2, 9, 4, 5,-1},
    { 6, 7,11, 1,10, 2, 9, 4, 5,-1,-1,-1,-1},
    { 6, 7,11, 4, 5, 8, 5, 3, 8, 5, 1, 3,-1},
    { 6, 7,11, 4, 1, 0, 4, 5, 1,-1,-1,-1,-1},
    { 4, 5, 9, 3, 8, 0,11, 6, 7,-1,-1,-1,-1},
    { 9, 4, 5, 7,11, 6,-1,-1,-1,-1,-1,-1,-1},
    {10, 6, 4,10, 4, 9,-1,-1,-1,-1,-1,-1,-1},
    { 8, 3, 0, 9,10, 6, 9, 6, 4,-1,-1,-1,-1},
    { 1,10, 0,10, 6, 0, 0, 6, 4,-1,-1,-1,-1},
    { 8, 6, 4, 8, 1, 6, 6, 1,10, 8, 3, 1,-1},
    { 9, 1, 4, 1, 2, 4, 4, 2, 6,-1,-1,-1,-1},
    { 1, 0, 9, 3, 2, 8, 2, 4, 8, 2, 6, 4,-1},
    { 2, 4, 0, 2, 6, 4,-1,-1,-1,-1,-1,-1,-1},
    { 3, 2, 8, 8, 2, 4, 2, 6, 4,-1,-1,-1,-1},
    { 2, 3,11, 6, 4, 9, 6, 9,10,-1,-1,-1,-1},
    { 0,10, 2, 0, 9,10, 4, 8,11, 4,11, 6,-1},
    {10, 2, 1,11, 6, 3, 6, 0, 3, 6, 4, 0,-1},
    {10, 2, 1,11, 4, 8,11, 6, 4,-1,-1,-1,-1},
    { 1, 4, 9,11, 4, 1,11, 1, 3,11, 6, 4,-1},
    { 0, 9, 1, 4,11, 6, 4, 8,11,-1,-1,-1,-1},
    {11, 6, 3, 3, 6, 0, 6, 4, 0,-1,-1,-1,-1},
    { 8, 6, 4, 8,11, 6,-1,-1,-1,-1,-1,-1,-1},
    { 6, 7,10, 7, 8,10,10, 8, 9,-1,-1,-1,-1},
    { 9, 3, 0, 6, 3, 9, 6, 9,10, 6, 7, 3,-1},
    { 6, 1,10, 6, 7, 1, 7, 0, 1, 7, 8, 0,-1},
    { 6, 7,10,10, 7, 1, 7, 3, 1,-1,-1,-1,-1},
    { 7, 2, 6, 7, 9, 2, 2, 9, 1, 7, 8, 9,-1},
    { 1, 0, 9, 3, 6, 7, 3, 2, 6,-1,-1,-1,-1},
    { 8, 0, 7, 7, 0, 6, 0, 2, 6,-1,-1,-1,-1},
    { 2, 7, 3, 2, 6, 7,-1,-1,-1,-1,-1,-1,-1},
    { 7,11, 6, 3, 8, 2, 8,10, 2, 8, 9,10,-1},
    {11, 6, 7,10, 0, 9,10, 2, 0,-1,-1,-1,-1},
    { 2, 1,10, 7,11, 6, 8, 0, 3,-1,-1,-1,-1},
    { 1,10, 2, 6, 7,11,-1,-1,-1,-1,-1,-1,-1},
    { 7,11, 6, 3, 9, 1, 3, 8, 9,-1,-1,-1,-1},
    { 9, 1, 0,11, 6, 7,-1,-1,-1,-1,-1,-1,-1},
    { 0, 3, 8,11, 6, 7,-1,-1,-1,-1,-1,-1,-1},
    {11, 6, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {11, 7, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 0, 8, 3,11, 7, 6,-1,-1,-1,-1,-1,-1,-1},
    { 9, 0, 1,11, 7, 6,-1,-1,-1,-1,-1,-1,-1},
    { 7, 6,11, 3, 1, 9, 3, 9, 8,-1,-1,-1,-1},
    { 1, 2,10, 6,11, 7,-1,-1,-1,-1,-1,-1,-1},
    { 2,10, 1, 7, 6,11, 8, 3, 0,-1,-1,-1,-1},
    {11, 7, 6,10, 9, 0,10, 0, 2,-1,-1,-1,-1},
    { 7, 6,11, 3, 2, 8, 8, 2,10, 8,10, 9,-1},
    { 2, 3, 7, 2, 7, 6,-1,-1,-1,-1,-1,-1,-1},
    { 8, 7, 0, 7, 6, 0, 0, 6, 2,-1,-1,-1,-1},
    { 1, 9, 0, 3, 7, 6, 3, 6, 2,-1,-1,-1,-1},
    { 7, 6, 2, 7, 2, 9, 2, 1, 9, 7, 9, 8,-1},
    { 6,10, 7,10, 1, 7, 7, 1, 3,-1,-1,-1,-1},
    { 6,10, 1, 6, 1, 7, 7, 1, 0, 7, 0, 8,-1},
    { 9, 0, 3, 6, 9, 3, 6,10, 9, 6, 3, 7,-1},
    { 6,10, 7, 7,10, 8,10, 9, 8,-1,-1,-1,-1},
    { 8, 4, 6, 8, 6,11,-1,-1,-1,-1,-1,-1,-1},
    {11, 3, 6, 3, 0, 6, 6, 0, 4,-1,-1,-1,-1},
    { 0, 1, 9, 4, 6,11, 4,11, 8,-1,-1,-1,-1},
    { 1, 9, 4,11, 1, 4,11, 3, 1,11, 4, 6,-1},
    {10, 1, 2,11, 8, 4,11, 4, 6,-1,-1,-1,-1},
    {10, 1, 2,11, 3, 6, 6, 3, 0, 6, 0, 4,-1},
    { 0, 2,10, 0,10, 9, 4,11, 8, 4, 6,11,-1},
    { 2,11, 3, 6, 9, 4, 6,10, 9,-1,-1,-1,-1},
    { 3, 8, 2, 8, 4, 2, 2, 4, 6,-1,-1,-1,-1},
    { 2, 0, 4, 2, 4, 6,-1,-1,-1,-1,-1,-1,-1},
    { 1, 9, 0, 3, 8, 2, 2, 8, 4, 2, 4, 6,-1},
    { 9, 4, 1, 1, 4, 2, 4, 6, 2,-1,-1,-1,-1},
    { 8, 4, 6, 8, 6, 1, 6,10, 1, 8, 1, 3,-1},
    { 1, 0,10,10, 0, 6, 0, 4, 6,-1,-1,-1,-1},
    { 8, 0, 3, 9, 6,10, 9, 4, 6,-1,-1,-1,-1},
    {10, 4, 6,10, 9, 4,-1,-1,-1,-1,-1,-1,-1},
    { 9, 5, 4, 7, 6,11,-1,-1,-1,-1,-1,-1,-1},
    { 4, 9, 5, 3, 0, 8,11, 7, 6,-1,-1,-1,-1},
    { 6,11, 7, 4, 0, 1, 4, 1, 5,-1,-1,-1,-1},
    { 6,11, 7, 4, 8, 5, 5, 8, 3, 5, 3, 1,-1},
    { 6,11, 7, 1, 2,10, 9, 5, 4,-1,-1,-1,-1},
    {11, 7, 6, 8, 3, 0, 1, 2,10, 9, 5, 4,-1},
    {11, 7, 6,10, 5, 2, 2, 5, 4, 2, 4, 0,-1},
    { 7, 4, 8, 2,11, 3,10, 5, 6,-1,-1,-1,-1},
    { 4, 9, 5, 6, 2, 3, 6, 3, 7,-1,-1,-1,-1},
    { 9, 5, 4, 8, 7, 0, 0, 7, 6, 0, 6, 2,-1},
    { 4, 0, 1, 4, 1, 5, 6, 3, 7, 6, 2, 3,-1},
    { 7, 4, 8, 5, 2, 1, 5, 6, 2,-1,-1,-1,-1},
    { 4, 9, 5, 6,10, 7, 7,10, 1, 7, 1, 3,-1},
    { 5, 6,10, 0, 9, 1, 8, 7, 4,-1,-1,-1,-1},
    { 5, 6,10, 7, 0, 3, 7, 4, 0,-1,-1,-1,-1},
    {10, 5, 6, 4, 8, 7,-1,-1,-1,-1,-1,-1,-1},
    { 5, 6, 9, 6,11, 9, 9,11, 8,-1,-1,-1,-1},
    { 0, 9, 5, 0, 5, 3, 3, 5, 6, 3, 6,11,-1},
    { 0, 1, 5, 0, 5,11, 5, 6,11, 0,11, 8,-1},
    {11, 3, 6, 6, 3, 5, 3, 1, 5,-1,-1,-1,-1},
    { 1, 2,10, 5, 6, 9, 9, 6,11, 9,11, 8,-1},
    { 1, 0, 9, 6,10, 5,11, 3, 2,-1,-1,-1,-1},
    { 6,10, 5, 2, 8, 0, 2,11, 8,-1,-1,-1,-1},
    { 3, 2,11,10, 5, 6,-1,-1,-1,-1,-1,-1,-1},
    { 9, 5, 6, 3, 9, 6, 3, 8, 9, 3, 6, 2,-1},
    { 5, 6, 9, 9, 6, 0, 6, 2, 0,-1,-1,-1,-1},
    { 0, 3, 8, 2, 5, 6, 2, 1, 5,-1,-1,-1,-1},
    { 1, 6, 2, 1, 5, 6,-1,-1,-1,-1,-1,-1,-1},
    {10, 5, 6, 9, 3, 8, 9, 1, 3,-1,-1,-1,-1},
    { 0, 9, 1, 5, 6,10,-1,-1,-1,-1,-1,-1,-1},
    { 8, 0, 3,10, 5, 6,-1,-1,-1,-1,-1,-1,-1},
    {10, 5, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {11, 7, 5,11, 5,10,-1,-1,-1,-1,-1,-1,-1},
    { 3, 0, 8, 7, 5,10, 7,10,11,-1,-1,-1,-1},
    { 9, 0, 1,10,11, 7,10, 7, 5,-1,-1,-1,-1},
    { 3, 1, 9, 3, 9, 8, 7,10,11, 7, 5,10,-1},
    { 2,11, 1,11, 7, 1, 1, 7, 5,-1,-1,-1,-1},
    { 0, 8, 3, 2,11, 1, 1,11, 7, 1, 7, 5,-1},
    { 9, 0, 2, 9, 2, 7, 2,11, 7, 9, 7, 5,-1},
    {11, 3, 2, 8, 5, 9, 8, 7, 5,-1,-1,-1,-1},
    {10, 2, 5, 2, 3, 5, 5, 3, 7,-1,-1,-1,-1},
    { 5,10, 2, 8, 5, 2, 8, 7, 5, 8, 2, 0,-1},
    { 9, 0, 1,10, 2, 5, 5, 2, 3, 5, 3, 7,-1},
    { 1,10, 2, 5, 8, 7, 5, 9, 8,-1,-1,-1,-1},
    { 1, 3, 7, 1, 7, 5,-1,-1,-1,-1,-1,-1,-1},
    { 8, 7, 0, 0, 7, 1, 7, 5, 1,-1,-1,-1,-1},
    { 0, 3, 9, 9, 3, 5, 3, 7, 5,-1,-1,-1,-1},
    { 9, 7, 5, 9, 8, 7,-1,-1,-1,-1,-1,-1,-1},
    { 4, 5, 8, 5,10, 8, 8,10,11,-1,-1,-1,-1},
    { 3, 0, 4, 3, 4,10, 4, 5,10, 3,10,11,-1},
    { 0, 1, 9, 4, 5, 8, 8, 5,10, 8,10,11,-1},
    { 5, 9, 4, 1,11, 3, 1,10,11,-1,-1,-1,-1},
    { 8, 4, 5, 2, 8, 5, 2,11, 8, 2, 5, 1,-1},
    { 3, 2,11, 1, 4, 5, 1, 0, 4,-1,-1,-1,-1},
    { 9, 4, 5, 8, 2,11, 8, 0, 2,-1,-1,-1,-1},
    {11, 3, 2, 9, 4, 5,-1,-1,-1,-1,-1,-1,-1},
    { 3, 8, 4, 3, 4, 2, 2, 4, 5, 2, 5,10,-1},
    {10, 2, 5, 5, 2, 4, 2, 0, 4,-1,-1,-1,-1},
    { 0, 3, 8, 5, 9, 4,10, 2, 1,-1,-1,-1,-1},
    { 2, 1,10, 9, 4, 5,-1,-1,-1,-1,-1,-1,-1},
    { 4, 5, 8, 8, 5, 3, 5, 1, 3,-1,-1,-1,-1},
    { 5, 0, 4, 5, 1, 0,-1,-1,-1,-1,-1,-1,-1},
    { 3, 8, 0, 4, 5, 9,-1,-1,-1,-1,-1,-1,-1},
    { 9, 4, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 7, 4,11, 4, 9,11,11, 9,10,-1,-1,-1,-1},
    { 3, 0, 8, 7, 4,11,11, 4, 9,11, 9,10,-1},
    {11, 7, 4, 1,11, 4, 1,10,11, 1, 4, 0,-1},
    { 8, 7, 4,11, 1,10,11, 3, 1,-1,-1,-1,-1},
    { 2,11, 7, 2, 7, 1, 1, 7, 4, 1, 4, 9,-1},
    { 3, 2,11, 4, 8, 7, 9, 1, 0,-1,-1,-1,-1},
    { 7, 4,11,11, 4, 2, 4, 0, 2,-1,-1,-1,-1},
    { 2,11, 3, 7, 4, 8,-1,-1,-1,-1,-1,-1,-1},
    { 2, 3, 7, 2, 7, 9, 7, 4, 9, 2, 9,10,-1},
    { 4, 8, 7, 0,10, 2, 0, 9,10,-1,-1,-1,-1},
    { 2, 1,10, 0, 7, 4, 0, 3, 7,-1,-1,-1,-1},
    {10, 2, 1, 8, 7, 4,-1,-1,-1,-1,-1,-1,-1},
    { 9, 1, 4, 4, 1, 7, 1, 3, 7,-1,-1,-1,-1},
    { 1, 0, 9, 8, 7, 4,-1,-1,-1,-1,-1,-1,-1},
    { 3, 4, 0, 3, 7, 4,-1,-1,-1,-1,-1,-1,-1},
    { 8, 7, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 8, 9,10, 8,10,11,-1,-1,-1,-1,-1,-1,-1},
    { 0, 9, 3, 3, 9,11, 9,10,11,-1,-1,-1,-1},
    { 1,10, 0, 0,10, 8,10,11, 8,-1,-1,-1,-1},
    {10, 3, 1,10,11, 3,-1,-1,-1,-1,-1,-1,-1},
    { 2,11, 1, 1,11, 9,11, 8, 9,-1,-1,-1,-1},
    {11, 3, 2, 0, 9, 1,-1,-1,-1,-1,-1,-1,-1},
    {11, 0, 2,11, 8, 0,-1,-1,-1,-1,-1,-1,-1},
    {11, 3, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 3, 8, 2, 2, 8,10, 8, 9,10,-1,-1,-1,-1},
    { 9, 2, 0, 9,10, 2,-1,-1,-1,-1,-1,-1,-1},
    { 8, 0, 3, 1,10, 2,-1,-1,-1,-1,-1,-1,-1},
    {10, 2, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 8, 1, 3, 8, 9, 1,-1,-1,-1,-1,-1,-1,-1},
    { 9, 1, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 8, 0, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
};

//Initialize all the variables and excess tables
void initve(float t, int r, int c, int p)
{
	thresh = t;
	row = r;
	col = c;
	pln = p;
	
	for(int i = 0; i < 12; i++)
	{
		coord c;
		c.p[0] = 0;
		c.p[1] = 0;
		c.p[2] = 0;
		
		ea.push_back(c);
		e.push_back(c);
		v.push_back(c);
	}
	
	e[2].p[0] = .5;		e[2].p[1] = 0; 		e[2].p[2] = 0;
	e[10].p[0] = 1;		e[10].p[1] = .5; 	e[10].p[2] = 0;
	e[6].p[0] = .5;		e[6].p[1] = 1; 		e[6].p[2] = 0;
	e[11].p[0] = 0;		e[11].p[1] = .5; 	e[11].p[2] = 0;
	e[0].p[0] = .5;		e[0].p[1] = 0; 		e[0].p[2] = 1;
	e[9].p[0] = 1;		e[9].p[1] = .5; 	e[9].p[2] = 1;
	e[4].p[0] = .5;		e[4].p[1] = 1; 		e[4].p[2] = 1;
	e[8].p[0] = 0;		e[8].p[1] = .5; 	e[8].p[2] = 1;
	e[3].p[0] = 0;		e[3].p[1] = 0;		e[3].p[2] = .5;
	e[1].p[0] = 1;		e[1].p[1] = 0;		e[1].p[2] = .5;
	e[5].p[0] = 1; 		e[5].p[1] = 1;	 	e[5].p[2] = .5;
	e[7].p[0] = 0;		e[7].p[1] = 1;	 	e[7].p[2] = .5;
	
	ea[0].p[0] = 0; 		ea[0].p[1] = 1;
	ea[1].p[0] = 2; 		ea[0].p[1] = 1;
	ea[2].p[0] = 3; 		ea[0].p[1] = 2;
	ea[3].p[0] = 3; 		ea[0].p[1] = 0;
	ea[4].p[0] = 4; 		ea[0].p[1] = 5;
	ea[5].p[0] = 6; 		ea[0].p[1] = 5;
	ea[6].p[0] = 7; 		ea[0].p[1] = 6;
	ea[7].p[0] = 7; 		ea[0].p[1] = 4;
	ea[8].p[0] = 0; 		ea[0].p[1] = 4;
	ea[9].p[0] = 1; 		ea[0].p[1] = 5;
	ea[10].p[0] = 2;		ea[10].p[1] = 6;
	ea[11].p[0] = 3;		ea[11].p[1] = 7;
	
	v[0].p[0] = 0;		v[0].p[1] = 0;		v[0].p[2] = 1;
	v[1].p[0] = 1;		v[1].p[1] = 0;		v[1].p[2] = 1;
	v[2].p[0] = 1;		v[2].p[1] = 0;		v[2].p[2] = 0;
	v[3].p[0] = 0;		v[3].p[1] = 0;		v[3].p[2] = 0;
	v[4].p[0] = 0;		v[4].p[1] = 1;		v[4].p[2] = 1;
	v[5].p[0] = 1;		v[5].p[1] = 1;		v[5].p[2] = 1;
	v[6].p[0] = 1;		v[6].p[1] = 1;		v[6].p[2] = 0;
	v[7].p[0] = 0;		v[7].p[1] = 1;		v[7].p[2] = 0;
}

vector<int> add_cube(vector<int> c)
{
	//Vector of edge values to be returned
	//Edge values are used to lookup in the edge table e
	vector<int> eVals;
	
	//index for the master aCase table
	int ind = 0;
	//get the index by converting that binary cube into 
	//an integer
	for(int i = 0; i < 8; i++)
	{
		//If it's not -1 we'll do stuff normally
		if(c[i] != -1)
			ind += c[i] * pow(2, i);
		//If it is, we'll set it to a 1 bit
		else
			ind += 1 * pow(2, i);
	}
	
	//Go through the aCase table for that ind
	for(int i = 0; i < 13; i++)
	{
		//If the entry is -1, we're done
		if(aCases[ind][i] == -1)
			break;
		//Push back the edge values in the table
		eVals.push_back(aCases[ind][i]);
	}
	
	//return the edge tables
	return eVals;
}

void getPts(vector<coord> &pts, vector<float> vals, vector<int> eVals, int x, int y, int z, vector<int> bins)
{
	//3 coordinates
		//p0 is the current place we are at (x y z)
			//It's used as a reference location
		//p1 is the lower vertex of a given edge
		//p2 is the upper vertex of a given edge
		//  [p1]--------[e]--------[p2]
	coord p0, p1, p2;
	//set up p0
	p0.p[0] = x;
	p0.p[1] = y;
	p0.p[2] = z;
	
	//go through the eVals table
	for(int i = 0; i < eVals.size(); i++)
	{
		//coordinate to be pushed back to points vector
		coord p;
		//We only need to adjust the midpoints of each
		//point, so this will be used as an index
		int chn;
		
		//go through and see which value will be chn
		//Also fill in the rest of p
		for(int j = 0; j < 3; j++)
		{	
			//if we aren't at the midpoint, we'll set
			//the point values
			if(e[eVals[i]].p[j] != 0.5)
			{
				p1.p[j] = p0.p[j] + e[eVals[i]].p[j];
				p2.p[j] = p0.p[j] + e[eVals[i]].p[j];
				p.p[j] = p0.p[j] + e[eVals[i]].p[j];
			}
			//if not, we'll setup p1 and p2
			//We'll also set j
			else
			{
				p1.p[j] = p0.p[j];
				p2.p[j] = p0.p[j] + 1;
				chn = j;
			}
		}
		
		//indexes to use to lookup in the vals vector
		int i1, i2;
		//values at p1 and p2
		float v1, v2;
		
		//compute i1 and i2
		i1 = p1.p[0] + p1.p[2]*row*pln + p1.p[1]*row;
		i2 = p2.p[0] + p2.p[2]*row*pln + p2.p[1]*row;
		
		//get v1 and v2
		v1 = vals[i1];
		v2 = vals[i2];
		
		//If v1 and v2 are equal, we'll set the point in between
			//This case would create a divide by 0 error too
		if(abs(v2 - v1) == 0)
			p.p[chn] = (float)(p1.p[chn] + p2.p[chn]) / 2;
		//otherwise, we'll interpolate
		else
		{
			//d1 is the difference between the thresh hold and v1
			float d1 = (thresh - v1);
			//d2 is the difference between v2 and v1
			float d2 = (v2 - v1);
			//time used to interpolate with is d1 / d2
			float t = d1 / d2;
			
			//interpolate
			p.p[chn] = p1.p[chn] + t * (p2.p[chn] - p1.p[chn]);
		}
		
		//push back p
		pts.push_back(p);
	}
}

void drawPts(vector<coord> pts, vector<coord> norms, bool nor)
{
	//if we are doing normals, print it out
	if(nor)
		cout << "PolySet \"PN\"\n";
	//if not, don't print N
	else
		cout << "PolySet \"P\"\n";
	
	//number of vertices is the number of points in the table
	int nVerts = pts.size();
	//number of faces is that number divided by 3
	int nFaces = nVerts / 3;
	
	//output those numbers
	cout << nVerts << " " << nFaces << endl;
	
	//go through the vertex list
	for(int i = 0; i < nVerts; i++)
	{
		//output the vertex
		for(int j = 0; j < 3; j++)
		{
			cout << pts[i].p[j] << " ";
		}
		//if normals set, output those too
		if(nor)
		{
			for(int j = 0; j < 3; j++)
			{
				cout << norms[i].p[j] << " ";
			}
		}
		//end the line
		cout << endl;
	}
	
	//go through and output the vertex list
	for(int i = 0; i < nVerts;)
	{
		//output the numbers in order
		//that's how they are in the list
		for(int j = 0; j < 3; j++)
			cout << i++ << " ";
		//every 3 is a completed face
		cout << "-1\n";
	}
}

vector<coord> draw(vector<int> bins, vector<float> vals, int row, int col, int pln)
{
	//points we will return
	vector<coord> pts;
	//int x y z to keep track of where we are
	int x, y, z;
	x = 0;
	y = x;
	z = y;
	
	//traverse through the 3-D space
    for(y = 0; y < row-1; y++)
	{
        for(z = 0; z < pln-1; z++)
		{
			for(x = 0; x < col-1; x++)
			{
				//binary cube of data
				vector<int> c;
				//edge values based on that cube
				vector<int> eVals;
				//indices for c values
				int vs[8];
				
				//compute indices
				vs[3] = (x) + (z)*row*pln + (y)*row;
				vs[2] = (x+1) + (z)*row*pln + (y)*row;
				vs[0] = (x) + (z+1)*row*pln + (y)*row;
				vs[1] = (x+1) + (z+1)*row*pln + (y)*row;
				vs[7] = (x) + (z)*row*pln + (y+1)*row;
				vs[6] = (x+1) + (z)*row*pln + (y+1)*row;
				vs[4] = (x) + (z+1)*row*pln + (y+1)*row;
				vs[5] = (x+1) + (z+1)*row*pln + (y+1)*row;
				
				//push back the c values (which are the binary values
				//on the cube
				for(int j = 0; j < 8; j++)
				{
					c.push_back(bins[vs[j]]);
					if(vs[j] > (row)*(col)*(pln))
						cerr << "Out of bounds!\n";
				}
				
				//get the edge values for that cube
				eVals = add_cube(c);
				
				//get the actual points of the the polygons based on those
				//edges
				getPts(pts, vals, eVals, x, y, z, bins);
			}
		}
	}
	
	//return the points
	return pts;
}

vector<coord> getNor(vector<coord> points, int row, int col, int pln)
{
	//a mapping between a string form of the coordinate
	//contains 
		//the normal vectors of all the faces associated with the coord
		//the average normal (calculated last)
	map<string, fNorm> faceNorms;
	//the actual vector of normals that will be returned
	vector<coord> normals;
	
	//initialize normals and faceNorms
	for(int i = 0; i < points.size(); i++)
	{
		string s = to_str(points[i]);
		fNorm n;
		faceNorms[s] = n;
		
		coord c;
		for(int j = 0; j < 3; j++)
			c.p[j] = 0;
		normals.push_back(c);
	}
	
	//compute the normals
	for(int i = 0; i < points.size(); i += 3)
	{
		//coordinate holder for a normal
		coord nor;
		
		//compute the sides of the triangle that we will
		//compute the normals for
		coord sides[2];
		sides[0] = subtract(points[i+1], points[i]);
		sides[1] = subtract(points[i+2], points[i]);
		
		//compute the normals by doing the cross produce
		nor = cross(sides[0], sides[1]);
		
		//if the normal is the zero vector
		//we will compute again with 2 different sides
		if(isZero(nor))
		{
			sides[0] = subtract(points[i], points[i+1]);
			sides[1] = subtract(points[i+2], points[i+1]);
			
			nor = cross(sides[1], sides[0]);
		}
		//if it's still zero, we'll use the last two sides
		//if it's still zero after that, there's nothing we can do
		//we've done all we can
		if(isZero(nor))
		{
			sides[0] = subtract(points[i], points[i+2]);
			sides[1] = subtract(points[i+1], points[i+2]);
			
			nor = cross(sides[0], sides[1]);
		}
		
		float len = nor.p[0]*nor.p[0]+nor.p[1]*nor.p[1]+nor.p[2]*nor.p[2];
		len = sqrt(len);
		if(len != 0)
		{
		//	nor.p[0] /= len;
		//	nor.p[1] /= len;
		//	nor.p[2] /= len;
		}
		
		//put the normal into the map
		for(int j = 0; j < 3; j++)
		{
			int k = j + i;
			string vert = to_str(points[k]);
			faceNorms[vert].fn.push_back(nor);
		}
	}
	
	//compute all the average normal for each vertex
	for(map<string, fNorm>::iterator i = faceNorms.begin(); 
		i != faceNorms.end(); 
		i++)
	{
		coord c = average(i -> second.fn);
		i -> second.nor = c;
	}
	
	//store those normals into the normal vector to be returned
	for(int i = 0; i < points.size(); i++)
	{
		string vert = to_str(points[i]);
		normals[i] = faceNorms[vert].nor;
	}
	
	//return the normal vector
	return normals;
}

coord average(vector<coord> fn)
{
	//coordinate to be returned
	coord c;
	
	//initialize c
	for(int i = 0; i < 3; i++)
	{
		c.p[i] = 0;
	}
	
	//add all the normals together
	//since the number of normals for a given vertex is fixed
		//we do not need to divide, we only care about direction,
		//not length
	for(int i = 0; i < fn.size(); i++)
	{
		c = add(c, fn[i]);
	}
	
	//return the average coordinate
	return c;
}

coord add(coord a, coord b)
{
	//coordinate to be returned
	coord c;
	
	//for each value in the coordinate, add them
	for(int i = 0; i < 3; i++)
	{
		c.p[i] = a.p[i] + b.p[i];
	}
	
	//return the coord
	return c;
}

/*
	Conversion to string taken from:
		http://stackoverflow.com/questions/5793986/convert-float-to-string
*/
string to_str(float f)
{
	//string stream to store the float
	ostringstream buff;
    buff << f;
    //convert it to a string and return
    return buff.str(); 
}

string to_str(coord c)
{
	//initialize string
	string s = "";
	//add each value in c followed by a space
	for(int j = 0; j < 3; j++)
		s += to_str(c.p[j]) + " ";
	
	//return that string
	return s;
}

coord cross(coord a, coord b)
{
	//coordinate to be returned
	coord c;
	
	//compute cross product
	c.p[0] = a.p[1]*b.p[2] - a.p[2]*b.p[1];
	c.p[1] = a.p[2]*b.p[0] - a.p[0]*b.p[2];
	c.p[2] = a.p[0]*b.p[1] - a.p[1]*b.p[0];
	
	return c;
}

bool isZero(coord c)
{
	//go through each value in c
	//if it's not zero, return false
	for(int i = 0; i < 3; i++)
	{
		if(c.p[i] != 0)
			return false;
	}
	
	//if we hit the end, it's the zero vector
	//return true
	return true;
}

coord subtract(coord a, coord b)
{
	//look at add function (above)
	coord c;
	for(int i = 0; i < 3; i++)
	{
		c.p[i] = a.p[i] - b.p[i];
	}
	
	return c;
}

vector<coord> getNor(vector<int> bins, vector<float> vals)
{
	vector<coord> normals;
	vector<coord> cubeNor;
	
	//int x y z to keep track of where we are
	int x, y, z;
	
    for(y = 0; y < row; y++)
	{
        for(z = 0; z < pln; z++)
		{
			for(x = 0; x < col; x++)
			{
				coord nor;
				float cubeLen[3];
				for(int i = 0; i < 3; i++)
					cubeLen[i] = 2.0;
				int vp[3];
				int vm[3];
				int v = (x) + (z)*row*pln + (y)*row;
				vp[0] = (x + 1) + (z)*row*pln + (y)*row;
				vm[0] = (x - 1) + (z)*row*pln + (y)*row;
				vp[1] = (x) + (z)*row*pln + (y + 1)*row;
				vm[1] = (x) + (z)*row*pln + (y - 1)*row;
				vp[2] = (x) + (z + 1)*row*pln + (y)*row;
				vm[2] = (x) + (z - 1)*row*pln + (y)*row;
				
				if(v > row*col*pln)
					cerr << "Out of bounds!\n";
				
				if(x + 1 >= col)
				{
					vp[0] = v;
					cubeLen[0] = 1;
				}
				if(y + 1 >= row)
				{
					vp[1] = v;
					cubeLen[1] = 1;
				}
				if(z + 1 >= pln)
				{
					cubeLen[2] = 1;
					vp[2] = v;
				}
				if(x - 1 < 0)
				{
					vm[0] = v;
					cubeLen[0] = 1;
				}
				if(y - 1 < 0)
				{
					vm[1] = v;
					cubeLen[1] = 1;
				}
				if(z - 1 < 0)
				{
					vm[2] = v;
					cubeLen[2] = 1;
				}
				
				for(int i = 0; i < 3; i++)
				{
					nor.p[i] = vals[vp[i]] - vals[vm[i]];
					nor.p[i] /= cubeLen[i];
				}
				
				cubeNor.push_back(nor);
			}
		}
	}

	//traverse through the 3-D space
    for(y = 0; y < row-1; y++)
	{
        for(z = 0; z < pln-1; z++)
		{
			for(x = 0; x < col-1; x++)
			{
				//binary cube of data
				vector<int> c;
				//edge values based on that cube
				vector<int> eVals;
				//indices for c values
				int vs[8];
				
				//compute indices
				vs[3] = (x) + (z)*row*pln + (y)*row;
				vs[2] = (x+1) + (z)*row*pln + (y)*row;
				vs[0] = (x) + (z+1)*row*pln + (y)*row;
				vs[1] = (x+1) + (z+1)*row*pln + (y)*row;
				vs[7] = (x) + (z)*row*pln + (y+1)*row;
				vs[6] = (x+1) + (z)*row*pln + (y+1)*row;
				vs[4] = (x) + (z+1)*row*pln + (y+1)*row;
				vs[5] = (x+1) + (z+1)*row*pln + (y+1)*row;
				
				//push back the c values (which are the binary values
				//on the cube
				for(int j = 0; j < 8; j++)
				{
					c.push_back(bins[vs[j]]);
					if(vs[j] > (row)*(col)*(pln))
						cerr << "Out of bounds!\n";
				}
				
				//get the edge values for that cube
				eVals = add_cube(c);
				
				//get the actual points of the the polygons based on those
				//edges
				getNor(normals, cubeNor, vals, eVals, x, y, z);
			}
		}
	}
	
	return normals;
}

void getNor(vector<coord> &nors, vector<coord> cubeNor, vector<float> vals, vector<int> eVals, int x, int y, int z)
{
	//3 coordinates
	//p0 is the current place we are at (x y z)
		//It's used as a reference location
	//p1 is the lower vertex of a given edge
	//p2 is the upper vertex of a given edge
	//  [p1]--------[e]--------[p2]
	coord p0, p1, p2;
	//set up p0
	p0.p[0] = x;
	p0.p[1] = y;
	p0.p[2] = z;
	
	//go through the eVals table
	for(int i = 0; i < eVals.size(); i++)
	{
		//coordinate to be pushed back to points vector
		coord p;	
		//go through and see which value will be chn
		//Also fill in the rest of p
		for(int j = 0; j < 3; j++)
		{	
			//if we aren't at the midpoint, we'll set
			//the point values
			if(e[eVals[i]].p[j] != 0.5)
			{
				p1.p[j] = p0.p[j] + e[eVals[i]].p[j];
				p2.p[j] = p0.p[j] + e[eVals[i]].p[j];
			}
			//if not, we'll setup p1 and p2
			//We'll also set j
			else
			{
				p1.p[j] = p0.p[j];
				p2.p[j] = p0.p[j] + 1;
			}
		}	
		//indexes to use to lookup in the vals vector
		int i1, i2;
		//values at p1 and p2
		float v1, v2;
		
		//compute i1 and i2
		i1 = p1.p[0] + p1.p[2]*row*pln + p1.p[1]*row;
		i2 = p2.p[0] + p2.p[2]*row*pln + p2.p[1]*row;
		
		if(i1 > cubeNor.size() || 
		   i2 > cubeNor.size() ||
		   i1 > vals.size() ||
		   i2 > vals.size() )
			cerr << "Out of range!\n";
		
		//get v1 and v2
		v1 = vals[i1];
		v2 = vals[i2];
		
		coord n1 = cubeNor[i1];
		coord n2 = cubeNor[i2];
		
		for(int j = 0; j < 3; j++)
		{
			//If v1 and v2 are equal, we'll set the point in between
				//This case would create a divide by 0 error too
			if(abs(v2 - v1) == 0)
				p.p[j] = (n1.p[j] + n2.p[j]) / 2;
			//otherwise, we'll interpolate
			else
			{
				//d1 is the difference between the thresh hold and v1
				float d1 = (thresh - v1);
				//d2 is the difference between v2 and v1
				float d2 = (v2 - v1);
				//time used to interpolate with is d1 / d2
				float t = d1 / d2;
		
				//interpolate
				p.p[j] = n1.p[j] + t * (n2.p[j] - n1.p[j]);
			}
		}
		
		//push back p
		nors.push_back(p);
	}
}
