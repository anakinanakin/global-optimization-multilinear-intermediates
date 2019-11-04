#include "five.h"

//if the number of variables in L(x) does not exceed nmin, 
//we do not attempt to break it down to smaller multilinears but store it for cut generation.
//use nmin in paper
Decompose::Decompose(Graph a, Multilinear b): nmin(4), g(a), m(b) {
	g.compute_connected_components();
	g.print_connected_components();

	csize = g.get_connected_components_size();
}

void Decompose::connected(vector<Multilinear>& L, vector<Multilinear>& ccomponents) {
	vector<int> v;
	Multilinear lt;

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
}

void Decompose::biconnected(vector<Multilinear>& L, vector<Multilinear>& U, vector<Multilinear>& ccomponents) {
	vector<struct Edge> vec_edge;
	int bcsize;
	int ntk;
	Multilinear ltk;

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
}











