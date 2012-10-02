/*
 * SolverStage.cpp
 *
 *  Created on: 1 oct. 2012
 *      Author: jd219546
 */
#include "SolverStage.h"

WordIntervalStage::WordIntervalStage(int start_idx, int last_idx, int row_or_col):
	start_idx(start_idx), last_idx(last_idx), row_or_col(row_or_col)
{
}
int WordIntervalStage::get_first_word_idx() const {return start_idx;}
int WordIntervalStage::get_last_word_idx() const {return last_idx;}
int WordIntervalStage::get_row_or_col() const {return row_or_col;}

SolverStage::~SolverStage(){}
SolverStage::SolverStage(const Dictionnary * dico_ptr, Grid & grid, ISolverMediator * med):
	my_dico_ptr(dico_ptr), my_grid_ref(grid), my_mediator(med)
{
}
ISolverMediator * SolverStage::get_my_mediator(){return this->my_mediator;}
const Dictionnary * SolverStage::get_dico_ptr() const{return my_dico_ptr;}
Grid & SolverStage::get_grid_ref() const{return my_grid_ref;}
const string & SolverStage::get_current_word() const {return current_word;}
void SolverStage::put_word_in_row(const string & word, int word_idx, int row)
{
	this->get_grid_ref().put_word_in_row(word, word_idx, row);
	this->set_current_word(word);
}
void SolverStage::put_word_in_column(const string & word, int word_idx, int col)
{
	this->get_grid_ref().put_word_in_col(word, word_idx, col);
	this->set_current_word(word);
}
void SolverStage::set_current_word(const string & word){current_word = word;}


SimpleRowWordPlacerStage::SimpleRowWordPlacerStage(const Dictionnary * dico_ptr, Grid & grid, ISolverMediator * med, int start_idx, int last_idx, int row):
	SolverStage(dico_ptr,grid,med),WordIntervalStage(start_idx, last_idx, row),
	PreviousTreeHolder(dico_ptr)
{
}

void SimpleRowWordPlacerStage::execute_stage()
{
	//cout << "EXECUTING" << endl;
	int row = this->get_row_or_col();
	for(int idx = get_first_word_idx() ; idx <= get_last_word_idx() ; idx++)
	{
		const string & word = this->get_dico_ptr()->get_word_from_index(idx);
		this->put_word_in_row(word, idx, row);
		//if (row == 0 )
		//	this->my_grid_ref.print();
			cout << "Placing word " << word << "[" << idx << "] in row " << row << endl;
//			if (idx == 100) break;
		this->get_my_mediator()->notify_stage_finished(this);
	}
}


SimpleRowWordTryAndCheckStage::SimpleRowWordTryAndCheckStage(const Dictionnary * dico_ptr, Grid & grid, ISolverMediator * med,
		int start_idx, int last_idx, int row, int first_col_to_check, int last_col_to_check)
:SolverStage(dico_ptr, grid, med),WordIntervalStage(start_idx, last_idx, row),
 IntervalCheckerStage(first_col_to_check, last_col_to_check),
 PreviousTreeHolder(dico_ptr)
{

}
void SimpleRowWordTryAndCheckStage::execute_stage()
{
	int row = this->get_row_or_col();
	for(int idx = get_first_word_idx() ; idx <= get_last_word_idx() ; idx++)
	{
		//cout << "Getting word idx " << idx << endl;
		const string & word = this->get_dico_ptr()->get_word_from_index(idx);
		//cout << word << endl;
		bool word_ok = true;
		for (int col_idx = get_first_index_to_check() ; col_idx <= get_last_index_to_check() && word_ok ; col_idx++)
		{
			word_ok = (this->get_previous_trees()[col_idx]->no_check_get_node(word[col_idx]) != 0);
		}
		//cout << "CHECK done" << endl;
		if (word_ok)
		{
			this->put_word_in_row(word, idx, row);
			//	if (row == 2)cout << "Placing word " << idx << " in row " << row << endl;
			this->get_my_mediator()->notify_stage_finished(this);
		}
		//cout << "OK done" << endl;
	}
}


ColWordPrefixTryStage::ColWordPrefixTryStage(const Dictionnary * dico_ptr, Grid & grid, ISolverMediator * med,
		int start_idx, int last_idx, int col, int first_col_to_check, int last_col_to_check,
		int prefix_size):
			SolverStage(dico_ptr, grid, med), WordIntervalStage(start_idx, last_idx, col),
			IntervalCheckerStage(first_col_to_check, last_col_to_check),
			PreviousTreeHolder(dico_ptr),
			prefix_size(prefix_size)
{
	for (int i = 0 ; i < int(dico_ptr->wordsNumber_) ; i++)
	{
		const string & word = dico_ptr->get_word_from_index(i);
		string prefix = word.substr(0, prefix_size);
		prefix_map[prefix].push_back(i);
	}
}

void ColWordPrefixTryStage::execute_stage()
{
	//cout << "Testing prefix in col " << this->get_row_or_col() << endl;
	const string & prefix = this->get_grid_ref().get_prefix_in_column(
			0,this->get_prefix_size());
	auto it = prefix_map.find(prefix);
	if (it == prefix_map.end()) return;
	const vector<int> & my_word_indexes = it->second;
	for (int i = 0 ; i < int(my_word_indexes.size()) ; i++)
	{
		int word_idx = my_word_indexes[i];
		//cout << "col _idx vs row _idx" << word_idx << " vs " << get_grid_ref().get_word_index_in_row(0) << endl;
		if (word_idx >= get_grid_ref().get_word_index_in_row(0))
			continue;
		const string & word = this->get_dico_ptr()->get_word_from_index(word_idx);
		this->put_word_in_column(word, word_idx, get_row_or_col());
		this->get_my_mediator()->notify_stage_finished(this);
	}
}

map<string, vector<int> > ColWordPrefixTryStage::prefix_map;

ColWordPrefixTryAndCheckStage::ColWordPrefixTryAndCheckStage(const Dictionnary * dico_ptr, Grid & grid,ISolverMediator * med,
		int start_idx, int last_idx, int col, int first_col_to_check, int last_col_to_check,
		int prefix_size):
			ColWordPrefixTryStage(
					dico_ptr, grid, med, start_idx, last_idx, col,
					first_col_to_check, last_col_to_check, prefix_size)
{

}

void ColWordPrefixTryAndCheckStage::execute_stage()
{
	const string & prefix = this->get_grid_ref().get_prefix_in_column(
				this->get_row_or_col(),this->get_prefix_size());
		auto it = prefix_map.find(prefix);
		if (it == prefix_map.end()) return;
		const vector<int> & my_word_indexes = it->second;
		for (int i = 0 ; i < int(my_word_indexes.size()) ; i++)
		{
			int word_idx = my_word_indexes[i];
			//cout << "col _idx vs row _idx" << word_idx << " vs " << get_grid_ref().get_word_index_in_row(0) << endl;
			const string & word = get_dico_ptr()->get_word_from_index(word_idx);
			bool word_ok = true;
			for (int row_idx = get_first_index_to_check() ; row_idx <= get_last_index_to_check() && word_ok ; row_idx++)
			{
				cout << "Checking letter " << word[row_idx] << endl;
				word_ok = (this->get_previous_trees()[row_idx]->no_check_get_node(word[row_idx]) != 0);
			}
			if (word_ok)
			{
				this->put_word_in_column(word, word_idx, get_row_or_col());
				if (get_row_or_col() == 3)cout << "Placing word " << word << "[" << word_idx << "] in col " << get_row_or_col() << endl;
				this->get_my_mediator()->notify_stage_finished(this);
			}
		}
}
