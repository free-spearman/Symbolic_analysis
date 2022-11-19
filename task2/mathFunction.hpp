#ifndef mathFunction_hpp
#define mathFunction_hpp

#include "custom_types.hpp"

#include <iostream>
#include <cmath>

#define NUMITERS_DUFF 50
#define START_TIME_DUFF 0

using namespace std;

/* x¨ + kx˙ + αx + βx3 = B cos(ht),
Добавим замену: 
y = ˙x
тогда 
y˙ + k*y + αx + βx3 = B cos(ht)

x˙ = y,
y˙ = −ky − αx − βx3 + B cos(ht).

Если B != 0, система периодична с периодом ω = 2π/h
. Пусть h = 2 и (X(t, x, y), Y (t, x, y)) 
решение с начальными данными (x, y) при t = 0.

f : (x, y) → (X(π, x, y), Y (π, x, y))

*/
/*
coords_t duffing_func (const coords_t& coords){

}
*/ 
// что если сделать функции как классы (вместо шаблонов)

/*class DisplayFunction{
named_map_t param;
public:
	virtual double dx(coords_t& coords);
	virtual double dy(coords_t& coords);
	virtual coords_t Function()


}
*/
class DisplayFunction{
protected:
	// набор параметров 
	named_map_t param;
	// имена параметров
	vector<string> param_name;
	// параметры по умолчанию
	named_map_t DEF_PARAM;
public:
	DisplayFunction( named_map_t& param, 
		vector<string>& param_name,
		named_map_t& DEF_PARAM
	){
		this->param_name = param_name;
		this->DEF_PARAM = DEF_PARAM; 
		// m.find(idx) != m.end()
		for (string key : this->param_name){
			if (param.find(key) != param.end())
				this->param[key] = param[key];
			else 
				this->param[key] = getDefaultParam(key);  	
		}
	}

	virtual coords_t getXY(const coords_t& coords) = 0;

	//а в каждом конкретном своя структура
	virtual double dfy(void* variables) = 0;
	virtual double dfx(void* variables) = 0;

	double getDefaultParam(const string& key){
		return DEF_PARAM[key];
	}
};

/*const*/ vector<string> DEF_PARAM_NAME_DUFFING = {"damping", //k
	"linr_stiff", //α
	"restoring_force", //β
	"amplitude", //B
	"anglr_freq" //h 
};

/*const*/ named_map_t DEF_PARAM_DUFFING = { {"damping", 0.25},
	{"linr_stiff", -1.0},
	{"restoring_force", 1.0 },
	{"amplitude", 0.3},
	{"anglr_freq", 1.0}
};



class DuffingFunction: public DisplayFunction{

protected:
	
	double step;
	size_t num_iters;
	/*double k; //= 0.25;
	double alpha; //= -1;
	double beta; //= 1;
	double h; //= 1.0;
	double B; //= 0.3;
	*/

public:
	DuffingFunction(named_map_t& param, size_t iters)
	: DisplayFunction( param,
		DEF_PARAM_NAME_DUFFING,
		DEF_PARAM_DUFFING
		){
		//num_iters = iters;
		// тут наверное можно посчитать период?
		setStepByIters(iters);
	}
	DuffingFunction(): 
		DuffingFunction( DEF_PARAM_DUFFING, (size_t) NUMITERS_DUFF){}

	double getDefaultParam(const string& key){
		return DEF_PARAM[key];
	}

