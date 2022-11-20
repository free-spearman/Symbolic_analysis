#include "../main/Manager.cpp"
#include "../main/SymbolicImage.cpp"
#include "../main/OrientedGraph.cpp"
#include "../main/mathFunction.hpp"
//#define TESTFUNC JuliaFunction
#define NUM_CELLS_P 1
#define NPOINTS_P 2
#define TESTFUNC_P 3


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
	
	DuffingFunction F = DuffingFunction();
	
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

	graph_t graph = test.CalcSymbolicImage_PRL(8);
	
	OrientedGraph g_test = OrientedGraph(graph);
	g_test.findStrongConComponents();
	//g_test.printStrongConComponents();
	Manager m = Manager(test, g_test, "data");
	//cout<<"начинаю запись!"<<endl;
	m.writeData();
	m.close();
	cout<<m.getFile();
	return 0;
}