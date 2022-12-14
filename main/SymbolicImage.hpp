#ifndef SymbolicImage_hpp
#define SymbolicImage_hpp

#include <iostream>
#include <iterator>
#include <utility>
#include <pthread.h>
//-lpthread

#include "custom_types.hpp"
#include "mathFunction.hpp"

using namespace std;

#define A -2.0
#define B  2.0
#define C -2.0
#define D  2.0
#define NUMCELLS 50
#define DISPLAYFUNC JuliaFunction
#define RRAND 1000
#define NPOINTS 100

#define SEP_AXIS ';'
#define SEP_VAL ','
#define SEP_EQ ':'

/* тестовая функция */
/*
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
*/
//потом убрать !!!!

/*coords_t DISPLAYFUNC(const coords_t& coords) {
	double a = 0.15;
	double b = 0.45;
	double x = coords.first;
	double y = coords.second;
	double x1 = x * x - y * y + a;
    double y1 = 2 * x * y + b;
	return make_pair(x1, y1);
}
*/


/*схема работы CalcSymbolicImage -> displayСell -> displayPoint*/
class SymbolicImage{
protected:
	named_map_t num_cells; // число ячеек на оси  
	named_map_t cell_size; // размер ячейки  
	options_t boundaries; // набор краевый условий области
	size_t num_points; // количество точек в ячейке
	//dfunction_t displayFunc = nullptr; // фукнция для отображения
	DisplayFunction* displayFunc = nullptr; // класс фукнция для отображения
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
	// выполняет работу нити
	static void* CalcSymbolicImage_thread(void* arg);  
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
		//временно 
		//num_cells["y"] =
		num_points = NPOINTS;
		calcSegmentLength();
		calcCellSize();
		calcNumCells();
		// по умолчанию
		JuliaFunction F = JuliaFunction();
		displayFunc = (DisplayFunction *) &F;
		range_rand = RRAND;
	}
	named_map_t getCellSize(){
		return cell_size;
	}
	string getStrCellSize(){
		string res;
		for(string axis : axes){
			res.append(axis);
			res.push_back(SEP_EQ);
			res.append(to_string(cell_size[axis]));
			res.push_back(SEP_AXIS);
		}
		return res;
	}
	options_t getBoundaries(){
		return boundaries;
	}
	string getStrBoundaries(){
		string res;
		for(string axis : axes){
			res.append(axis);
			res.push_back(SEP_EQ);
			res.append(to_string(boundaries[axis]["begin"]));
			res.push_back(SEP_VAL);
			res.append(to_string(boundaries[axis]["end"]));
			res.push_back(SEP_AXIS);
		}
		return res;
	}
	string getParamName(){
		string name = string("SymIm");
		for(string axis : axes){
			name.push_back('_');
			name.append(to_string(cell_size[axis]));
		}
		name.push_back('_');
		name.append(to_string(num_points));
		return name;
	}
	SymbolicImage( set<string> axes,
		 options_t boundaries,
		 named_map_t num_cells,
		 size_t num_points,
		 DisplayFunction* displayFunc,
		 double range_rand
	){
		this->axes = axes;
		this->boundaries = boundaries;
		this->num_cells = num_cells;
		this->num_points = num_points;	
		this->displayFunc = displayFunc;
		this->range_rand = range_rand; 	
		calcSegmentLength();
		calcCellSize();
		calcNumCells();
		}

		SymbolicImage( set<string> axes,
		 options_t boundaries,
		 named_map_t cell_size,
		 size_t num_points,
		 DisplayFunction* displayFunc,
		 double range_rand,
		 bool cell_size_flag
	){
		this->axes = axes;
		this->boundaries = boundaries;
		this->cell_size = cell_size;
		this->num_points = num_points;	
		this->displayFunc = displayFunc;
		this->range_rand = range_rand; 	
		calcSegmentLength();
		calcNumCells();
		}
	virtual coords_t CellToPoint(size_t cell);
	//Отображает точки ячейки
	virtual size_t displayPoint(const double l_board, const double t_board);
	//Отображает ячейку
	virtual set<size_t> displayCell(const size_t cell);
	// считаем граф 
	virtual graph_t& CalcSymbolicImage ();
	//считает разбиение по по шагу
	virtual named_map_t calcNumCells();
	//параллельный вариант CalcSymbolicImage
	virtual graph_t& CalcSymbolicImage_PRL(size_t nthreads);
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