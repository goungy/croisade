#ifndef SOLVERBUILDER
#define SOLVERBUILDER

#include "Dictionnary.h"

class ISolver
{

	

};

class SolverBuilder
{
public:
	SolverBuilder();
	ISolver & build_solver(const Parameters & parameters);
private:
	int word_size;
	Dictionnary * my_dico;
	ISolver * my_solver;
};

SolverBuilder::SolverBuilder():word_size(0),my_dico(0),my_solver(0)
{
}
ISolver & SolverBuilder::build_solver(const Parameters & parameters)
{
	word_size = atoi(parameters.get_parameter("word_size").c_str());
	my_dico = new Dictionnary(parameters.get_parameter("dictionary_file"), word_size);
	my_solver = new ISolver();
	return *my_solver;	
}

#endif
