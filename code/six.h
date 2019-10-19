#pragma once

#include <iostream>
#include <math.h>
#pragma once

#include <metis.h>

#include "five.h"

using namespace std;

class Partition
{
	public:
		//default arguments should be in the declaration but not in the definition
		Partition(Graph g, float B1 = 0.1, float B2 = 1.25, int nmin = 4, int nmax = 15);

		vector<Multilinear> identify_dense();

	private:
		//gain factor Θ
		int o;
		int pmin;
		int pmax;
		int sp;
		int rp;

		idx_t nvtxs;
		idx_t ncon;//suppose it's rp
		//for xadj, adjncy representation explaination:
		//https://www.researchgate.net/figure/Compressed-Sparse-Row-CSR-representation-Every-graph-can-be-represented-as-an_fig3_324640550
		idx_t *xadj;
		idx_t *adjncy;
		idx_t nparts;
		idx_t objval;
    	idx_t *part;

		int myceil(int numerator, int denominator);
};






