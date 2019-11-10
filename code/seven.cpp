#include "seven.h"

Reduce::Reduce(Multilinear m, vector<Multilinear>& L): ncv(0) {
	int lsize = L.size();
	int term_size;
	int a;

	if (lsize > 0) {
		//compute u, ncv
		for (int i = 0; i < m.getsize(); ++i) {
			u.push_back(0);
			term_size = m.get_term_size(i);
			for (int k = 0; k < lsize; ++k) {
				for (int x = 0; x < L[k].getsize(); ++x) {
					a = 0;

					if (term_size == L[k].get_term_size(x)) {
						for (int j = 0; j < term_size; ++j) {
							//not the same term
							if (m.getvar(i, j) != L[k].getvar(x, j)) {
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

	cout << "\nu:";
	for (int i = 0; i < u.size(); ++i) {
		cout << " " << u[i];
	}
	cout << " ncv: " << ncv << endl;

	int var;

	if (ncv > 0) {
		//compute covered variables
		covered_var = candi_covered_var(m);

		//if in u=0 terms exist a variable, exclude it
		for (int i = 0; i < u.size(); ++i) {
			if (u[i] == 0) {
				for (int j = 1; j < m.get_term_size(i); ++j) {
					var = m.getvar(i, j);
					//if covered_var contains var
					if (find(covered_var.begin(), covered_var.end(), var) != covered_var.end()) {
						covered_var.erase(find(covered_var.begin(), covered_var.end(), var));
					}
				}
			}
		}

		cout << "covered_var:";
		for (int i = 0; i < covered_var.size(); ++i) {
			cout << " " << covered_var[i];
		}
	}
}

//removing all terms of L(x) in which at least one covered variable appears
Multilinear Reduce::remove(Multilinear m, float a) {
	int x;

	for (int i = 0; i < m.getsize(); ++i) {
		x = 0;

		for (int j = 1; j < m.get_term_size(i); ++j) {
			for (int k = 0; k < covered_var.size(); ++k) {
				if (m.getvar(i, j) == covered_var[k]) {
					x = 1;

					m.remove_term(i);
					u.erase(u.begin() + i);

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

	for (int i = 0; i < u.size(); ++i) {
		if (u[i] > 0) {
			for (int j = 1; j < m.get_term_size(i); ++j) {
				var = m.getvar(i, j);
				//if v doesn't contain var
				if (find(v.begin(), v.end(), var) == v.end()) {
					v.push_back(var);
				}
			}
		}
	}

	sort(v.begin(), v.end());

	cout << "candi:";
	for (int i = 0; i < v.size(); ++i) {
		cout << " " << v[i];
	}
	cout << endl;

	return v;
}









