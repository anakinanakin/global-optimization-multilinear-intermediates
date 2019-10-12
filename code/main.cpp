//g++ main.cpp one.cpp five.cpp multilinear.cpp -lmetis
#include <math.h>
#include <metis.h>
#include <thread>

#include "multilinear.h"
#include "one.h"
#include "five.h"


using namespace std;

int myceil(int numerator, int denominator);

int main(int argc, char *argv[])
{
	Multilinear m(1);
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

	cout << "m:";
	r.mprint();
	cout << "y:";
	r.yprint();
	cout << "f:";
	r.fprint();
	cout << "";

	Graph g(m);
	g.toString();

	//algorithm 5 

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

	/*int *first_vertices = g.get_connected_components_num();

	cout << " first_vertices: ";
	for (int i = 0; i < csize; ++i)
	{
		cout << first_vertices[i] << " ";
	}*/

	//L for cut generation
	vector<Multilinear> L;
	vector<int> v;
	Multilinear lt;
	vector<Multilinear> ccomponents;

	//for all connected components gt
	for (int i = 0; i < csize; ++i){ 
		v = g.get_connected_components_vertices(i);
		//get the corresponding multilinear of the component
		lt = Multilinear(m, v);
		//store all connected components for finding biconnected components
		ccomponents.push_back(lt);

		//if nt=3 && gt has 3 edges
		if (g.get_component_size(i) == 3 && g.three_edge_component(i)){

			cout << "v: ";
			for (int j = 0; j < v.size(); ++j){
				cout << v[j] << " ";
			}
			
			//store lt for cut generation
			L.push_back(lt);
		}
	}

	cout << "L.size = " << L.size() << endl;
	cout << "L:";
	for (int i = 0; i < L.size(); ++i){
		L[i].toString();
		cout << endl;
	}

	vector<struct Edge> vec_edge;
	int bcsize;
	int ntk;
	Multilinear ltk;
	//uncovered multilinears U for further decomposition
	vector<Multilinear> U;

	//for each connected components gt
	for (int i = 0; i < csize; ++i){
		ccomponents[i].toString();

		//if univariable term, skip it or there'll be error for compute_biconnected_components()
		if (ccomponents[i].getsize() == 1 && ccomponents[i].get_term_size(0) == 2) {
			continue;
		}

		//need to use this initialization method to prevent error
		Graph gt(ccomponents[i]);
		
		gt.compute_biconnected_components();

		bcsize = gt.get_biconnected_components_size();

		//for each biconnected components gtk of gt
		for (int j = 0; j < bcsize; ++j) {
			//get vertice num of a biconnected component
			ntk = gt.get_bicomponent_size(j);

			if (ntk >= 3) {
				vec_edge = gt.get_biconnected_components_edges(j);
				//cout << "\nvec_edge: ";
				//for (int k = 0; k < vec_edge.size(); ++k) {
					//cout << vec_edge[k].v << "  " << vec_edge[k].i << endl;
				//}

				ltk = Multilinear(m, vec_edge);
				ltk.toString();

				if (ntk <= nmin) {
					//store Ltk for cut generation
					L.push_back(ltk);
				}
				else {
					//add Ltk to the list of uncovered multilinears U
					U.push_back(ltk);		
				}
			}
		}
	}

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

	//math.ceil() has problem with fraction, build myceil()
	int pmin = myceil(n, nmax);
	int pmax = floor(n/nmin);
	
	int sp, rp;

	//number of threads
	//unsigned threads = thread::hardware_concurrency();

	/*MPI_Comm comm;
	int mpisize, myrank;

    MPI_Init(&argc, &argv);
  	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
  	MPI_Comm_size(comm, &mpisize);
  	MPI_Comm_rank(comm, &myrank);*/

	//parameters for parmetis
	//idx_t nvtxs = n;
	idx_t ncon;//suppose it's rp
	//for xadj, adjncy representation explaination:
	//https://www.researchgate.net/figure/Compressed-Sparse-Row-CSR-representation-Every-graph-can-be-represented-as-an_fig3_324640550
	idx_t xadj[n+1];
	idx_t adjncy[e];
	//idx_t vtxdist[mpisize+1];
	idx_t nparts;
	idx_t objval;
    idx_t part[n];
    //idx_t npes = mpisize, mype = myrank;
    idx_t numflag = 0, wgtflag = 0;
    idx_t options[1] = {0};
    idx_t edgecut = 0;
    idx_t ndims = 1;
    real_t xyz[n];

	int *tempxadj = g.get_csr_vertex();
	int *tempadjncy = g.get_csr_edge();
	//int *tempvtxdist = g.get_csr_processor(mpisize);

	cout << "xadj:";
	for (int i = 0; i < n+1; ++i){
		xadj[i] = tempxadj[i];
		cout << xadj[i] << " ";
	}

	cout << "adjncy:";
	for (int i = 0; i < e; ++i){
		adjncy[i] = tempadjncy[i];
		cout << adjncy[i] << " ";
	}

	/*
	cout << "vtxdist:";
	for (int i = 0; i < mpisize+1; ++i){
		vtxdist[i] = tempvtxdist[i];
		cout << vtxdist[i] << " ";
	}
	//cout << endl;

	for (int i = 0; i < n; ++i){
		xyz[i] = 0.0;
	}*/

	int _metis;
	//real_t tpwgts = 0.0, ubvec = 0.0;
	
	/*//for (int p = pmin; p <= pmax; ++p){
	cout << "d2";
	//cout << "n:" << n << endl;
	//cout << "pmax:" << pmax << endl;
	//cout << "pmin:" << pmin << endl;

	cout << "p:" << p;

	sp = myceil(n, p);//nominal partition size
	cout << "sp:" << sp;
	cout << "nmax:" << nmax;
	rp = nmax/sp;//load imbalance
	//ncon = rp;//ncon can't be 0
	//nparts = p;*/
	ncon = 3;
	nparts = 3;

	/*real_t tpwgts[ncon*nparts];
	real_t ubvec[ncon];

	//tpwgts = rmalloc(nparts*ncon, "tpwgts");
	//ubvec = new real_t[ncon];//ncon can't be 0

	for (int i = 0; i < ncon*nparts; ++i)
	{
		tpwgts[i] = 1/nparts;
	}

	for (int i = 0; i < ncon; ++i)
	{
		ubvec[i] = 1.05;
	}

	cout << "mpisize: " << mpisize;//mpisize = 1, need more

	//par_metis = ParMETIS_V3_PartKway(vtxdist, xadj, adjncy, NULL, NULL, &wgtflag, &numflag, &ncon, &nparts, tpwgts, ubvec, options, &edgecut, part, &comm);
	*/
	idx_t nvtxs = n;

	//both methods work
	_metis = METIS_PartGraphRecursive(&nvtxs, &ncon, xadj, adjncy, NULL, NULL, NULL, &nparts, NULL, NULL, NULL, &objval, part);
	//_metis = METIS_PartGraphKway(&nvtxs, &ncon, xadj, adjncy, NULL, NULL, NULL, &nparts, NULL, NULL, NULL, &objval, part);

	//cout << "edgecut:" << edgecut;

	cout << "objval:" << objval;
	cout << " _metis:" << _metis;
	cout << endl;
	//delete[] ubvec;
	//}

	//MPI_Comm_free(&comm);
	//MPI_Finalize();

	return 0;
}

int myceil(int numerator, int denominator){
	if (numerator == denominator){
		return floor(numerator/denominator);
	}
	else{
		return floor(numerator/denominator)+1;
	}
}










