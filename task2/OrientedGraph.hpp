#ifndef OrientedGraph_hpp
#define OrientedGraph_hpp

#include <iostream>
#include <stack>
#include <algorithm>

#include "custom_types.hpp"
#include "SymbolicImage.hpp"


#define NOVISITED 0
#define DEFAULT_STKITEM false

using namespace std;


class OrientedGraph{
	//количество вершин
	size_t numVertices;
	//adjacency List или список смежных вершин 
	graph_t adjList;
	csc_t filtered_csc;
	// комоненты сильной связности/Components of strong communication
	csc_t csc;
	// список посещений 
	vector<size_t> disc;
	// нижния вершина, которую можно посетить
	vector<size_t> lowLink;
	// стек или порядок посещений 
	stack <size_t> stk;
	//bool stkItem[]
	vector<bool> stkItem;
	size_t min(size_t a, size_t b) {
		return (a < b) ? a : b;
	}
	size_t max(size_t a, size_t b){
		return (a > b) ? a : b;	
	}
	void findComponent(size_t vertex);
public:
	/*Tarjan's strongly connected components algorithm */
	csc_t findStrongConComponents();
	
	OrientedGraph(graph_t graph){
		adjList = graph;
		numVertices = graph.size();
		// вершины начинаются с 1, ветрок с 0
		disc.assign(numVertices+1, NOVISITED);
		lowLink.assign(numVertices+1, NOVISITED);
		stkItem.assign(numVertices+1, DEFAULT_STKITEM);
	}
	void printStrongConComponents();
	csc_t filterComponents();
	size_t findMaxRangComponent();
};

#endif /* OrientedGraph_hpp */