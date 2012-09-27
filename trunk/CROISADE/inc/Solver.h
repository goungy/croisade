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
	virtual int get_word_size() const = 0;
	virtual void execute_solver() = 0;
};

class BaseSolver : public ISolver
{
public:
	BaseSolver(const Dictionnary * dico):my_dico_ptr(dico), my_grid(dico->wordsSize_)
	{}
	int get_word_size() const {return my_dico_ptr->wordsSize_;}
	virtual void execute_solver() = 0;
protected:
	void add_stage(SolverStage* new_stage)
	{
		this->solver_stages.push_back(new_stage);
	}
	void link_solver_stages()
	{
		for(auto it = solver_stages.begin() ; it != (solver_stages.end()-1) ; it++)
			(*it)->link_to_next_stage(*(it+1));
		solver_stages.back()->link_to_next_stage(new DummyStage(my_dico_ptr, my_grid));
	}
	const Dictionnary * my_dico_ptr;
	vector< SolverStage *> solver_stages;
	Grid my_grid;
};

class StephaneSolver : public BaseSolver
{
public:
	StephaneSolver(const Dictionnary * dico, const Parameters & parameters)
	: BaseSolver(dico)
	{
		my_prefix_max_size = atoi(parameters.get_parameter("saved_solver_state_prefix_size").c_str());
		this->check_max_prefix_size();
		int start_idx = 0;
		int last_idx = my_dico_ptr->wordsNumber_-1;
		this->add_stage(new SimpleRowWordPlacerStage(my_dico_ptr, my_grid, start_idx, last_idx, 0));
		for (int i = 1 ; i < my_prefix_max_size ; i++)
		{
			int first_col_to_check = 0;
			int last_col_to_check = this->get_word_size()-1;
			this->add_stage(new SimpleRowWordTryAndCheckStage(my_dico_ptr, my_grid, start_idx, last_idx, i, first_col_to_check, last_col_to_check));
		}
		this->link_solver_stages();
	}
	void execute_solver()
	{
		cout << "There are " << this->solver_stages.size() << " solver stages" << endl;
		solver_stages[0]->execute_stage();
	}
protected:
private:
	void check_max_prefix_size() const
	{
		if (my_prefix_max_size > this->get_word_size())
			throw "Asking for a max prefix size larger than word size in Stephane solver";
	}

	int my_prefix_max_size;
};



#endif /* SOLVER_H_ */
