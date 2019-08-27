#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Relation
{
	public:
		Relation();

		void mpush(int j, int coef, int first, int second);
		void ypush(int j, int coef, int x);
		void fpush(int coef, int x);

		void mprint();
		void yprint();
		void fprint();

		int fgetsize();
		int fgetcoef(int i);
		int fgetval(int i);

		void addj(int add);
		int getj();

	private:
		int j;

		vector<struct Bilinear> m; 
	    vector<struct Univariate> y;
		vector<struct Unifunction> f;
};

//add gk(x) to f
struct Unifunction
{
	int coef;
	int x;
	
	Unifunction(int a, int b){
		coef = a;
		x = b;
	}

	string toString();
};

//yj = gk(x)
struct Univariate
{
	int j;
	int coef;
	int x;
	
	Univariate(int a, int b, int c){
		j = a;
		coef = b;
		x = c;
	}

	string toString();
};

//bilinear relation (if second = -1 and coef != 0, it is for yj = fi(x))
struct Bilinear
{
	int j;
	int coef;
	int first;
	int second;

	Bilinear(int a, int b, int c, int d){
		j = a;
		coef = b;
		first = c;
		second = d;
	}

	string toString();
};



