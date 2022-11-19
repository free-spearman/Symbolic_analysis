#include "SymbolicImage.hpp"
using namespace std;
/*схема работы CalcSymbolicImage -> displayСell -> displayPoint*/

using SymIm_Pieces_t = struct{
	SymbolicImage* p_si;
	pieces_graph_t piece; 
};

double SymbolicImage::randStep(string coords){
		//h_rand = 1000 * h
	int norm_rand = cell_size[coords] * range_rand;
	return (rand() % (norm_rand - 1) + 1) / (double) range_rand;
}
size_t SymbolicImage::roundUp(const double& src){
	long long int down = (long long int) src;
	if (src > 0){
		return src > down ? down + 1: down;
	}
    return down; 
}

SymbolicImage& SymbolicImage::calcSegmentLength(){
	for( string axis : axes)
		segment_lengths.insert({ axis,
			fabs(boundaries[axis]["end"] - boundaries[axis]["begin"])
		});
	return *this;
}
named_map_t SymbolicImage::calcNumCells(){
	double all = 1.0;  
	for( string axis : axes){
		// хочу 25 ячеек, длина по оси 4, оси 2, тогда 4*2/25 = 0.32
		num_cells.insert({ axis, 
			segment_lengths[axis]/cell_size[axis]
		});
		all= all*num_cells[axis];
		}

	num_cells.insert( make_pair("total", all) ); 
	return num_cells;	
}
SymbolicImage& SymbolicImage::calcCellSize(){
	for( string axis : axes)
		cell_size.insert({axis,  
			segment_lengths[axis]/num_cells[axis]
		});
	return *this;	
}

size_t SymbolicImage::displayPoint(const double l_board,
	const double t_board
){
	double x = l_board + randStep("x");
    double y = t_board - randStep("y");
    coords_t tgt_coords = displayFunc->getXY(make_pair(x,y));			
	double tgt_x = tgt_coords.first;
	double tgt_y = tgt_coords.second;
	//int row1 = l_y - roundUp((y1 + len_y/2) / h);
    //int column1 = (int)((x1 + len_x/2) / h);
    //tgt_x:-3.958 tgt_y: -1.498 tgt_ncol:-4.9475 tgt_nrow:5
	double tgt_nrow = num_cells["y"] - roundUp((tgt_y + segment_lengths["y"]/2) / cell_size["y"]);
	double tgt_ncol = ((tgt_x + segment_lengths["x"]/2) / cell_size["x"]);
	//cout<<"tgt_x:"<<tgt_x<<" tgt_y: "<< tgt_y;
    //cout<<" tgt_ncol:"<<tgt_ncol<<" tgt_nrow:"<<tgt_nrow<<endl;
    //cout<<(size_t) num_cells["x"] * (size_t)tgt_nrow + (size_t)tgt_ncol + 1<<endl;
    //tgt_x:-1.379 tgt_y: -2.16 tgt_ncol:0.77625 tgt_nrow:4
    if ( tgt_nrow > num_cells["y"]-1 || tgt_nrow < 0  ||  tgt_ncol > num_cells ["x"] - 1 || tgt_ncol < 0 ){
    	return 0;
    }

    //cout<<(size_t) num_cells["x"] * (size_t)tgt_nrow + (size_t)tgt_ncol + 1<<endl;
    return (size_t) num_cells["x"] * (size_t)tgt_nrow + (size_t)tgt_ncol + 1;
}
//важно делать проверку на количество точек в tgt_cells
set<size_t> SymbolicImage::displayCell(const size_t cell){
	set<size_t> tgt_cells;
	//cout<<"cell"<<cell<<endl;
	//определяем ячейку  
    size_t nrow = roundUp( cell / (double)num_cells["x"]) - 1;
    size_t ncol = cell - nrow * num_cells["x"] - 1;	
	//границы ячейки
	double left_board = ncol*cell_size["x"] + boundaries["x"]["begin"]; 
	double top_board = (num_cells["y"] - nrow) * cell_size["y"] + boundaries["y"]["begin"];
	size_t tgt_cell = 0;
    for (size_t j = 0; j < num_points; j++){
    	tgt_cell = displayPoint(left_board, top_board);
    	if (tgt_cell)
    		tgt_cells.insert(tgt_cell); 
    }
    return tgt_cells;
}
graph_t& SymbolicImage::CalcSymbolicImage(){
	// вычисляем длины сегментов
	calcSegmentLength();
	calcCellSize();
	calcNumCells();
	size_t numberCells = num_cells["total"];
	// вычисляем количество ячеек нужно заменить ме = тод
    
    for (size_t i = 1; i < numberCells + 1; i++)
    	image_graph.insert({i, displayCell(i)} );
    return image_graph;
}
//вычисление одной нитки 
void* SymbolicImage::CalcSymbolicImage_thread(void* arg){
	// мне будет больно ...
	SymIm_Pieces_t* parameters = (SymIm_Pieces_t*) arg;
	
	SymbolicImage* p_si = parameters->p_si;
	pieces_graph_t* piece = &parameters->piece;
	
	for (size_t i = piece->begin; i < piece->end; i++)
    	piece->graph.insert({i, p_si->displayCell(i)});
    return 0;
}

