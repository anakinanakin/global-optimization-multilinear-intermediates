#pragma once

#include "graph.h"

using namespace std;

class Decompose
{
	public:
		Decompose(Graph a, Multilinear b);

		void connected(vector<Multilinear>& L, vector<Multilinear>& ccomponents);
		void biconnected(vector<Multilinear>& L, vector<Multilinear>& U, vector<Multilinear>& ccomponents);

	private:
		int nmin;
		int csize;

		Graph g;
		Multilinear m;
};


