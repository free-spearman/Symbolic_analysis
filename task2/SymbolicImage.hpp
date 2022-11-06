#ifndef SymbolicImage_hpp
#define SymbolicImage_hpp

#include <iostream>

#include <cmath>
#include <string>
#include <set>
#include <map>

#include <iterator>
#include <utility>

using namespace std;

#define A -2.0
#define B  2.0
#define C -2.0
#define D  2.0
#define NUMCELLS 5
#define DISPLAYFUNC testFunc
#define RRAND 1000
#define NPOINTS 100

using graph_t = map<size_t, set<size_t> >;
using named_map_t = map<string, double>;
using options_t = map<string, named_map_t>;
using coords_t  = pair<double, double>;
using dfunction_t = coords_t (*)(const coords_t&);

/* тестовая функция */
pair<double, double> DISPLAYFUNC (const coords_t& coords ) {
	int** m = new int * [2];
	m[0] = new int[2];
	m[1] = new int[2];
	m[0][0] = 1;
	m[0][1] = -2;
	m[1][0] = 4;
	m[1][1] = -3;
	//int m[2][2] = { {1,-2}, {4,-3} };
	//double x1 = m[0][0] * coords.first + m[0][1] * coords.second;
    //double y1 = m[1][0] * coords.first + m[1][1] * coords.first;
    double x1 = coords.first - 0.9; 
    double y1 = coords.second - 0.9;
    delete(m);
	return make_pair(x1, y1);
}

/*схема работы CalcSymbolicImage -> displayСell -> displayPoint*/
class SymbolicImage{
	named_map_t num_cells; // число ячеек на оси  
	named_map_t cell_size; // размер ячейки  
	options_t boundaries; // набор краевый условий области
	size_t num_points; // количество точек в ячейке
	dfunction_t displayFunc = nullptr; // фукнция для отображения
	//граф образов
	graph_t image_graph;
	//длины отрезков области 
	named_map_t segment_lengths;
	//список осей, переменных
	set<string> axes; 
	//для вычисления диапазона разных точек
	double range_rand = RRAND;
	double randStep (string coords);
	// округлялка
	size_t roundUp(const double& src);
	// считает длину сегмента (x_к - x_н)
	virtual SymbolicImage& calcSegmentLength(); 
	
	// считает шаг по оси по разбиению 
	virtual SymbolicImage& calcCellSize();  
public:
	SymbolicImage(){
		axes.insert("x");
		axes.insert("y");
		for (string axis : axes){
			named_map_t bound = {
				{"begin", A},
				{"end", B}
			};

			boundaries.insert({axis, 
				bound
			});
		}
		for (string axis : axes){
			num_cells.insert({axis,
				NUMCELLS
			});
		}
		num_points = NPOINTS;
		calcSegmentLength();
		calcCellSize();
		calcNumCells();

		displayFunc = DISPLAYFUNC;
		range_rand = RRAND;
	}
	//Отображает точки ячейки
	virtual size_t displayPoint(const double l_board, const double t_board);
	//Отображает ячейку
	virtual set<size_t> displayCell(const size_t cell);
	// считаем граф 
	virtual graph_t& CalcSymbolicImage ();
	//считает разбиение по по шагу
	virtual named_map_t calcNumCells();
};
#endif /* SymbolicImage_hpp */
/*
-2    -1.2     -0.4   0.4    1.2     2
            2
   1*  		2 	    3 	  4 	  5
           1.2  
   6*  		7  	    8     9      10
     0.4 
  11* 		12 		13 	  14     15
    -0.6
  16* 		17 		18 	  19	 20
    -1.2
  21* 		22* 		23* 	  24*     25*
	  -2
*/