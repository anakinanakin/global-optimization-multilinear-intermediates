#include "five.h"

#define NIL -1 

Graph::Graph(Multilinear m) {
    //initialize private variables
    numEdges = 0;
    bi_components_num = 0;
    varnums = m.get_varnums();
    numVertices = varnums.size();

    adjMatrix = new int*[numVertices];

    //initialize the matrix with 0s
    for (int i = 0; i < numVertices; ++i) {
        adjMatrix[i] = new int[numVertices];
        for (int j = 0; j < numVertices; ++j){
            adjMatrix[i][j] = 0;
        }
    }

    int term_size, coef, fixvar = 0;
    int size = m.getsize();

    //add the edges
    for (int i = 0; i < size; ++i){
        term_size = m.get_term_size(i);

        //term with only one variable cannot add edge
        if (term_size == 1){
            cout << "univariable found";
        }
        else{
            //add edges coef times
            coef = m.getvar(i, 0);

            //negative coef add the same edges
            if (coef < 0){
                coef *= -1;
            }

            //add edges
            for (int j = 1; j < term_size; ++j){
                fixvar = m.getvar(i, j);

                for (int k = 1; k < term_size; ++k){
                    if (j != k){
                        if (adjMatrix[find_vector_index(fixvar)][find_vector_index(m.getvar(i, k))] == 0){
                            numEdges++;//twice of usual edges, because (u,v) and (v,u) count twice
                        }

                        addEdge(find_vector_index(fixvar), find_vector_index(m.getvar(i, k)), coef);
                    }
                }
            }
        }
    }
}

int Graph::find_vector_index(int element){
    vector<int>::iterator it = find(varnums.begin(), varnums.end(), element);

    if (it != varnums.cend()){
        return distance(varnums.begin(), it);
    }
    else{
        cout << "Element not found";
        return 0;
    }
}
 
void Graph::addEdge(int i, int j, int coef) {
    adjMatrix[i][j] += coef;
    //adjMatrix[j][i] += coef; will iterate with opposite order in for loop above, so no need
}
 
void Graph::removeEdge(int i, int j) {
    adjMatrix[i][j]--;
    adjMatrix[j][i]--;
}
 
int Graph::edgeNum(int i, int j) {
    return adjMatrix[i][j];
}

void Graph::DFS(int i, bool visited[], vector<int> &v){
    // Mark the current node as visited and store it 
    visited[i] = true; 
    v.push_back(i);
  
    // Recur for all the vertices adjacent to this vertex 
    for(int j = 0; j < numVertices; ++j) 
        if(!visited[j] && adjMatrix[i][j] > 0) 
            DFS(j, visited, v); 
}

// A recursive function that finds and prints strongly connected components using DFS traversal 
// v --> The vertex to be visited next 
// disc[] --> Stores discovery times of visited vertices 
// low[] --> earliest visited vertex (the vertex with minimum discovery time) 
// that can be reached from subtree rooted with current vertex 
// *st --> To store visited edges
void Graph::BCC(int v, int disc[], int low[], list<struct Edge>* st, int parent[]){
    // A static variable is used for simplicity, we can avoid use of static variable by passing a pointer
    static int time = 0; 
  
    // Initialize discovery time and low value 
    disc[v] = low[v] = ++time; 
    int children = 0; 
    cout << "d12";
  
    // Recur for all the vertices adjacent to this vertex 
    for (int i = 0; i < numVertices; ++i) { 
        if (adjMatrix[v][i] > 0){
            // If i is not visited yet, then recur for it 
            if (disc[i] == -1) { 
                children++; 
                parent[i] = v; 

                // store the edge in stack 
                st->push_back(Edge(v, i)); 
                BCC(i, disc, low, st, parent); 
  
                // Check if the subtree rooted with 'i' has a connection to one of the ancestors of 'v' 
                // Case 1 -- per Strongly Connected Components Article 
                low[v] = min(low[v], low[i]); 
  
                // If v is an articulation point, pop all edges from stack till v -- i 
                if ((disc[v] == 1 && children > 1) || (disc[v] > 1 && low[i] >= disc[v])) { 
                    while (st->back().v != v || st->back().i != i) { 
                        cout << varnums[st->back().v] << "--" << varnums[st->back().i] << " "; 
                        st->pop_back(); 
                        cout << "d10";
                    } 

                    cout << varnums[st->back().v] << "--" << varnums[st->back().i]; 
                    cout << "d9";
                    st->pop_back(); 
                    cout << "d11";
                    bi_components_num++; 
                } 
            } 

            // Update low value of 'v' only of 'i' is still in stack (i.e. it's a back edge, not cross edge). 
            // Case 2 -- per Strongly Connected Components Article 
            else if (i != parent[v]) { 
                low[v] = min(low[v], disc[i]); 
                if (disc[i] < disc[v]) { 
                    st->push_back(Edge(v, i)); 
                } 
            } 
        }
    } 
}

