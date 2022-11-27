#include "../main/Manager.cpp"
#include "../main/SymbolicImage.cpp"
#include "../main/OrientedGraph.cpp"
#include "../main/mathFunction.hpp"
//#define TESTFUNC JuliaFunction
#define NUM_CELLS_P 1
#define NPOINTS_P 2
#define TESTFUNC_P 3
#define NODE 8
graph_t simple_graph;

int graph_matrix[NODE][NODE] = {
   {0, 0, 0, 1, 1, 0, 0, 0},//1
   {0, 0, 0, 0, 0, 0, 0, 0},//2
   {1, 0, 0, 0, 0, 0, 0, 0},//3
   {0, 1, 1, 0, 0, 0, 0, 0},//4
   {0, 0, 0, 0, 0, 0, 0, 0},//5
   {1, 0, 0, 0, 0, 0, 0, 0},//6
   {0, 0, 0, 0, 0, 0, 0, 1},//7
   {0, 0, 0, 0, 0, 0, 0, 0}//8
};
//using graph_t = map<size_t, set<size_t> >;
graph_t makeSimpleGraph(){
	graph_t test;
	for (size_t i = 0; i < NODE; i++){
		set<size_t> v;
		/*
		for(size_t j = 0; j < NODE; j++)
			if (graph_matrix[i][j] == 1)
				v.insert(j+1);
		test.insert({i+1, v});
		*/
		for(size_t j = 1; j < NODE+1; j++)
			if (graph_matrix[i][NODE-j] == 1)
				v.insert(NODE-j+1);
		test.insert({i+1, v});
	}
	return test;
}

int main(int argc, char const *argv[])
{
	
	set<string> axes = {"x","y"};
	options_t boundaries;
	named_map_t bound_x = { {"begin", A},
			{"end", B}
		};
	named_map_t bound_y = { {"begin", C},
			{"end", D}
		};	
	boundaries.insert({"x", bound_x});
	boundaries.insert({"y", bound_y});
	named_map_t num_cells;
	named_map_t cell_size;
	if ( argc > 3 ){
		num_cells.insert({"x", stoi(argv[1]) });
		num_cells.insert({"y", stoi(argv[2]) });

	}
	else {
		num_cells.insert({"x", NUMCELLS});
		num_cells.insert({"y",  NUMCELLS});		
	}

	size_t num_points = argc > NPOINTS_P ? stoi(argv[NPOINTS_P]) : NPOINTS;

	if (argc > 4)
		num_points = stoi(argv[3]);
	else 
		num_points = NPOINTS;

	double range_rand = RRAND;
	
	JuliaFunction F = JuliaFunction();
	
	DisplayFunction* displayFunc = (DisplayFunction *) &F;
	
	SymbolicImage test = SymbolicImage(axes,
		boundaries,
		num_cells,
		//cell_size,
		num_points,
		displayFunc,
		range_rand
		//,true
	);
	//using graph_t = map<size_t, set<size_t> >;
	graph_t graph = test.CalcSymbolicImage_PRL(8);
	//graph_t graph = makeSimpleGraph();
	OrientedGraph g_test = OrientedGraph(graph);
	
	csc_t rew = g_test.findStrongConComponents();
	g_test.printToplgSortedGraph();

	/*reverse(rew.begin(), rew.end());
	for (vector<size_t> comp :rew){
		cout<<"comp_size:"<<comp.size()<<endl;
		for(size_t t :comp)
			cout<<t<<", ";
		cout<<endl;
	}
	*/

	return 0;
	
}