//тип параллельный вариант
graph_t& SymbolicImage::CalcSymbolicImage_PRL(size_t nthreads){
	// вычисляем длины сегментов
	calcSegmentLength();
	calcCellSize();
	calcNumCells();
	size_t numberCells = num_cells["total"];
	// вычисляем количество ячеек нужно заменить ме = тод
    if (nthreads < 1)
    	nthreads = 1;
    pthread_t threads[nthreads-1];
    size_t niterations = (numberCells+1);
    size_t size_one_piece =  (niterations) / nthreads; // 10/4 = 2 
	size_t additive =  niterations % nthreads; //10%4 = 2
	// 2х3 + 2x2
	size_t active_threads = 0, distributed = 0;

	/*
	мне нужно будет слить в одну кучку кучу map(графов), 
	так что можно передать указатель на это добро
	
	нужна структура begin, end, graph  
	*/

	//pieces_graph_t pieces[nthreads];
	SymIm_Pieces_t parameters[nthreads];
	//parameters.p_si = this;
	
	//запуск ниток

	for (; active_threads < nthreads && additive > 0; active_threads++, additive--){
		//заполняем arg

		parameters[active_threads].piece.begin = distributed; //0 //3
		distributed += size_one_piece + 1; // 3 ) //6
		parameters[active_threads].piece.end = distributed;
		//pieces[active_threads].arg = arg;
		// вызов action
		
		parameters[active_threads].p_si = this;

		pthread_create( &threads[active_threads], 
			NULL, 
			CalcSymbolicImage_thread, //нужно дописать !!! или уже нет 
			(void*) &parameters[active_threads]
			);
	}
	// additive = 0 active_threads = 2

	for (; active_threads < nthreads - 1; active_threads++){
		//заполняем arg
		parameters[active_threads].piece.begin = distributed; //6//8 
		distributed += size_one_piece; // 8 
		parameters[active_threads].piece.end = distributed;

		parameters[active_threads].p_si = this;		
		// вызов action
		pthread_create( &threads[active_threads], 
			NULL, 
			CalcSymbolicImage_thread, 
			(void*) &parameters[active_threads]
		);
	}
	// active_threads = 3 
	// даем работу основной нитке
	parameters[active_threads].piece.begin = distributed; //8 
	distributed += size_one_piece; // 10 
	parameters[active_threads].piece.end = distributed;
	//вызов action
	parameters[active_threads].p_si = this;
	CalcSymbolicImage_thread( (void*) &parameters[active_threads]);
	//собрать потоки, граф и выйти
	//foo.insert(bar.begin(),bar.end());
	image_graph.insert(parameters[nthreads - 1].piece.graph.begin(),
		parameters[nthreads - 1].piece.graph.end()
	);
	for (size_t i = 0; i < nthreads - 1 ; i++){
		pthread_join( threads[i], NULL);
		image_graph.insert(parameters[i].piece.graph.begin(),
			parameters[i].piece.graph.end()
		);
	}

	return image_graph;
}

/*
int auto_parallelization ( void *arg,
	//const type_info arg_type,
	size_t nthreads, 
	size_t niterations 
	){
	if (niterations < nthreads ) 
		nthreads = niterations;
	
	arg_thread arg_list[nthreads];
	pthread_t threads[nthreads-1];   

	size_t size_one_piece =  niterations / nthreads; // 10/4 = 2 
	size_t additive =  niterations % nthreads; //10%4 = 2
	// 2х3 + 2x2
	size_t active_threads = 0, distributed = 0;
	//запуск ниток
	for (; active_threads < nthreads && additive > 0; active_threads++, additive--){
		//заполняем arg
		arg_list[active_threads].begin = distributed; //0 //3
		distributed += size_one_piece + 1; // 3 ) //6
		arg_list[active_threads].end = distributed;
		arg_list[active_threads].arg = arg;
		// вызов action
		pthread_create( &threads[active_threads], 
			NULL, 
			action, 
			(void*) &arg_list[active_threads]);
	}
	// additive = 0 active_threads = 2
	for (; active_threads < nthreads - 1; active_threads++){
		//заполняем arg
		arg_list[active_threads].begin = distributed; //6//8 
		distributed += size_one_piece; // 8 
		arg_list[active_threads].end = distributed;
		arg_list[active_threads].arg = arg;
		// вызов action
		pthread_create( &threads[active_threads], 
			NULL, 
			action, 
			(void*) &arg_list[active_threads]);
	}
	// active_threads = 3 
	// даем работу основной нитке
	arg_list[active_threads].begin = distributed; //8 
	distributed += size_one_piece; // 10 
	arg_list[active_threads].end = distributed;
	arg_list[active_threads].arg = arg;
	//вызов action
	action( (void*) &arg_list[active_threads] );
	//собрать потоки и выйти
	for (size_t i = 0; i < nthreads - 1 ; i++){
		pthread_join( threads[i], NULL);
		
	}
	return 0;
}

*/










































coords_t SymbolicImage::CellToPoint(size_t cell){
	//num_cells["x"] * (size_t)tgt_nrow + (size_t)tgt_ncol + 1
	//cout<<"cell:"<<cell<<endl;
	double nrow = roundUp( cell / (double)num_cells["x"]) - 1;
	//cout<<"nrow:"<<nrow<<endl;
    double ncol = cell - nrow* num_cells["x"];
    //cout<<"num_cells[x]"<<num_cells["x"]<<endl;
    //cout<<"ncol:"<<ncol<<endl<<endl;
	double x = (ncol-1)*cell_size["x"] + boundaries["x"]["begin"];
	double y = boundaries["y"]["end"] - nrow*cell_size["y"];
	return make_pair(x,y);
}



/*
(tgt_y + )  =  
(num_cells["y"] - double tgt_nrow)*cell_size["y"] - segment_lengths["y"]/2 ;
	double tgt_ncol = ((tgt_x + segment_lengths["x"]/2) / cell_size["x"]);
long long row = roundUp((double)comp / (double)c_x);
long long column = comp - (row - 1) * c_x;
out<<"row:"<<row<<" column"<<column<<endl;
double x_tl = d_x * (column - 1) + u_l;
double y_tl = (c_y - row + 1) * d_y + h_b;

*/