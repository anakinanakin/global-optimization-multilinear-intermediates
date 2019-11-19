#pragma once

#include "multilinear.h"

using namespace std;

int compute_covered(Multilinear m, vector<Multilinear>& v, vector<int>& u);

class Reduce
{
	public:
		Reduce(Multilinear m, vector<Multilinear>& L);

		Multilinear remove(Multilinear m, float a);
		
	private:
		vector<int> u;//μk denote the number of multilinears in L in which term k appears
		int ncv;//number of covered terms(none 0 elements in u)
		vector<int> covered_var;

		vector<int> candi_covered_var(Multilinear m);
};