void Graph::compute_connected_components(){ 
    // Mark all the vertices as not visited 
    bool *visited = new bool[numVertices]; 
    vector<int> v;

    for(int i = 0; i < numVertices; ++i){
        visited[i] = false; 
    }
  
    for (int i = 0; i < numVertices; ++i){ 
        if (visited[i] == false){   
            //pass in a vector and store the vertices of the same component, then store the vector
            DFS(i, visited, v);
            connected_components.push_back(v);
            v.clear();
        } 
    } 
} 

void Graph::print_connected_components(){
    cout << "connected components:";
    for(int i = 0; i < connected_components.size(); ++i){
        cout << "vertices of component " << i+1 << ": ";
        for(int j = 0; j < connected_components[i].size(); ++j){
            cout << varnums[connected_components[i][j]] << " ";
            cout << "s3";
        }
        cout << "s2";
    }
    cout << "s1";
    //cout << "\n";
}

int Graph::get_connected_components_size(){
    return connected_components.size();
}

int Graph::get_component_size(int i){
    return connected_components[i].size();
}

int Graph::get_vertice_size(){
    return numVertices;
}

int Graph::get_edge_size(){
    return numEdges;
}

// see if a 3-vertices connected component has exactly 3 edges
bool Graph::three_edge_component(int i){
    int a = find_vector_index(connected_components[i][0]);
    int b = find_vector_index(connected_components[i][1]);
    int c = find_vector_index(connected_components[i][2]);

    if (adjMatrix[a][b] > 0 && adjMatrix[a][c] > 0 && adjMatrix[b][c] > 0){
        return true;
    }
    else{
        return false;
    }
}

// The function to do DFS traversal. It uses BCC() 
void Graph::compute_biconnected_components(){
    cout << "biconnected components:";

    int* disc = new int[numVertices]; 
    int* low = new int[numVertices]; 
    int* parent = new int[numVertices];
    list<struct Edge>* st = new list<struct Edge>[numVertices*(numVertices-1)]; 
  
    // Initialize disc and low, and parent arrays 
    for (int i = 0; i < numVertices; i++) { 
        disc[i] = NIL; 
        low[i] = NIL; 
        parent[i] = NIL; 
    } 
    cout << "d3";
  
    for (int i = 0; i < numVertices; i++) { 
        if (disc[i] == NIL) 
            BCC(i, disc, low, st, parent); 
  
        int j = 0; 

        // If stack is not empty, pop all edges from stack 
        while (st->size() > 0) { 
            j = 1; 
            cout << "d7";
            cout << varnums[st->back().v] << "--" << varnums[st->back().i] << " ";  
            cout << "d5";
            st->pop_back(); 
            cout << "d6";
        } 
        cout << "d4";

        if (j == 1) { 
            cout << "d8";
            bi_components_num++; 
        } 
    } 
    cout << "d2";
}

void Graph::toString() {
    cout << "Graph matrix:   ";
    for (int i = 0; i < numVertices; ++i){
        cout << varnums[i] <<" ";
    }
    //cout << "\n";
    for (int i = 0; i < numVertices; ++i){
        cout << varnums[i] << "  ";
        for (int j = 0; j < numVertices; ++j){
            cout << adjMatrix[i][j] << " ";
        }
        //cout << "\n";
    }
}

//for metis xadj
int* Graph::get_csr_vertex(){
    int ctr = 0, idx = 0;
    int *arr = new int[numVertices+1];
    arr[idx] = ctr;

    for (int i = 0; i < numVertices; ++i){
        for (int j = 0; j < numVertices; ++j){
            if (adjMatrix[i][j] != 0){
                ctr++;
            }
        }
        idx++;
        arr[idx] = ctr;
    }

    return arr;
}

//for metis adjncy
int* Graph::get_csr_edge(){
    int *arr = new int[numEdges];
    int idx = 0;

    for (int i = 0; i < numVertices; ++i){
        for (int j = 0; j < numVertices; ++j){
            if (adjMatrix[i][j] != 0){
                arr[idx] = j;
                idx++;
            }
        }
    }

    return arr;
}

//void Graph::set_varnums(std::vector<int> v){
    //this->varnums = v;

    //for (int i = 0; i < this->varnums.size(); ++i){
    //    std::cout << this->varnums[i];
    //}
//}
 
Graph::~Graph() {
    for (int i = 0; i < numVertices; ++i){
        delete[] adjMatrix[i];
    }
    delete[] adjMatrix;
};



