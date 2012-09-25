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

int main(void) {
	Parameters parameters;
	parameters.read_parameters_from_txt_file("data/parameters.txt");
	parameters.display_parameters();
	return EXIT_SUCCESS;
}
