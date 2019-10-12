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
}

//construct a multilinear by collecting all terms of multilinear m with each of vertex in arr, used for connected components
Multilinear::Multilinear(Multilinear m, vector<int> v){
	//for breaking the loop
	int a = 0;

	for (int i = 0; i < m.func.size(); ++i) {
		//ignore coef
		for (int j = 1; j < m.func[i].size(); ++j) {
			for (int k = 0; k < v.size(); ++k) {
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
Multilinear::Multilinear(Multilinear m, vector<struct Edge> v) {
	int ctr = 0;
	//for breaking the loop
	int a = 0;
	//for continue to next iteration
	int b = 0;
	vector<int> same_num;

	for (int i = 0; i < m.func.size(); ++i) {
		same_num.clear();
		
		for (int k = 0; k < v.size(); ++k) {
			ctr = 0;
			//ignore coef
			for (int j = 1; j < m.func[i].size(); ++j) {
				//if same variable is found, continue to next iteration
				for (int l = 0; l < same_num.size(); ++l) {
					if (same_num[l] == m.func[i][j]) {
						b = 1;
						break;
					}
				}

				if (b == 1) {
					b = 0;
					continue;
				}

				//need to find two vertices of one edge both matches variables of one term
				if (m.func[i][j] == v[k].v || m.func[i][j] == v[k].i) {
					ctr++;

					//record the variables found
					same_num.push_back(m.func[i][j]);

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

//get the number of terms of the function
int Multilinear::getsize(){
	return func.size();
}

//get the number of variables in a specific term
int Multilinear::get_term_size(int term){
	return func[term].size();
}

//get the specific variable
int Multilinear::getvar(int term, int var){
	return func[term][var];
}

void Multilinear::toString(){
	cout << "\nmultilinear function:";

	for(int i = 0; i < this->func.size(); ++i){
		for(int j = 0; j < this->func[i].size(); ++j){
			//don't print coef 1
			if (j != 0 || this->func[i][j] != 1){
				cout << this->func[i][j];
			}
				
  			if (j != this->func[i].size()-1){
  				cout << "x";
  			}
  		}

  		if (i != this->func.size()-1){
  			cout << "+";
  		}
	}
}

//return the distinct variables used, in increasing order
vector <int> Multilinear::get_varnums(){
	for(int i = 0; i < this->func.size(); ++i){
		for(int j = 1; j < this->func[i].size(); ++j){
			if (find(this->varnums.begin(), this->varnums.end(), func[i][j]) == this->varnums.end()){
				this->varnums.push_back(func[i][j]);
			}
		}
	}
	sort(this->varnums.begin(), this->varnums.end());

	//for (int i = 0; i < this->varnums.size(); ++i){
	//	cout << this->varnums[i];
	//}

	return this->varnums;
}






