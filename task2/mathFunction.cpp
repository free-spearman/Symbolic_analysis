#include <cmath>
#include "SymbolicImage.hpp"

//coords_t (*)(const coords_t&);
coords_t julia_func(const coords_t& coords) {
	double a = 0.15;
	double b = 0.45;

	double x = coords.first;
	double y = coords.second;
	double x1 = x * x - y * y + a;
    double y1 = 2 * x * y + b;
	return make_pair(x1, y1);
}