	coords_t getXY(const coords_t& coords){
		
		xyt_t vars = {coords.first,
			coords.second,
			START_TIME_DUFF
		};
		xyt_t out_vars;
		double k1, k2, k3, k4, q1, q2, q3, q4;
		for (size_t itr = 0; itr < num_iters; itr++) {
			out_vars = {vars.x, vars.y, vars.t};

			k1 = dfy((void*) &out_vars);
			q1 = dfx((void*) &out_vars);
 			
 			out_vars.x = vars.x + step * q1 / 2.0;
 			out_vars.y = vars.y + step * k1 / 2.0;
 			out_vars.t =  vars.t + step / 2.0; 

			k2 = dfy((void*) &out_vars);
			q2 = dfx((void*) &out_vars);

			out_vars.x = vars.x + step * q2 / 2.0;
 			out_vars.y = vars.y + step * k2 / 2.0;
 			out_vars.t =  vars.t + step / 2.0;
	
			k3 = dfy((void*) &out_vars);
			q3 = dfx((void*) &out_vars);
			
			out_vars.x = vars.x + step * q3;
 			out_vars.y = vars.y + step * k3;
 			out_vars.t = vars.t + step;

			k4 = dfy((void*) &out_vars);
			q4 = dfx((void*) &out_vars);
	
			vars.y += step * (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
			vars.x += step * (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
			vars.t += step;
		//cout << t << " " << x << " " << y << endl;
		}
		return make_pair(vars.x,vars.y);
	}

	double dfy(void* variables){
		xyt_t* v = (xyt_t*) variables;
		return -1.0 * param["damping"] * v->y -  
		param["linr_stiff"] * v->x - param["restoring_force"]*pow(v->x,3) 
		+ param["amplitude"] * cos(param["anglr_freq"] * v->t);
	}
	double dfx(void* variables){
		xyt_t* v = (xyt_t*) variables;
		return v->y;
	}
	double setStepByIters(size_t iters){
		step = 2 * M_PI / param["anglr_freq"] / iters;
		num_iters = iters;
		return step; 
	}
	//сомнительно, но пусть будет
	size_t setItersByStep(double step){
		num_iters  =(size_t)(2 * M_PI / step / param["anglr_freq"]) ;
		this->step = step;
		return num_iters;
	}

};

/*const*/ vector<string> DEF_PARAM_NAME_JULIA = {"a", "b"};
/*const*/ named_map_t DEF_PARAM_JULIA = { {"a", 0.15},
	{"b", 0.45}
};

class JuliaFunction: public DisplayFunction{

public:
	JuliaFunction(named_map_t& param)
	:DisplayFunction( param,
		DEF_PARAM_NAME_JULIA,
		DEF_PARAM_JULIA
		){}
	JuliaFunction(): JuliaFunction(DEF_PARAM_JULIA){
		/*cout<<"default JuliaFunction"<<endl;*/
	}
	coords_t getXY(const coords_t& coords){
		double x = coords.first;
		double y = coords.second;
		double x1 = pow(x, 2) - pow(y, 2) + param["a"];
    	double y1 = 2 * x * y + param["b"];
		return make_pair(x1, y1);
	}

	//а в каждом конкретном своя структура
	double dfy(void* variables){
		return 0;
	}
	double dfx(void* variables){
		return 0;
	}

};

/*
class DuffingFunction{
	//тут наверное нужен шаблон, а не класс, но ладно
	named_map_t param;
	vector<string> param_name = {"damping", //k
		"linr_stiff", //α
		"restoring_force", //β
		"amplitude", //B
		"anglr_freq" //h 
	};
	const named_map_t DEFPARAM = named_map_t({
		{"damping", 0.25}
		{"linr_stiff", -1.0},
		{"restoring_force", 1.0 },
		{"amplitude", 0.3},
		{"anglr_freq", 1.0}
	});
	double step;
	size_t num_iters;

	/*double k; //= 0.25;
	double alpha; //= -1;
	double beta; //= 1;
	double h; //= 1.0;
	double B; //= 0.3;
	*/
/*
public:
	DuffingFunction(const named_map_t& param, size_t iters){
		for (string key : param_name){
			if (param.contains(key))
				this->param[key] = param[key];
			else 
				this->param[key] = getDefaultParam(key);  	
		}
		//num_iters = iters;
		setStepByIters(iters);
		// тут наверное можно посчитать период?
	}
	DuffingFunction(){
		DuffingFunction(DEFPARAM, NUMITERS);
	}
	double getDefaultParam(const string& key){
		return DEFPARAM[key];
	}
	coords_t getXY(const coords_t& coords){
		double x = coords.first;
		double y = coords.second;
		double t = START_TIME_DUFF; 
		double k1, k2, k3, k4, q1, q2, q3, q4;
		for (size_t itr = 0; itr < num_iters; itr++) {
			k1 = dfy_2d(x, y, t);
			q1 = dfx_2d(y);
 	
			k2 = dfy_2d(x + d * q1 / 2.0, y + d * k1 / 2.0, t + d / 2.0);
			q2 = dfx_2d(y + d * k1 / 2.0);
	
			k3 = dfy_2d(x + d * q2 / 2.0, y + d * k2 / 2.0, t + d / 2.0);
			q3 = dfx_2d(y + d * k2 / 2.0);

			k4 = dfy_2d(x + d * q3, y + d * k3, t + d);
			q4 = dfx_2d(y + d * k3);
	
			y += d * (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
			x += d * (q1 + 2.0 * q2 + 2.0 * q3 + q4) / 6.0;
			t += d;
		//cout << t << " " << x << " " << y << endl;
		}
		return make_pair(x,y);
	}

	double dfy(double x, double y, double t){
		return -1.0 * param["damping"] * y -  
		param["linr_stiff"] * x - param["restoring_force"]*pow(x,3) 
		+ param["amplitude"] * cos(param["anglr_freq"] * t);
	}
	double dfx(double y){
		return y;
	}
	double setStepByIters(size_t iters){
		step = 2 * M_PI / param["anglr_freq"] / iters;
		num_iters = iters;
		return step; 
	}
	//сомнительно, но пусть будет
	size_t setItersByStep(double step){
		num_iters  =(size_t)(2 * M_PI / step / param["anglr_freq"]) ;
		this->step = step;
		return num_iters;
	}

}

*/

#endif /* mathFunction_hpp */