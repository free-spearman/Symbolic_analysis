#include "SymbolicImage.hpp"
#include "OrientedGraph.hpp"
#include <fstream>

#define PARAM_LABEL '#' 

#define SEP_SPACE " " 
using namespace std;
class Manager
{
	SymbolicImage* symim_p;
	OrientedGraph* graph_p;
	ofstream fout;
	string file_name;
	string path;
public:
	Manager(SymbolicImage& s, OrientedGraph& g, string path){
		symim_p = &s;
		graph_p = &g;
		file_name = symim_p->getParamName();
		//тут нужен иф на проверку /
		path.push_back('/');
		this->path = path;
		fout.open(path+file_name); // связываем объект с файлом

	}
	void writeData(){
		fout<<PARAM_LABEL<<"boundaries"<<endl;
		fout<<symim_p->getStrBoundaries()<<endl;
		fout<<endl;
		fout<<PARAM_LABEL<<"cell_size"<<endl;
		fout<<symim_p->getStrCellSize()<<endl;
		fout<<endl;
		fout<<PARAM_LABEL<<"components"<<endl;
		csc_t csc = graph_p->filterComponents();
		coords_t coords;
		for (vector<size_t> component :csc){
			for(size_t cell :component){
				coords = symim_p->CellToPoint(cell); 
				fout<<coords.first<<" "<<coords.second<<endl;
			}
		}
	}
	void close(){
		fout.close();
	}
	string getFile(){
		return path+file_name; 
	}
};