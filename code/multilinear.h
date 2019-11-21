#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

//default settings in the paper
#define NMIN 4
#define NMAX 15
#define B1 0.1
#define B2 1.25
#define A 0.6
#define RC 0.85

using namespace std;

int myceil(int numerator, int denominator);
int min(int a, int b);

class Multilinear
{
	public:
		Multilinear();
		Multilinear(int a);
		Multilinear(Multilinear m, const vector<int>& v);
		Multilinear(Multilinear m, const vector<struct Edge>& v);
		Multilinear(Multilinear m, const vector<int>& v, int a);

		int getsize();
		int get_term_size(int term);
		int getvar(int term, int var);
		void remove_term(int term);
		void toString();
		vector<int> get_varnums();
		bool repeated_var();
		bool isEmpty();

	private:
		vector< vector<int> > func;
    	vector<int> varnums;//distinct variable nums

    	//void reduce_dimension(int term, int nk, int nmax);
};

//edge for representing biconnected components
struct Edge 
{  
    int v; 
    int i; 
    Edge(int a, int b){
    	v = a;
    	i = b;
    }
};






