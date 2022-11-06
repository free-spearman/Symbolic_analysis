#include <iostream>
#include "SymbolicImage.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
	SymbolicImage test;
	cout<<"Создал test"<<endl;
	graph_t graph = test.CalcSymbolicImage();
	cout<<"CalcSymbolicImage"<<endl;
	size_t num_cells = test.calcNumCells()["total"];
	cout << num_cells << endl;
	for (int i = 1; i < num_cells + 1 ; i++)
	{
		cout << i << ": ";
        copy(graph[i].begin(), graph[i].end(), ostream_iterator<int>(cout, " "));
        cout << endl;
	}
	return 0;
}