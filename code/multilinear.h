#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "edge.h"

using namespace std;

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
};