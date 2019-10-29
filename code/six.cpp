#include "six.h"

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

//g is a weighted biconnected graph
//use parameters in the paper
Partition::Partition(Graph g, float B1, float B2, int nmin, int nmax):o(0) {
	//Compute ρmin and ρmax. Let ρ = ρmin.
	int n = g.get_vertice_size();
	int e = g.get_edge_size();

	//math.ceil() has problem with fraction, build myceil()
	pmin = myceil(n, nmax);
	pmax = floor(n/nmin);

	nvtxs = n;
	xadj = new idx_t[n+1];
	adjncy = new idx_t[e];
	part = new idx_t[n];

	int *tempxadj = g.get_csr_vertex();
	int *tempadjncy = g.get_csr_edge();

	cout << "\nxadj:";
	for (int i = 0; i < n+1; ++i){
		xadj[i] = tempxadj[i];
		cout << xadj[i] << " ";
	}

	cout << "adjncy:";
	for (int i = 0; i < e; ++i){
		adjncy[i] = tempadjncy[i];
		cout << adjncy[i] << " ";
	}
}

vector<Multilinear> Partition::identify_dense() {
	int _metis;
	vector<Multilinear> D;

	/*sp = myceil(n, p);//nominal partition size
	cout << "sp:" << sp;
	cout << "nmax:" << nmax;
	rp = nmax/sp;//load imbalance
	ncon = rp;//ncon can't be 0
	nparts = p;

	for (int p = pmin; p <= pmax; ++p) {
		cout << "d2";
		cout << "n:" << n << endl;
		cout << "pmax:" << pmax << endl;
		cout << "pmin:" << pmin << endl;
		cout << "p:" << p;
	}*/

	ncon = 3;
	nparts = 3;

	//both methods work
	_metis = METIS_PartGraphRecursive(&nvtxs, &ncon, xadj, adjncy, NULL, NULL, NULL, &nparts, NULL, NULL, NULL, &objval, part);
	//_metis = METIS_PartGraphKway(&nvtxs, &ncon, xadj, adjncy, NULL, NULL, NULL, &nparts, NULL, NULL, NULL, &objval, part);

	cout << "objval:" << objval;
	cout << " _metis:" << _metis << endl;

	return D;
}

int Partition::myceil(int numerator, int denominator) {
	if (numerator == denominator){
		return floor(numerator/denominator);
	}
	else{
		return floor(numerator/denominator)+1;
	}
}

Partition::~Partition() {
	delete[] xadj;
	delete[] adjncy;
	delete[] part;
}







