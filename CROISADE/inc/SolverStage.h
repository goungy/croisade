/*
 * SolverStage.h
 *
 *  Created on: 27 sept. 2012
 *      Author: jd219546
 */

#ifndef SOLVERSTAGE_H_
#define SOLVERSTAGE_H_

#include <map>
#include <iostream>
using namespace std;

#include "Grid.h"
#include "Dictionnary.h"
#include "SolverMediator.h"

class WordIntervalStage
{
public:
	WordIntervalStage(int start_idx, int last_idx, int row_or_col);
	int get_first_word_idx() const;
	int get_last_word_idx() const;
	int get_row_or_col() const;
private:
	int start_idx;
	int last_idx;
	int row_or_col;
};

class ISolverMediator;

class SolverStage
{
public:
	virtual ~SolverStage();
	SolverStage(const Dictionnary * dico_ptr, Grid & grid, ISolverMediator * med);
	virtual void execute_stage() = 0;
	ISolverMediator * get_my_mediator();
	const Dictionnary * get_dico_ptr() const;
	Grid & get_grid_ref() const;
	const string & get_current_word() const;
	void put_word_in_row(const string & word, int word_idx, int row);
	void put_word_in_column(const string & word, int word_idx, int col);
protected:
	void set_current_word(const string & word);
private:
	const Dictionnary * my_dico_ptr;
	Grid & my_grid_ref;
	ISolverMediator * my_mediator;
	string current_word;
};

class GridCheckerStage:public SolverStage
{
public:
	GridCheckerStage(const Dictionnary * dico_ptr, Grid & grid):
		SolverStage(dico_ptr,grid,0), grids_found_count(0)
	{
	}
	void execute_stage()
	{
		bool grid_ok = true;
		int word_size = get_grid_ref().get_word_size();
		for(int i = 0 ; i < word_size && grid_ok; i++)
		{
			const string word = get_grid_ref().get_prefix_in_row(i, word_size);
			grid_ok = get_dico_ptr()->findFirstNodeOfPrefix(word);
			if (!grid_ok)
				cout << "word " << word << " does not exist in row " << i << endl;
		}
		for(int i = 0 ; i < word_size && grid_ok; i++)
		{
			const string word = get_grid_ref().get_prefix_in_column(i, word_size);
			grid_ok = get_dico_ptr()->findFirstNodeOfPrefix(word);
			if (!grid_ok)
				cout << "word " << word << " does not exist in column " << i << endl;
		}
		if (grid_ok)
			grid_ok = get_grid_ref().check_no_doublons_in_grid();
		//cout << "Grid ok = " << grid_ok << endl;
		//get_grid_ref().print();
		//exit(0);
		if (grid_ok)
		{
			//cout << "Words in grid ok" << endl;
			//get_grid_ref().print();
			grids_found_count++;
			//exit(0);
		}
		//get_grid_ref().print();
	}
	int get_grids_found_count()const {return grids_found_count;}
private:
	int grids_found_count;
};

class DummyStage : public SolverStage
{
public:
	DummyStage(const Dictionnary * dico_ptr, Grid & grid):SolverStage(dico_ptr, grid, 0)
	{}
	void execute_stage()
	{
		//cout << "Dummy stage doing nothing" << endl;
	}
};

class PreviousTreeHolder
{
public:
	PreviousTreeHolder(const Dictionnary * dico):
		previous_trees(dico->wordsSize_), my_dico(dico)
	{}
	bool update_previous_trees_first_letter(const string & word)
	{
		//cout << "Updating THE previous tree first letter with word " << word << endl;
		for(unsigned int i = 0 ; i < previous_trees.size() ; i++)
		{
			//cout << "Ok for " << i << endl;
			const Tree* tree = my_dico->tree.get_node(word[i]);
			if (!tree)
				return false;
			//cout << "Ok for " << i << endl;
			previous_trees[i] = tree;
			//cout << "Ok for " << i << endl;
		}
		return true;
	}

	bool update_previous_trees_letter(const string & word, const PreviousTreeHolder * other_trees)
	{
		for(unsigned int i = 0 ; i < previous_trees.size() ; i++)
		{
			previous_trees[i] = ((other_trees->get_previous_trees())[i])->no_check_get_node(word[i]);
			if (!previous_trees[i])
				return false;
		}
		return true;
	}
	const vector< const Tree * > & get_previous_trees() const{return previous_trees;}
private:
	vector< const Tree * > previous_trees;
	const Dictionnary * my_dico;
};

class SimpleRowWordPlacerStage : public SolverStage, public WordIntervalStage, public PreviousTreeHolder
{
public:
	SimpleRowWordPlacerStage(const Dictionnary * dico_ptr, Grid & grid, ISolverMediator * med,
			int start_idx, int last_idx, int row);

	void execute_stage();
};

class IntervalCheckerStage
{
public:
	IntervalCheckerStage(int first_col_to_check, int last_col_to_check):
		first_col_to_check(first_col_to_check), last_col_to_check(last_col_to_check){}
	int get_first_index_to_check() const { return first_col_to_check;}
	int get_last_index_to_check() const { return last_col_to_check;}
private:
	int first_col_to_check;
	int last_col_to_check;
};

class SimpleRowWordTryAndCheckStage :
	public SolverStage, public WordIntervalStage, public IntervalCheckerStage, public PreviousTreeHolder
{
public:
	SimpleRowWordTryAndCheckStage(const Dictionnary * dico_ptr, Grid & grid, ISolverMediator * med,
			int start_idx, int last_idx, int row, int first_col_to_check, int last_col_to_check);
	void execute_stage();
};

class ColWordPrefixTryStage : public SolverStage,
public WordIntervalStage, public IntervalCheckerStage, public PreviousTreeHolder
{
public:
	ColWordPrefixTryStage(const Dictionnary * dico_ptr, Grid & grid, ISolverMediator * med,
			int start_idx, int last_idx, int col, int first_col_to_check, int last_col_to_check,
			int prefix_size);
	void execute_stage();
	int get_prefix_size() const {return prefix_size;}
protected:
	static map<string,vector<int> > prefix_map;
private:
	int prefix_size;

};



class ColWordPrefixTryAndCheckStage : public ColWordPrefixTryStage
{
public:
	ColWordPrefixTryAndCheckStage(const Dictionnary * dico_ptr, Grid & grid,ISolverMediator * med,
			int start_idx, int last_idx, int col, int first_col_to_check, int last_col_to_check,
			int prefix_size);

	void execute_stage();

protected:
private:
};


#endif /* SOLVERSTAGE_H_ */
