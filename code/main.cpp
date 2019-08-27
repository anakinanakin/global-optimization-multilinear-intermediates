//g++ main.cpp one.cpp five.cpp multilinear.cpp -lmetis
#include <metis.h>
#include <math.h>

#include "multilinear.h"
#include "one.h"
#include "five.h"

using namespace std;

int main()
{
	Multilinear m;
	m.toString();

	Relation r;

	//algorithm 1
	//multilinear function
	for (int i = 0; i < m.getsize(); ++i){
		int l = m.get_term_size(i);//get term size plus 1(coef)
		int term_size = m.get_term_size(i) - 1;//get term size
		int coef = m.getvar(i,0); //get term coefficient
		int x = m.getvar(i,1); //get first x var

		r.addj(1);
		//the first term has the coefficient coef
		r.ypush(r.getj(), coef, x);
		r.fpush(coef, x);

		for (int j = 2; j < l; ++j){
			r.addj(1);
			//others have coef=1
			r.ypush(r.getj(), 1, m.getvar(i,j));
			r.fpush(1, m.getvar(i,j));
		}

		r.addj(1);
		r.mpush(r.getj(), 1, r.getj()-term_size, r.getj()-term_size+1);

		for (int j = 3; j < l; ++j){
			r.addj(1);
			r.mpush(r.getj(), 1, r.getj()-1, r.getj()-term_size+1);
		}
	}

	int fsize = r.fgetsize();

	//basic univariate function in f
	for (int i = 0; i < fsize; ++i){
		r.addj(1);
		r.mpush(r.getj(), r.fgetcoef(i), r.fgetval(i), -1);
	}

	cout << "\nm:";
	r.mprint();
	cout << "\ny:";
	r.yprint();
	cout << "\nf:";
	r.fprint();
	cout << "\n";

	Graph g(m);
	g.toString();

	//algorithm 5 
	//todo:Lt and Ltk, wait to decide data structure

	//if the number of variables in L(x) does not exceed nmin, 
	//we do not attempt to break it down to smaller multilinears but store it for cut generation.
	//use nmin in paper
	int nmin = 4;

	//input nmin
	//cout << "input nmin for decomposition:";
	//cin >> nmin;

	g.compute_connected_components();
	g.print_connected_components();

	int csize = g.get_connected_components_size();
	//for all gt
	for (int i = 0; i < csize; ++i){ 
		//if nt=3 && gt has 3 edges
		if (g.get_component_size(i) == 3 && g.three_edge_component(i)){
			//store Lt for cut generation
		}
	}

	g.compute_biconnected_components();

	//for all gtk:
	//if (ntk >= 3){
		//if (ntk <= nmin){
			//store Ltk for cut generation
		//}
		//else{
		//	add Ltk to the list of uncovered multilinears
		//}
	//}

	//algorithm 6

	//nmin: lower bound on the size of each partition element
	//nmax: upper bound on the size of each partition element
	//pmin: lower bound on number of partition elements
	//pmax: upper bound on number of partition elements
	//For a graph with n nodes, we define pmin = ⌈n/nmax⌉ and pmax = ⌊n/nmin⌋
	//For each number of partition elements p, we define a nominal partition size sp = ⌈n/p⌉
	//load imbalance rp = nmax/sp
	//Let Dρ be the set of multilinear functions associated with ρ and let NLP = |Dρ|
	//Denote by Ne the total number of multilinear terms in Dρ that are not present in the cut generation list L
	//Let δ or d denote the maximum number of variables in the multilinear functions in Dρ
	//define a gain factor Θ or O where 0 < β1 ≤ 1 ≤ β2

	//Given a multilinear function L(x) with the weighted biconnected graph G = (V, E)
	//use g for now
	
	//use parameters in the paper
	float B1 = 0.1, B2 = 1.25;
	int nmax = 15;

	//input the set of parameters {β1, β2, nmin, nmax}.
	//cout << "input B1 for graph partitioning(0 < B1 ≤ 1): ";
	//cin >> B1;
	//cout << "input B2 for graph partitioning(1 ≤ B2): ";
	//cin >> B2;
	//cout << "input nmax for graph partitioning(nmax >= " << nmin << "): ";
	//cin >> nmax;

	//Initialize the gain factor Θ = 0, and the set of dense components D = ∅
	int o = 0;

	//Compute ρmin and ρmax. Let ρ = ρmin.
	int n = g.get_vertice_size();
	int e = g.get_edge_size();
	int pmin = ceil(n/nmax);
	int pmax = floor(n/nmin);
	int sp, rp;

	//parameters for metis
	idx_t nvtxs = n;
	idx_t ncon;//suppose it's rp
	//for xadj, adjncy representation explaination:
	//https://www.researchgate.net/figure/Compressed-Sparse-Row-CSR-representation-Every-graph-can-be-represented-as-an_fig3_324640550
	idx_t xadj[n+1];
	idx_t adjncy[e];
	idx_t nparts;
	idx_t objval;
    idx_t part[n];

	int *tempxadj = g.get_csr_vertex();
	int *tempadjncy = g.get_csr_edge();

	for (int i = 0; i < n+1; ++i){
		xadj[i] = tempxadj[i];
	}

	for (int i = 0; i < e; ++i){
		adjncy[i] = tempadjncy[i];
	}

	int metis_ret;

	for (float p = pmin; p <= pmax; ++p){
		sp = ceil(n/p);//nominal partition size
		rp = nmax/sp;//load imbalance
		ncon = rp;
		nparts = p;

		metis_ret = METIS_PartGraphRecursive(&nvtxs, &ncon, xadj, adjncy, NULL, NULL, NULL, &nparts, NULL,
		NULL, NULL, &objval, part);

	}

	return 0;
}










