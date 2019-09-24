#include "multilinear.h"

//the whole function is a vector, each element is a vector representing each term with integers(coef, var1, var2....)
Multilinear::Multilinear(){
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

		func.push_back(v);//Floating point exception while using METIS_PartGraphKway
		v.clear();
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
	cout << "multilinear function:";

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






