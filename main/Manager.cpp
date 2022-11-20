#include "SymbolicImage.hpp"
#include "OrientedGraph.hpp"
#include <fstream>

#define PARAM_LABEL '#' 
#define SEP_CMD '='
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

	}
	void open(){
		// добавить обработку на открытие потока
		fout.open(path+file_name); // связываем объект с файлом
	}
	void writeData(){
		open();
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
		close();
	}
	void close(){
		fout.close();
	}
	string getFile(){
		return path+file_name; 
	}

	/*pair< named_map_t, vector<string>> 
		сlParse ( int argc, char const *argv[]){
		size_t nparam = 1;
		named_map_t param;
		vector<string> names; 
		while(nparam < argc){
			string name = string(argv[nparam]);
			if (argv[nparam + 1] == SEP_CMD){
				names.push_back(name);
				param.insert({name, argv[nparam + 2]});
				nparam += 3;  
			}
			else
				nparam += 1;
		}
		return make_pair(param, names);
	}
	*/
};