#include "multilinear.h"

//default constructor
Multilinear::Multilinear() {}

//the whole function is a vector, each element is a vector representing each term with integers(coef, var1, var2....)
//a is only for indicating this constructor
Multilinear::Multilinear(int a){
	int terms, coef, vars, tempVar;
	vector<int> v;

	//user input function
	cout << "input number of terms:";
	cin >> terms;

	for (int i = 1; i <= terms; ++i){
		cout << "input multilinear function:\n" << "The number " << i << " term:\n"<< "coefficient:";

		cin >> coef;
		v.push_back(coef);

		cout << "number of variables:";
		cin >> vars;

		for (int j = 0; j < vars; ++j){
			cout << "X";
			cin >> tempVar;
			v.push_back(tempVar);
		}

		func.push_back(v);
		v.clear();
	}

	/*int nk, nmax = NMAX;
	int size = func.size();

	//dimension not reduced, causing infinite loop
	//replace each term tk with nk > nmax by dk multilinear terms
	for (int i = 0; i < size; ++i) {
		cout << "d3";
		//number of variables in term i
		nk = func[i].size() - 1;
		cout << "nk: " << nk;
		cout << "nmax: " << nmax;
		if (nk > nmax) {
			reduce_dimension(i, nk, nmax);

			//to examine the correct term
			--i;
		}
	}*/
}

//construct a multilinear by collecting all terms of multilinear m with each of vertex in arr, used for connected components
Multilinear::Multilinear(Multilinear m, const vector<int>& v){
	//for breaking the loop
	int a = 0, msize = m.func.size(), mfuncsize, vsize = v.size();

	for (int i = 0; i < msize; ++i) {
		mfuncsize = m.func[i].size();

		//ignore coef
		for (int j = 1; j < mfuncsize; ++j) {
			for (int k = 0; k < vsize; ++k) {
				//if found same vertex, push it to the new multilinear
				if (m.func[i][j] == v[k]){
					func.push_back(m.func[i]);

					a = 1;
					break;
				}
			}

			if (a == 1) {
				break;
			}
		}

		a = 0;
	}
}

//for biconnected components
Multilinear::Multilinear(Multilinear m, const vector<struct Edge>& v) {
	int ctr = 0;
	//for breaking the loop
	int a = 0;
	//for continue to next iteration
	//int b = 0;
	//vector<int> same_num;
	int msize = m.func.size(), mfuncsize, vsize = v.size();

	for (int i = 0; i < msize; ++i) {
		//same_num.clear();
		mfuncsize = m.func[i].size();

		for (int k = 0; k < vsize; ++k) {
			ctr = 0;
			//ignore coef
			for (int j = 1; j < mfuncsize; ++j) {
				//if same variable is found, continue to next iteration
				/*for (int l = 0; l < same_num.size(); ++l) {
					if (same_num[l] == m.func[i][j]) {
						b = 1;
						break;
					}
				}

				if (b == 1) {
					b = 0;
					continue;
				}*/

				//need to find two vertices of one edge both matches variables of one term
				if (m.func[i][j] == v[k].v || m.func[i][j] == v[k].i) {
					ctr++;

					//record the variables found
					//same_num.push_back(m.func[i][j]);

					if (ctr == 2) {
						func.push_back(m.func[i]);
						ctr = 0;
						a = 1;
						break;
					}
				}
			}

			if (a == 1) {
				break;
			}
		}

		a = 0;
	}
}

//for metis partitioning result
//if a term only consist of vertices in v, then include the term
//a is only for indicating this constructor
Multilinear::Multilinear(Multilinear m, const vector<int>& v, int a) {
	int discard = 0;
	int msize = m.func.size(), mfuncsize, vsize = v.size();
	//convert v to varnums for m
	vector<int> varnums_m = m.get_varnums();

	for (int i = 0; i < msize; ++i) {
		mfuncsize = m.func[i].size();

		//func only can use elements from v, so can't be larger than v
		if ((m.func[i].size()-1) > v.size()) {
			continue;
		}

		for (int j = 1; j < mfuncsize; ++j) {
			for (int k = 0; k < vsize; ++k) {
				//found element, test next variable
				if (m.func[i][j] == varnums_m[v[k]]) {
					discard = 0;
					break;
				}

				//not found element, discard term
				discard = 1;
			}

			//discard term
			if (discard == 1) {
				break;
			}
		}

		if (discard == 0) {
			func.push_back(m.func[i]);
		}
	}
}

//get the number of terms of the function
int Multilinear::getsize() {
	return func.size();
}

//get the number of variables in a specific term
int Multilinear::get_term_size(int term) {
	return func[term].size();
}

//get the specific variable
int Multilinear::getvar(int term, int var) {
	return func[term][var];
}

//remove specific term
void Multilinear::remove_term(int term) {
	func.erase(func.begin() + term);
}

void Multilinear::toString() {
	int size = func.size(), funcsize;
	cout << "\nmultilinear function:";

	for(int i = 0; i < size; ++i){
		funcsize = func[i].size();

		for(int j = 0; j < funcsize; ++j){
			//don't print coef 1
			if (j != 0 || func[i][j] != 1){
				cout << func[i][j];
			}
				
  			if (j != func[i].size()-1){
  				cout << "x";
  			}
  		}

  		if (i != func.size()-1){
  			cout << "+";
  		}
	}
}

//return the distinct variables used, in increasing order
vector <int> Multilinear::get_varnums() {
	int size = func.size(), funcsize;

	for(int i = 0; i < size; ++i) {
		funcsize = func[i].size();

		for(int j = 1; j < funcsize; ++j) {
			if (find(varnums.begin(), varnums.end(), func[i][j]) == varnums.end()){
				varnums.push_back(func[i][j]);
			}
		}
	}

	sort(varnums.begin(), varnums.end());

	return varnums;
}

bool Multilinear::repeated_var() {
	int size = func.size(), funcsize;

	for(int i = 0; i < size; ++i) {
		funcsize = func[i].size();
		
		for(int j = 1; j < funcsize; ++j) {
			for(int k = j+1; k < funcsize; ++k) {
				//found repeated var
				if (func[i][j] == func[i][k]) {
					return true;
				}
			}
		}
	}

	return false;
}

bool Multilinear::isEmpty() {
	if (func.size() == 0) {
		return true;
	}
	else {
		return false;
	}
}

//todo: implement term t1,....,ts+1
//replace each term with nk > nmax by dk multilinear terms
/*void  Multilinear::reduce_dimension(int term, int nk, int nmax) {
	int dk = 1 + myceil(nk - nmax, nmax - 1);
	vector<int> v;
	int ls, us;

	//t1
	for (int i = 0; i <= nmax; ++i) {
		v.push_back(func[term][i]);
		cout << "d1";
	}
	func.push_back(v);

	//t(s+1)
	for (int s = 1; s < dk; ++s) {
		ls = s * (nmax - 1) + 2;
		us = min(s * (nmax - 1) + nmax, nk);
		cout << "ls: " << ls; 
		cout << "us: " << us; 

		for (int i = ls; i <= us; ++i) {
		 	v.push_back(func[term][i]);
			cout << i;
		}

		func.push_back(v);
		cout << "d2";
	}

	func.erase(func.begin() + term);
}*/

int myceil(int numerator, int denominator) {
	if (numerator == denominator){
		return floor(numerator/denominator);
	}
	else{
		return floor(numerator/denominator)+1;
	}
}

int min(int a, int b) {
	if (a > b) {
		return b;
	}
	else {
		return a;
	}
}



























