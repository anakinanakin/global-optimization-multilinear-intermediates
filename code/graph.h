#pragma once

#include <iostream>
#include <vector>
#include <list> 
#include <stack> 
#include <unordered_set>

#include "multilinear.h"

using namespace std;

class Graph
{
	public:
		Graph();
		Graph(Multilinear m);

		void compute_connected_components();
		void print_connected_components();
		//int* get_connected_components_num();
		vector<int> get_connected_components_vertices(int i);
		int get_connected_components_size();
		int get_component_size(int i);
		int get_vertice_size();
		int get_edge_size();
		bool three_edge_component(int i);

		void compute_biconnected_components();
		vector<struct Edge> get_biconnected_components_edges(int i);
		int get_biconnected_components_size();
		int get_bicomponent_size(int i);

		void toString();

		int* get_csr_vertex();
		int* get_csr_edge();

		//~Graph();
	
	private:
		int** adjMatrix;
        int numVertices;
        int numEdges;//twice of usual edges, because (u,v) and (v,u) count twice
        int bi_components_num;

        //variable nums(x1,x2....) correspond to the vertices(0,1,2...)
    	vector<int> varnums;

    	//get the connected vertices (ex: [1,4,3],[2,6],[5,7,8]...)
    	vector< vector<int> > connected_components;

    	//get the biconnected components
    	vector< vector<struct Edge> > bi_connected_components;

    	int find_vector_index(int element);
    	void addEdge(int i, int j, int coef);
		void removeEdge(int i, int j);
		int edgeNum(int i, int j);

		void DFS(int i, bool visited[], vector<int> &v);
		void BCC(int u, int disc[], int low[], list<struct Edge>* st, int parent[]);

		int countDistinct(const vector<int> &v);
}; 




