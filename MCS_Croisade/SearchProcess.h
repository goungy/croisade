/*
 * SearchProcess.h
 *
 *  Created on: 25 nov. 2010
 *      Author: goungy
 */

#ifndef SEARCHPROCESS_H_
#define SEARCHPROCESS_H_

#include "Dictionnary.h"
#include "Grid.h"
#include "Workitem.h"
#include "Tree.h"

class SearchProcess {
public:
	SearchProcess(Dictionnary & dico);
	virtual ~SearchProcess();

	void run(const Workitem & wi,int words_done, double time_done);
	int check_columns(int row,const string & word,const Dictionnary& my_dico,Grid & my_grid,int my_tid);
	int check_rows(int col,const string & prefix,const Dictionnary& my_dico,Grid & my_grid,int my_tid);
	void fill_row(int row,const Dictionnary& my_dico,Grid & my_grid,int my_tid);
	void fill_column(int col,const Dictionnary& my_dico,Grid & my_grid,int my_tid);
	void put_word_in_first_column(int wordIdx,const Dictionnary& my_dico,Grid & my_grid,int my_tid);
	bool initialize_temp_trees(vector<Tree *> & tree, const string & word);
	const Dictionnary  dico_;
	Grid grid;
	//vector<Grid> grid;
	//vector<Dictionnary> dicos;
	int nb_grids_found;
	int words_done;
	vector < vector<Tree*> >tree_rows;
	vector < vector<Tree*> >tree_cols;

	vector<string> tabu_list;

	bool print_results;

private:
	void try_to_fill_grid(int idxBeg,int idxEnd,int words_done, double time_done);
};

#endif /* SEARCHPROCESS_H_ */
