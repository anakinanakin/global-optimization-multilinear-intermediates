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
//Let δ denote the maximum number of variables in the multilinear functions in Dρ
//define a gain factor Θ where 0 < β1 ≤ 1 ≤ β2

//g is a weighted biconnected graph
//use parameters in the paper:nmin(4), nmax(15)
Partition::Partition(Graph g):nmin(NMIN), nmax(NMAX) {
	//Compute ρmin and ρmax. Let ρ = ρmin.
	n = g.get_vertice_size();
	e = g.get_edge_size();

	cout << " n: " << n;
	cout << " nmax: " << nmax;
	cout << " nmin: " << nmin;

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

vector<Multilinear> Partition::identify_dense(Multilinear m, vector<Multilinear> L) {
	int _metis;
	int nlp, ne, s;
	int dsize, lsize, lksize;
	int terms, term_size;
	int a;
	int vsize, visize;
	int varsize;
	float theta = 0.0, theta_p;
	float b1 = 0.1, b2 = 1.25;
	vector<Multilinear> D, D_final;
	vector< vector<int> > v;
	vector<int> temp;
	vector<int> varnums;
	Multilinear multi;

	int p = pmin;

	for (int p = pmin; p <= pmax; ++p) {	
		sp = myceil(n, p);//nominal partition size
		rp = nmax/sp;//load imbalance
		cout << " pmin: " << pmin;
		cout << " pmax: " << pmax;
		cout << " p: " << p;
		cout << " sp: " << sp;
		cout << " nmax: " << nmax;
		cout << " rp: " << rp << endl;

		ncon = rp;//ncon can't be 0
		nparts = p;//nparts can't be 1

		if (ncon == 0 || nparts == 1) {
			cout << "skip iteration" << endl;
			continue;
		}

		cout << "start metis:" << endl;

		//both methods work
		_metis = METIS_PartGraphRecursive(&nvtxs, &ncon, xadj, adjncy, NULL, NULL, NULL, &nparts, NULL, NULL, NULL, &objval, part);
		//_metis = METIS_PartGraphKway(&nvtxs, &ncon, xadj, adjncy, NULL, NULL, NULL, &nparts, NULL, NULL, NULL, &objval, part);

		cout << "objval:" << objval;
		cout << " _metis:" << _metis << endl;
		cout << "partition: ";
		for (int i = 0; i < n; ++i) {
			cout << part[i] << " ";
		}
		cout << endl;

		//group the vertices into categories
		for (int i = 0; i < nparts; ++i) {
			for (int j = 0; j < n; ++j) {
				if (part[j] == i) {
					temp.push_back(j);
				}
			}

			v.push_back(temp);
			temp.clear();
		}

		vsize = v.size();
		cout << "v: ";
		for (int i = 0; i < vsize; ++i) {
			visize = v[i].size();

			for (int j = 0; j < visize; ++j) {
				cout << v[i][j] << " ";
			}
			cout << "/";
		}

		//construct multilinears according to each categories
		for (int i = 0; i < vsize; ++i) {
			multi = Multilinear(m, v[i], 1);
			if (!multi.isEmpty()) {
				D.push_back(multi);
			}
		}

		v.clear();

		dsize = D.size();
		nlp = dsize;

		cout << endl << "D.size(): " << dsize << endl;

		//if dsize = 0, theta will be infinite
		if (dsize == 0) {
			cout << "dsize = 0" << endl;
			continue;
		}

		ne = 0;
		lsize = L.size();

		//compute Ne
		for (int i = 0; i < dsize; ++i) {
			D[i].toString();

			terms = D[i].getsize();
			ne += terms;

			cout << " ne before: " << ne;

			//if term of D[i] is present in L, exclude it 
			if (lsize > 0) {
				for (int j = 0; j < terms; ++j) {
					term_size = D[i].get_term_size(j);

					for (int k = 0; k < lsize; ++k) {
						lksize = L[k].getsize();
						
						for (int x = 0; x < lksize; ++x) {
							a = 0;

							if (term_size == L[k].get_term_size(x)) {
								for (int y = 0; y < term_size; ++y) {
									//not the same term
									if (D[i].getvar(j, y) != L[k].getvar(x, y)) {
										a = 1;
										break;
									}
								}
							}
							//not the same term
							else {
								a = 1;
							}

							//same term
							if (a == 0) {
								ne--;
								break;
							}
						}

						if (a == 0) {
							break;
						}
					}
				}
			}
			cout << " ne after: " << ne;
		}
		cout << endl;

		s = 0;

		//compute s(δ):maximum number of variables in the multilinear functions in Dρ
		for (int i = 0; i < dsize; ++i) {
			varnums = D[i].get_varnums();
			varsize = varnums.size();
			if (varsize > s){
   	 			s = varsize;
   	 		}
		}
		cout << "s: " << s << endl;

		//compute gain factor Θρ
		theta_p = ne/(pow(nlp,b1)*pow(b2,s));
		cout << "theta_p: " << theta_p << endl;

		if (theta_p > theta) {
			theta = theta_p;
			D_final = D;
		}

		D.clear();
	}

	cout << "D_final size: " << D_final.size() << endl;

	return D_final;
}

Partition::~Partition() {
	delete[] xadj;
	delete[] adjncy;
	delete[] part;
}








