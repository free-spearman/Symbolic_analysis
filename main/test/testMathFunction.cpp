#include "mathFunction.hpp"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
	/* code */
	JuliaFunction a = JuliaFunction();
	DisplayFunction* b = (DisplayFunction*) &a;

	cout<<b->getXY( make_pair(1,1)).first <<endl;
	return 0;
}
