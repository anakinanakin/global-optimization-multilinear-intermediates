#pragma once

#include "five.h"
#include "six.h"
#include "seven.h"

using namespace std;

class Integrate
{
	public:
		Integrate(Multilinear m);

		vector<Multilinear> construct(Multilinear m);

	private:
		float rc;//controls the strength of the relaxation, 0<rc<1
		int nc;//number of covered terms
		vector<Multilinear> U;//list of uncovered functions
		vector<Multilinear> L;//cut generation list
};