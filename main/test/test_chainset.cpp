#include "SymbolicImage.cpp"
#include "OrientedGraph.cpp"
#define TESTFUNC julia_func
#define NUM_CELLS_P 1
#define NPOINTS_P 2
#define TESTFUNC_P 3
#include "mathFunction.cpp"

coords_t TESTFUNC(const coords_t& coords);
/*
int main(int argc, char const *argv[])
{
	SymbolicImage test = SymbolicImage();
	cout<<"Создал test"<<endl;
	graph_t graph = test.CalcSymbolicImage();
	OrientedGraph g_test = OrientedGraph(graph);
	g_test.findStrongConComponents();
	g_test.printStrongConComponents();

	return 0;
}
*/

/*
set<string> axes,
		 options_t boundaries,
		 named_map_t num_cells,
		 size_t num_points,
		 dfunction_t displayFunc,
		 double range_rand
*/
int main(int argc, char const *argv[])
{
	//соберем приколюхи

	set<string> axes = {"x","y"};
	options_t boundaries;
	for (string axis : axes){
			named_map_t bound = {
				{"begin", A},
				{"end", B}
			};

			boundaries.insert({axis, 
				bound
			});
	}
	named_map_t num_cells;

	size_t in_num_cells = argc > 1? stoi(argv[NUM_CELLS_P]):NUMCELLS;
	cout<<in_num_cells<<endl;
	
	for (string axis : axes){
			num_cells.insert({axis,
				in_num_cells
			});
	}

	double range_rand = RRAND;
	size_t num_points = argc > NUM_CELLS_P ? stoi(argv[NPOINTS_P]) : NPOINTS;
	cout<<num_points<<endl;
	dfunction_t displayFunc = TESTFUNC;
	displayFunc(make_pair(1,1));
	cout<<displayFunc<<endl;
	SymbolicImage test = SymbolicImage(axes,
		boundaries,
		num_cells,
		num_points,
		displayFunc,
		range_rand
		);
	cout<<"Создал test"<<endl;
	graph_t graph = test.CalcSymbolicImage();
	cout<<"OrientedGraph"<<endl;
	OrientedGraph g_test = OrientedGraph(graph);
	g_test.findStrongConComponents();
	g_test.printStrongConComponents();

	return 0;
}