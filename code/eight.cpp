#include "eight.h"

Integrate::Integrate(Multilinear m): rc(0.85) {
	//using algorithm 5, update nc and U

	cout << "\n--------------START 5---------------------" << endl;

	vector<int> v;

	Graph g(m);
	g.toString();

	Decompose d(g, m);

	vector<Multilinear> ccomponents;

	d.connected(L, ccomponents);

	d.biconnected(L, U, ccomponents);

	int usize = U.size();
	cout << "\nU.size: " << usize;
	for (int i = 0; i < usize; ++i) {
		U[i].toString();
	}

	nc = compute_covered(m, U, v);

	int vsize = v.size();
	cout << "\nv:";
	for (int i = 0; i < vsize; ++i) {
		cout << " " << v[i];
	}

	cout << " nc: " << nc << endl;
}

vector<Multilinear> Integrate::construct(Multilinear m) {
	int usize = U.size();

	//U should be nonempty to continue
	if (usize > 0) {
		int msize = m.getsize(), lisize;
		int ne;
		float a = 0.6;
		Multilinear Li, Lrd;
		vector<Multilinear> D, ccomponents;
		vector<int> v;

		while ( (nc >= rc*msize) && (usize > 0) ) {
		//while (nc >= msize) {
			//select the first multilinear
			Li = U[0];

			//using algorithm 6, update nc and U

			cout << "\n--------------START 6---------------------" << endl;

			Graph g(Li);
			g.toString();

			Partition p(g);

			D = p.identify_dense(Li, L);

			U.insert(U.end(), D.begin(), D.end());

			usize = U.size();
			cout << "\nU.size: " << usize;
			for (int i = 0; i < usize; ++i) {
				U[i].toString();
			}

			v.clear();
			nc = compute_covered(m, U, v);

			cout << " nc: " << nc << endl;

			v.clear();
			//let ne be the number of covered terms in Li
			ne = compute_covered(Li, U, v);

			cout << "ne: " << ne << endl;

			lisize = Li.getsize();

			cout << "lisize: " << lisize << endl;

			if ( (ne < rc * lisize) && (lisize > 0) ) {
				//construct Lrd using algorithm 7

				cout << "\n--------------START 7---------------------" << endl;

				Reduce re(Li, U);

				Lrd = re.remove(Li, a);

				Lrd.toString();
				cout << endl;

				//using algorithm 5, update nc and U

				cout << "\n--------------START 5-2---------------------" << endl;

				Graph g2(Lrd);
				g2.toString();

				Decompose d(g2, Lrd);

				ccomponents.clear();

				d.connected(L, ccomponents);

				d.biconnected(L, U, ccomponents);

				usize = U.size();
				cout << "\nU.size: " << usize;
				for (int i = 0; i < usize; ++i) {
					U[i].toString();
				}

				v.clear();
				nc = compute_covered(m, U, v);

				cout << " nc: " << nc << endl;
			}

			//erase the first multilinear
			U.erase(U.begin());

			usize = U.size();
		}
	}

	return U;
}












