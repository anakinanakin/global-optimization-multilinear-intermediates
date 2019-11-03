//g++ main.cpp one.cpp five.cpp multilinear.cpp six.cpp -lmetis

#include "multilinear.h"
#include "one.h"
#include "five.h"
#include "six.h"

using namespace std;

int main(int argc, char *argv[])
{
	Multilinear m(1);
	m.toString();

	//repeated variable not acceptable, error will occur
	if (m.repeated_var()) {
		cout << "\nrepeated variable found!!\n";
		return -1;
	}

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

	g.compute_connected_components();
	g.print_connected_components();

	int csize = g.get_connected_components_size();

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

	cout << "L.size = " << L.size() << endl;

	//algorithm 6

	//input the set of parameters {β1, β2, nmin, nmax}.
	//cout << "input B1 for graph partitioning(0 < B1 ≤ 1): ";
	//cin >> B1;
	//cout << "input B2 for graph partitioning(1 ≤ B2): ";
	//cin >> B2;
	//cout << "input nmax for graph partitioning(nmax >= " << nmin << "): ";
	//cin >> nmax;
	
	Partition p(g);

	vector<Multilinear> D;

	D = p.identify_dense(m, L);

	//store the set of dense components D for cut generation
	L.insert(L.end(), D.begin(), D.end());

	cout << "L.size: " << L.size() << endl;

	return 0;
}








