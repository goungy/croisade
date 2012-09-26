//============================================================================
// Name        : croisade.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "Parameters.h"
#include "SolverBuilder.h"
int main(void) {
	try
	{
		Parameters parameters;
		parameters.read_parameters_from_txt_file("data/parameters.txt");
		parameters.display_parameters();
		SolverBuilder solver_builder;
		ISolver & my_solver = solver_builder.build_solver(parameters);
	}
	catch(const char * e)
	{
		cout << "ERROR during execution : " << e << endl;
	}
	return EXIT_SUCCESS;
}
