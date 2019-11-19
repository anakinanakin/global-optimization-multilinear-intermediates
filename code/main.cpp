//g++ main.cpp one.cpp graph.cpp multilinear.cpp five.cpp six.cpp seven.cpp eight.cpp -lmetis

#include "multilinear.h"
#include "one.h"
#include "graph.h"
#include "five.h"
#include "six.h"
#include "seven.h"
#include "eight.h"

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

	//algorithm 1

	Relation r;
	int msize = m.getsize();
	int l, term_size, coef, x;

	//multilinear function
	for (int i = 0; i < msize; ++i){
		l = m.get_term_size(i);//get term size plus 1(coef)
		term_size = m.get_term_size(i) - 1;//get term size
		coef = m.getvar(i, 0); //get term coefficient
		x = m.getvar(i, 1); //get first x var

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

	//algorithm 5 

	Graph g(m);
	g.toString();

	Decompose d(g, m);

	vector<Multilinear> L;//L for cut generation
	vector<Multilinear> ccomponents;
	vector<Multilinear> U;//uncovered multilinears U for further decomposition

	d.connected(L, ccomponents);

	d.biconnected(L, U, ccomponents);

	cout << "\nL.size: " << L.size() << endl;
	cout << "\nU.size: " << U.size() << endl;


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

	int lsize = L.size();
	cout << "L.size: " << lsize;	
	for (int i = 0; i < lsize; ++i) {
		L[i].toString();
	}

	//algorithm 7

	Multilinear lrd;
	//using paper parameters
	float a = 0.6;

	Reduce re(m, L);

	lrd = re.remove(m, a);

	lrd.toString();
	cout << endl;

	L.push_back(lrd);

	//algorithm 8

	vector<Multilinear> UU;

	Integrate in(m);

	UU = in.construct(m);

	int uusize = UU.size();
	cout << "UU.size: " << uusize;
	for (int i = 0; i < uusize; ++i) {
		UU[i].toString();
	}
	cout << endl;

	return 0;
}








