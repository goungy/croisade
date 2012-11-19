/*
 * Solver.h
 *
 *  Created on: 27 sept. 2012
 *      Author: jd219546
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include <iostream>
using namespace std;

#include "SolverStage.h"
#include "Parameters.h"
#include "Grid.h"

class ISolver
{
public:
	virtual ~ISolver(){}
	virtual int get_word_size() const = 0;
	virtual void execute_solver() = 0;
};

class BaseSolver : public ISolver
{
public:
	BaseSolver(const Dictionnary * dico):
		my_dico_ptr(dico), my_grid(dico->wordsSize_),checker_stage(dico, my_grid)
	{}
	int get_word_size() const {return my_dico_ptr->wordsSize_;}
	virtual void execute_solver() = 0;
protected:
	void add_stage(SolverStage* new_stage)
	{
		this->solver_stages.push_back(new_stage);
	}
	const Dictionnary * my_dico_ptr;
	vector< SolverStage *> solver_stages;
	Grid my_grid;
	GridCheckerStage checker_stage;
};

class StephaneSolver : public BaseSolver
{
public:
	StephaneSolver(const Dictionnary * dico, const Parameters & parameters)
	: BaseSolver(dico),
	  my_mediator(atoi(parameters.get_parameter("saved_solver_state_prefix_size").c_str()), dico->wordsSize_)
	{
		my_prefix_max_size = atoi(parameters.get_parameter("saved_solver_state_prefix_size").c_str());
		this->check_max_prefix_size();
		int start_idx = 0;
		int last_idx = my_dico_ptr->wordsNumber_-1;
		cout << "Stephane solver adding 1st row placer" << endl;
		SimpleRowWordPlacerStage * first_r = new SimpleRowWordPlacerStage(my_dico_ptr, my_grid, &my_mediator, start_idx, last_idx, 0);
		first_row = first_r;
		my_mediator.register_stage(first_r);
		for (int i = 1 ; i < my_prefix_max_size ; i++)
		{
			int first_col_to_check = 0;
			int last_col_to_check = this->get_word_size()-1;
			cout << "Stephane solver adding "<< i << " row try and check" << endl;
			my_mediator.register_stage(new SimpleRowWordTryAndCheckStage(my_dico_ptr, my_grid, &my_mediator,start_idx, last_idx, i, first_col_to_check, last_col_to_check));
		}
		int first_row_to_check = my_prefix_max_size;
		int last_row_to_check = this->get_word_size()-1;
		cout << "Stephane solver adding 1st col prefix placer" << endl;
		my_mediator.register_stage(new ColWordPrefixTryStage(my_dico_ptr, my_grid, &my_mediator,start_idx, last_idx, 0, first_row_to_check, last_row_to_check, my_prefix_max_size));
		for(int i = 1; i < this->get_word_size() ; i++)
		{
			int first_row_to_check = my_prefix_max_size;
			int last_row_to_check = this->get_word_size()-1;
			cout << "Stephane solver adding "<< i<< " col prefix try and check placer" << endl;
			my_mediator.register_stage(new ColWordPrefixTryAndCheckStage(my_dico_ptr, my_grid, &my_mediator,start_idx, last_idx, i, first_row_to_check, last_row_to_check, my_prefix_max_size));
		}
		my_mediator.register_stage(&checker_stage);
	}
	void execute_solver()
	{
		//cout << "There are " << this->solver_stages.size() << " solver stages" << endl;
		first_row->execute_stage();
		cout << "Found " << checker_stage.get_grids_found_count() << " grids" << endl;
	}
protected:
private:
	void check_max_prefix_size() const
	{
		if (my_prefix_max_size > this->get_word_size())
			throw "Asking for a max prefix size larger than word size in Stephane solver";
	}

	int my_prefix_max_size;
	SolverStage * first_row;
	StephaneSolverMediator my_mediator;

};

#include "OptimizedDataStructures.h"

class ArthurSolver: public BaseSolver
{
public:
	ArthurSolver(const Dictionnary * dico, const Parameters & parameters):
		BaseSolver(dico), fixed_letters_tree(*dico, atoi(parameters.get_parameter("saved_solver_state_max_fixed_letters").c_str()))
	{

	}
	void execute_solver()
	{

	}
private:
	FixedLettersRootNode fixed_letters_tree;
};

#endif /* SOLVER_H_ */
