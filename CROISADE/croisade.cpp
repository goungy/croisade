//============================================================================
// Name        : croisade.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include <stdlib.h>
#include "SolverDirector.h"

int main(void) {
	try
	{
		SolverDirector solver_director("data/parameters.txt");
		ISolver & my_solver = solver_director.build_solver();
		my_solver.execute_solver();
		cout << "Done" << endl;
	}
	catch(const char * e)
	{
		cout << "ERROR during execution : " << e << endl;
	}
	return EXIT_SUCCESS;
}
