#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

#include "edge.h"

#define NMAX 2
#define NMIN 1

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
		//distinct variable nums
    	vector<int> varnums;

    	//void reduce_dimension(int term, int nk, int nmax);
};






