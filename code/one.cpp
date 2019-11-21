#include "one.h"

Relation::Relation():j(0) {
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
	int size = m.size();

	for(int i = 0; i < size; ++i)
  		cout << m[i].toString();
}

void Relation::yprint(){
	int size = y.size();

	for(int i = 0; i < size; ++i)
  		cout << y[i].toString();
}

void Relation::fprint(){
	int size = f.size();

	for(int i = 0; i < size; ++i)
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
	if (coef == 1){
		return "x" + to_string(x) + " ";		
	}
	else{
		return to_string(coef) + "x" + to_string(x) + " ";
	}
}

string Univariate::toString(){
	if (coef == 1){
		return "y" + to_string(j) + " = x" + to_string(x);
	}
	else{
		return "y" + to_string(j) + " = " + to_string(coef) + "x" + to_string(x);
	}
}

string Bilinear::toString(){
	if (second == -1){
		if (coef == 1){
			return "y" + to_string(j) + " = x" + to_string(first);
		}
		else{
			return "y" + to_string(j) + " = " + to_string(coef) + "x" + to_string(first);
		}
	}
	else{
		return "y" + to_string(j) + " = y" + to_string(first) + "y" + to_string(second);
	}
}




