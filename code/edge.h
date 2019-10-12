#pragma once

//edge for representing biconnected components
struct Edge 
{  
    int v; 
    int i; 
    Edge(int a, int b){
    	v = a;
    	i = b;
    }
};