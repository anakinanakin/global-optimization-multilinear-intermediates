#pragma once

#include "multilinear.h"

using namespace std;

class Reduce
{
	public:
		Reduce(Multilinear m, vector<Multilinear>& L);

		Multilinear remove(Multilinear m, float a);
		
	private:
		//μk denote the number of multilinears in L in which term k appears
		vector<int> u;
		//number of covered terms(none 0 elements in u)
		int ncv;
		vector<int> covered_var;

		vector<int> candi_covered_var(Multilinear m);
};