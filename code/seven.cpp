#include "seven.h"

Reduce::Reduce(Multilinear m, vector<Multilinear>& L) {
	ncv = compute_covered(m, L, u);

	int usize = u.size();
	cout << "\nu:";
	for (int i = 0; i < usize; ++i) {
		cout << " " << u[i];
	}
	cout << " ncv: " << ncv << endl;

	int var;
	int term_size, cover_size;

	if (ncv > 0) {
		//compute covered variables
		covered_var = candi_covered_var(m);

		//if in u=0 terms exist a variable, exclude it
		for (int i = 0; i < usize; ++i) {
			term_size = m.get_term_size(i);

			if (u[i] == 0) {
				for (int j = 1; j < term_size; ++j) {
					var = m.getvar(i, j);
					//if covered_var contains var
					if (find(covered_var.begin(), covered_var.end(), var) != covered_var.end()) {
						covered_var.erase(find(covered_var.begin(), covered_var.end(), var));
					}
				}
			}
		}

		cover_size = covered_var.size();
		cout << "covered_var:";
		for (int i = 0; i < cover_size; ++i) {
			cout << " " << covered_var[i];
		}
	}
}

//removing all terms of L(x) in which at least one covered variable appears
Multilinear Reduce::remove(Multilinear m, float a) {
	int x, term_size, cover_size = covered_var.size();

	//m changes during for loop, can't replace m.getsize()
	for (int i = 0; i < m.getsize(); ++i) {
		x = 0;
		term_size = m.get_term_size(i);

		for (int j = 1; j < term_size; ++j) {
			for (int k = 0; k < cover_size; ++k) {
				if (m.getvar(i, j) == covered_var[k]) {
					x = 1;

					m.remove_term(i);
					u.erase(u.begin() + i);

					//to examine the correct term
					--i;

					break;
				}
			}

			if (x == 1) {
				break;
			}
		}
	}

	//todo:
	//Remove from Lrd the nrs terms with largest μ values. 
	//In case of a tie, remove terms with a minimal number of common variables.
	//common variables not implemented

	/*int nrs = m.getsize() - a*ncv;
	vector<int>::iterator max, position;

	if (nrs > 0) {
		for (int i = 0; i < nrs; ++i) {
			max = max_element(u.begin(), u.end());
			position = find(u.begin(), u.end(), max);
			m.remove_term(position - u.begin());
			u.erase(position);
		}
	}*/

	return m;
}

//collect variables that appears in u>0 terms
vector<int> Reduce::candi_covered_var(Multilinear m) {
	vector<int> v;
	int var;
	int usize = u.size(), term_size;

	for (int i = 0; i < usize; ++i) {
		term_size = m.get_term_size(i);

		if (u[i] > 0) {
			for (int j = 1; j < term_size; ++j) {
				var = m.getvar(i, j);
				//if v doesn't contain var
				if (find(v.begin(), v.end(), var) == v.end()) {
					v.push_back(var);
				}
			}
		}
	}

	sort(v.begin(), v.end());

	int vsize = v.size();
	cout << "candi:";
	for (int i = 0; i < vsize; ++i) {
		cout << " " << v[i];
	}
	cout << endl;

	return v;
}

//compute u, ncv
int compute_covered(Multilinear m, vector<Multilinear>& v, vector<int>& u) {
	int size = m.getsize(), vsize = v.size(), term_size, lksize;
	int ncv = 0, a;

	if (vsize > 0) {
		//compute u, ncv
		for (int i = 0; i < size; ++i) {
			u.push_back(0);

			term_size = m.get_term_size(i);

			for (int k = 0; k < vsize; ++k) {
				lksize = v[k].getsize();

				for (int x = 0; x < lksize; ++x) {
					a = 0;

					if (term_size == v[k].get_term_size(x)) {
						for (int j = 0; j < term_size; ++j) {
							//not the same term
							if (m.getvar(i, j) != v[k].getvar(x, j)) {
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
						u[i]++;
						break;
					}
				}
			}

			if (u[i] > 0) {
				ncv++;
			}	
		}
	}

	return ncv;
}









