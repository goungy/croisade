/*
 * SolverMediator.cpp
 *
 *  Created on: 1 oct. 2012
 *      Author: jd219546
 */

#include "SolverMediator.h"


StephaneSolverMediator::StephaneSolverMediator(int prefix_size, int word_size)
:first_row_stage(0), second_to_k_row_stages(prefix_size-1),
 first_col_stage(0), col_stages(word_size-1)
{
}
void StephaneSolverMediator::notify_stage_finished(const SimpleRowWordPlacerStage * solver_stage)
{
	//cout << " row 0" << endl;
	const string & word = solver_stage->get_current_word();
	if (this->second_to_k_row_stages.size() > 0)
	{
		//cout << "tree update" << endl;
		bool ok = second_to_k_row_stages[0]->update_previous_trees_first_letter(word);
		//cout << "executing" << endl;
		if (ok) second_to_k_row_stages[0]->execute_stage();
	}
	else
	{
		this->first_col_stage->execute_stage();
	}
}
void StephaneSolverMediator::notify_stage_finished(const SimpleRowWordTryAndCheckStage * solver_stage)
{
	int row = solver_stage->get_row_or_col();
	//cout << "Row " << row  << endl;
	if ((row) != int(second_to_k_row_stages.size()))
	{
		bool ok = second_to_k_row_stages[row]->update_previous_trees_letter( solver_stage->get_current_word(),
			 solver_stage);
		if (ok) second_to_k_row_stages[row]->execute_stage();
	}
	else
	{
		//cout << "Executing first column" << endl;
		this->first_col_stage->execute_stage();
	}
}
void StephaneSolverMediator::notify_stage_finished(const ColWordPrefixTryStage * solver_stage)
{
	//cout << "Need to notify second col " << endl;
	//cout << "col 1" << endl;
	bool ok = col_stages[0]->update_previous_trees_first_letter(solver_stage->get_current_word());
	//solver_stage->get_grid_ref().print();
	if (ok) col_stages[0]->execute_stage();
}

void StephaneSolverMediator::notify_stage_finished(const ColWordPrefixTryAndCheckStage * solver_stage)
{
	//cout << "Need to notify second col " << endl;
	//solver_stage->get_grid_ref().print();
	int col = solver_stage->get_row_or_col();
	if (col != (solver_stage->get_dico_ptr()->wordsSize_-1))
	{
		//cout << "col " << col << endl;
		//solver_stage->get_grid_ref().print();
		const string & word = solver_stage->get_current_word();
		bool ok = col_stages[col]->update_previous_trees_letter(word, solver_stage);
		if (ok) col_stages[col]->execute_stage();
	}else
	{
		//cout << "check stage" << endl;
		this->check_stage->execute_stage();
		//cout << "Need to test grid, maybe it is good???" << endl;

		//solver_stage->get_grid_ref().print();
		//exit(0);
	}
}

void StephaneSolverMediator::register_stage(SimpleRowWordPlacerStage * stage)
{
	cout << "Registering simple row word " << stage->get_row_or_col() << endl;
	first_row_stage = stage;
}
void StephaneSolverMediator::register_stage(SimpleRowWordTryAndCheckStage * stage)
{
	cout << "Registering try and check row word " << stage->get_row_or_col() << endl;
	second_to_k_row_stages[stage->get_row_or_col()-1] = stage;
}
void StephaneSolverMediator::register_stage(ColWordPrefixTryStage * stage)
{
	cout << "Registering prefix col word " << stage->get_row_or_col() << endl;
	first_col_stage = stage;
}
void StephaneSolverMediator::register_stage(ColWordPrefixTryAndCheckStage * stage)
{
	cout << "Registering try and check prefix col " << stage->get_row_or_col() << endl;
	col_stages[stage->get_row_or_col()-1] = stage;
}




