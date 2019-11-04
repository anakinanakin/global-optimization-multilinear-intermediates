#pragma once

#include <iostream>
#include <math.h>
#include <metis.h>

#include "graph.h"

using namespace std;

class Partition
{
	public:
		//default arguments should be in the declaration but not in the definition
		Partition(Graph g);

		vector<Multilinear> identify_dense(Multilinear m, vector<Multilinear> L);

		~Partition();

	private:
		int n;
		int e;
		int nmax;
		int nmin;
		int pmin;
		int pmax;
		int sp;
		int rp;

		idx_t nvtxs;
		idx_t ncon;//suppose it's rp
		//for xadj, adjncy representation explaination:
		//https://www.researchgate.net/figure/Compressed-Sparse-Row-CSR-representation-Every-graph-can-be-represented-as-an_fig3_324640550
		idx_t* xadj;
		idx_t* adjncy;
		idx_t nparts;
		idx_t objval;
    	idx_t* part;

		int myceil(int numerator, int denominator);
};






