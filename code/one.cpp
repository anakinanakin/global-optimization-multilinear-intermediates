#include "one.h"

Relation::Relation(){
	j = 0;
}

void Relation::mpush(int j, int coef, int first, int second){
	Bilinear b(j, coef, first, second);
	m.push_back(b);
}

void Relation::ypush(int j, int coef, int x){
	Univariate u(j, coef, x);
	y.push_back(u);
}

void Relation::fpush(int coef, int x){
	Unifunction u(coef, x);
	f.push_back(u);
}

void Relation::mprint(){
	for(int i = 0; i < m.size(); ++i)
  		cout << m[i].toString();
}

void Relation::yprint(){
	for(int i = 0; i < y.size(); ++i)
  		cout << y[i].toString();
}

void Relation::fprint(){
	for(int i = 0; i < f.size(); ++i)
  		cout << f[i].toString();
}

int Relation::fgetsize(){
	return f.size();
}

int Relation::fgetcoef(int i){
	return f[i].coef;
}

int Relation::fgetval(int i){
	return f[i].x;
}

void Relation::addj(int add){
	j += add;
}

int Relation::getj(){
	return j;
}

string Unifunction::toString(){
	if (this->coef == 1){
		return "x" + to_string(this->x) + " ";		
	}
	else{
		return to_string(this->coef) + "x" + to_string(this->x) + " ";
	}
}

string Univariate::toString(){
	if (this->coef == 1){
		return "\ny" + to_string(this->j) + " = x" + to_string(this->x);
	}
	else{
		return "\ny" + to_string(this->j) + " = " + to_string(this->coef) + "x" + to_string(this->x);
	}
}

string Bilinear::toString(){
	if (this->second == -1){
		if (this->coef == 1){
			return "\ny" + to_string(this->j) + " = x" + to_string(this->first);
		}
		else{
			return "\ny" + to_string(this->j) + " = " + to_string(this->coef) + "x" + to_string(this->first);
		}
	}
	else{
		return "\ny" + to_string(this->j) + " = y" + to_string(this->first) + "y" + to_string(this->second);
	}
}




