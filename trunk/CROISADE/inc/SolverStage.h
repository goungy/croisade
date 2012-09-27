/*
 * SolverStage.h
 *
 *  Created on: 27 sept. 2012
 *      Author: jd219546
 */

#ifndef SOLVERSTAGE_H_
#define SOLVERSTAGE_H_

#include <iostream>
using namespace std;

#include "Grid.h"

class SolverStage
{
public:
	SolverStage(const Dictionnary * dico_ptr, Grid & grid):
		my_dico_ptr(dico_ptr), my_grid_ref(grid),next_stage(0)
	{
		initialize_previous_trees(dico_ptr->wordsSize_);
	}
	virtual void execute_stage() = 0;
	void link_to_next_stage(SolverStage * stage){ this->next_stage = stage;}
	static void update_row_previous_trees(int row, const string & word, const Dictionnary * dico)
	{
		vector<const Tree *> & row_to_update = previous_trees[row];
		if (row == 0)
			for(int col_idx = 0 ; col_idx < int(row_to_update.size()) ; col_idx++)
			{
				//cout << "Updating " << row << "x"<<col_idx << endl;
				const Tree * node = dico->tree.no_check_get_node(word[col_idx]);
				if (!node) throw "Error : prefix not found as it should be when updating previous_trees";
				row_to_update[col_idx] = node;
			}
		else
			for(int col_idx = 0 ; col_idx < int(row_to_update.size()) ; col_idx++)
			{
				//cout << "Updating " << row << "x"<<col_idx << endl;
				row_to_update[col_idx] = previous_trees[row-1][col_idx]->no_check_get_node(word[col_idx]);
			}
	}
	static void initialize_previous_trees(int word_size)
	{
		if (previous_trees.size() != 0)
			if (previous_trees.size() != word_size)
				throw "Trying to resize previously sized previous_trees to another size, which is forbidden for now!";

		previous_trees.resize(word_size);
		for (auto it = previous_trees.begin() ; it != previous_trees.end() ; it++)
			it->resize(word_size);
	}
protected:
	const Dictionnary * my_dico_ptr;
	Grid & my_grid_ref;
	SolverStage * next_stage;
	static vector< vector<const Tree *> >previous_trees;
};

vector< vector<const Tree *> > SolverStage::previous_trees;

class DummyStage : public SolverStage
{
public:
	DummyStage(const Dictionnary * dico_ptr, Grid & grid):SolverStage(dico_ptr, grid)
	{}
	void execute_stage()
	{
		//cout << "Dummy stage doing nothing" << endl;
	}
};

class SimpleRowWordPlacerStage : public SolverStage
{
public:
	SimpleRowWordPlacerStage(const Dictionnary * dico_ptr, Grid & grid, int start_idx, int last_idx, int row):
		SolverStage(dico_ptr,grid),start_idx(start_idx), last_idx(last_idx), row(row)
	{
	}

	void execute_stage()
	{
		for(int idx = start_idx ; idx <= last_idx ; idx++)
		{
			const string & word = this->my_dico_ptr->get_word_from_index(idx);
			this->my_grid_ref.put_word_in_row(word, idx, this->row);
			//if (row == 0 )
			//	this->my_grid_ref.print();
				cout << "Placing word " << idx << " in row " << row << endl;
			update_row_previous_trees(row, word, my_dico_ptr);
			next_stage->execute_stage();
		}
	}
protected:
	int start_idx;
	int last_idx;
	int row;
};

class SimpleRowWordTryAndCheckStage : public SimpleRowWordPlacerStage
{
public:
	SimpleRowWordTryAndCheckStage(const Dictionnary * dico_ptr, Grid & grid,
			int start_idx, int last_idx, int row, int first_col_to_check, int last_col_to_check)
	:SimpleRowWordPlacerStage(dico_ptr, grid, start_idx, last_idx, row),
	 first_col_to_check(first_col_to_check), last_col_to_check(last_col_to_check)
	{
	}
	void execute_stage()
	{
		/*
		vector<const Tree * > col_prefix_in_tree(last_col_to_check-first_col_to_check+1);
		for (int col_idx = first_col_to_check ; col_idx <= last_col_to_check ; col_idx++)
		{
			const string grid_prefix = my_grid_ref.get_prefix_in_column(col_idx, row);
			col_prefix_in_tree[col_idx] = this->my_dico_ptr->findFirstNodeOfPrefix(grid_prefix);
		}
		*/
		for(int idx = start_idx ; idx <= last_idx ; idx++)
		{
			const string & word = this->my_dico_ptr->get_word_from_index(idx);

			bool word_ok = true;
			for (int col_idx = first_col_to_check ; col_idx <= last_col_to_check && word_ok ; col_idx++)
			{
				//cout << "Testing prefix " << prefix << " for row " << this->row << " and col " << col_idx << " for word " << word << endl;
				//cout << "Previous_trees sizes : " << previous_trees.size() << endl;
				//cout << "Row prev trees size : " << previous_trees[row].size() << endl;
				//cout << "Getting next node from " << row << "x" << col_idx << endl;
				word_ok = (previous_trees[row-1][col_idx]->no_check_get_node(word[col_idx]) != 0);
			}
			if (word_ok)
			{
				this->my_grid_ref.put_word_in_row(word, idx, this->row);
				//if (row == 2 )
				//	this->my_grid_ref.print();
				//cout << "Placing word " << idx << " in row " << row << endl;
				update_row_previous_trees(row, word, my_dico_ptr);
				next_stage->execute_stage();
			}
		}
	}


private:
	int first_col_to_check;
	int last_col_to_check;
};



#endif /* SOLVERSTAGE_H_ */
