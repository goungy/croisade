/*
 * SolverDirector.h
 *
 *  Created on: 27 sept. 2012
 *      Author: jd219546
 */

#ifndef SOLVERDIRECTOR_H_
#define SOLVERDIRECTOR_H_

#include <string>
using namespace std;

#include "SolverBuilder.h"

class SolverDirector
{
public:
	SolverDirector(const string & filepath);
	ISolver & build_solver();
private:
	Parameters my_parameters;
	ISolverBuilder * my_solver_builder;
	string my_solver_type;
};

SolverDirector::SolverDirector(const string & filepath)
{
	my_parameters.read_parameters_from_txt_file(filepath);
	my_solver_type = my_parameters.get_parameter("solver_type");
	if (my_solver_type == "Stephane")
		my_solver_builder = new StephaneSolverBuilder();
	else
		my_solver_builder = new ArthurSolverBuilder();
}


ISolver & SolverDirector::build_solver()
{
	my_solver_builder->read_dictionary(
			my_parameters.get_word_size(),
			my_parameters.get_dictionary_file());
	my_solver_builder->check_code_version(
			atof(my_parameters.get_parameter("saved_solver_state_version").c_str()));
	my_solver_builder->create_solver(my_parameters);
	return my_solver_builder->get_solver();
}



#endif /* SOLVERDIRECTOR_H_ */
