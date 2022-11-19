#ifndef custom_types_hpp
#define custom_types_hpp
/*from SymbolicImage*/
using namespace std;

#include <set>
#include <vector>
#include <map>
#include <string>


using graph_t = map<size_t, set<size_t> >;
using named_map_t = map<string, double>;
using options_t = map<string, named_map_t>;
using coords_t  = pair<double, double>;
using dfunction_t = coords_t (*)(const coords_t&);

/*from mathFunction*/

using xyt_t = struct
{
	double x;
	double y;
	double t;
};

/*from OrientedGraph*/
using csc_t = vector<vector<size_t>>;
#endif /*custom_types_hpp*/