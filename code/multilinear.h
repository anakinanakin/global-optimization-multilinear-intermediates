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
		Multilinear(Multilinear m, vector<int> v);
		Multilinear(Multilinear m, vector<struct Edge> v);

		int getsize();
		int get_term_size(int term);
		int getvar(int term, int var);
		void toString();
		vector<int> get_varnums();

	private:
		vector< vector<int> > func;
		//distinct variable nums
    	vector<int> varnums;
};