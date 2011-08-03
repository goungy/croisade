/*
 * SearchProcess.cpp
 *
 *  Created on: 25 nov. 2010
 *      Author: goungy
 */

#include "SearchProcess.h"
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <mpi.h>

#include <algorithm>
#include <ctime>

using namespace std;


int SearchProcess::check_columns(int row,const string & word,const Dictionnary& my_dico,Grid & my_grid,int my_tid)
{
	int columns_ok = -1;
	for(int col = row; col < my_dico.wordsSize_ ; col++)
	{
/*
		string prefix_col = my_grid.get_prefix_in_column(col,row);
		prefix_col += word[col];
		int index = my_dico.findFirstIndexOfPrefix(prefix_col);
*/
		Tree * tree_tmp = this->tree_cols[row-1][col]->get_node(word[col]);
		//columns_ok = tree_tmp != NULL;
		//columns_ok = index != -1;
		if ( tree_tmp == NULL)
		{
			columns_ok = col;
			break;
		}
		else
			this->tree_cols[row][col] = tree_tmp;
	}
	return columns_ok;
}

int SearchProcess::check_rows(int col,const string & word,const Dictionnary& my_dico,Grid & my_grid,int my_tid)
{
	int rows_ok = -1;
	for(int row = col+1; row < my_dico.wordsSize_ ; row++)
	{
		/*
		string prefix_row = my_grid.get_prefix_in_row(row,col);
		prefix_row += word[row];
		//const string & new_word = prefix_row;//(prefix_row + word[row]);
		int index = my_dico.findFirstIndexOfPrefix(prefix_row);
		*/
		Tree * tree_tmp = this->tree_rows[col-1][row]->get_node(word[row]);
		//columns_ok = tree_tmp != NULL;
		//rows_ok = tree_tmp != NULL;
		if (tree_tmp == NULL)
		{
			rows_ok = row;
			break;
		}
		//rows_ok = index != -1;
		else
			this->tree_rows[col][row] = tree_tmp;
	}
	return rows_ok;
}

void SearchProcess::fill_column(int col,const Dictionnary& my_dico,Grid & my_grid,int my_tid)
{
	if (col == my_dico.wordsSize_ -1)
	{
		//cout << "CANDIDATE!!!" << endl;
		if (my_grid.rows[col] == my_grid.cols[col])
			;//cout << "TOO bad word already in grid" << endl;
		else
		{
			this->nb_grids_found++;
			if (this->print_results) this->grid.print();
		}
		return;
	}
	const string & prefix = my_grid.get_prefix_in_column(col,col+1);
	int idxBeg = my_dico.findFirstIndexOfPrefix(prefix);
	if (idxBeg != -1)
	{
		for (int wordIdx = idxBeg ; wordIdx < my_dico.wordsNumber_ ; wordIdx++)
		{
			const string & word = my_dico.words_[wordIdx];
			//Checking if new word has same prefix as necessary
			const string word_prefix = word.substr(0,col+1);
			if (word_prefix != prefix)
				break;
			//Checking if new word satisfies row dependencies
			if (!this->check_rows(col,word,my_dico,my_grid,my_tid))
				continue;
			int row_diff = this->check_rows(col,word,my_dico,my_grid,my_tid);
			if (row_diff != -1)
			{
				const int next_idx = this->dico_.shared_prefix[wordIdx][row_diff]-1;
				//cout << "Jumping from " << wordIdx+1 << " to " << next_idx+1 << " because of col " << col_diff << endl;
				wordIdx = next_idx;
				continue;
			}
			//Checking if new word is not already in grid
			if (my_grid.word_already_in_grid(col+1,col,word,wordIdx))
				continue;
			//Everything's fine, putting word in grid
			my_grid.put_word_in_col(word,wordIdx,col);
			this->fill_row(col+1,my_dico,my_grid,my_tid);
		}
	}
}

void SearchProcess::fill_row(int row,const Dictionnary& my_dico,Grid & my_grid,int my_tid)
{
	const string & prefix = my_grid.get_prefix_in_row(row,row);
	int idxBeg = my_dico.findFirstIndexOfPrefix(prefix);

	if (idxBeg != -1)
	{
		for (int wordIdx = idxBeg ; wordIdx < my_dico.wordsNumber_ ; wordIdx++)
		{
			const string & word = my_dico.words_[wordIdx];
			//cout << "Trying to put word " << word << endl;
			//Checking if new word has same prefix as necessary
			const string word_prefix = word.substr(0,row);
			if (word_prefix != prefix)
				break;
			//Checking if new word satisfies column dependencies
			int col_diff = this->check_columns(row,word,my_dico,my_grid,my_tid);
			if (col_diff != -1)
			{
				const int next_idx = this->dico_.shared_prefix[wordIdx][col_diff]-1;
				//cout << "Jumping from " << wordIdx+1 << " to " << next_idx+1 << " because of col " << col_diff << endl;
				wordIdx = next_idx;
				continue;
			}
			//Checking if new word is not already in grid
			if (my_grid.word_already_in_grid(row,row,word,wordIdx))
				continue;
			//Everything's fine, putting word in grid
			my_grid.put_word_in_row(word,wordIdx,row);
			this->fill_column(row,my_dico,my_grid,my_tid);
		}
		return;
	}
}

void SearchProcess::put_word_in_first_column(int wordIdx,const Dictionnary& my_dico, Grid & my_grid, int my_tid)
{
	const string prefix = my_grid.get_prefix_in_column(0,1);
	int idxBeg = wordIdx+1;
	if (idxBeg >= my_dico.wordsNumber_)
		return;
	const string & word_after_word_in_first_row = my_dico.words_[idxBeg];
	if (prefix[0] != word_after_word_in_first_row[0])
		return;

	for( int wordIdx = idxBeg; wordIdx < my_dico.wordsNumber_; wordIdx++)
	{
		const string & word = my_dico.words_[wordIdx];
		//Checking if new word has same prefix as necessary
		if (word[0] != prefix[0])
			break;
		/* initializing temp trees */
		bool word_not_good = initialize_temp_trees(this->tree_rows[0],word);
		if (word_not_good) continue;
		my_grid.put_word_in_col(word,wordIdx,0);
		this->fill_row(1,my_dico,my_grid,my_tid);
	}
}

bool SearchProcess::initialize_temp_trees(vector<Tree *> & tree, const string & word)
{
	bool word_not_good = false;
	for(int i = 0; i < this->dico_.wordsSize_ ; i++)
	{
		tree[i] = dico_.tree.get_node(word[i]);
		if (tree[i] == NULL)
		{
			word_not_good = true;
			break;
		}
		//cout << this->dico_.words_[this->tree_cols[0][i]->isWord] << endl;;
	}
	return word_not_good;
}

void SearchProcess::try_to_fill_grid(int idxBeg,int idxEnd,int words_done2, double time_done)
{
	/*
	int idxBeg = 0;
	int idxEnd = this->dico_.words_.size()-1;
	*/
	double starting_clock = clock();
	//int nb_words_total = idxEnd-idxBeg+1;
	/*
#pragma omp parallel
	{
	*/
		words_done = words_done2;
		const Dictionnary & my_dico = dico_;
		Grid & my_grid = grid;//Grid(my_dico.wordsSize_);
		int my_tid = omp_get_thread_num();
//#pragma omp for schedule(static,15)
		for (int wordIdx = idxBeg ; wordIdx < idxEnd ; wordIdx++)
		{
			const string & word = my_dico.words_[wordIdx];
			if (count(tabu_list.begin(),tabu_list.end(),word) >= 1)
				continue;

			cout << "["<<MPI::COMM_WORLD.Get_rank()<<"] treating word " << word << endl;
			my_grid.put_word_in_row(word,wordIdx,0);
			/* Initializing temporary trees */
			bool word_not_good = initialize_temp_trees(this->tree_cols[0],word);
			if (word_not_good) continue;
			/* End of init */
			this->put_word_in_first_column(wordIdx,my_dico,my_grid,my_tid);
			/*
			words_done++;
			double current_clock = clock();
			double current_time = (current_clock - starting_clock) / (1. * CLOCKS_PER_SEC) + time_done;
			double time_per_word = current_time / (words_done);
			double rest_time = (dico_.wordsNumber_-words_done) * time_per_word;
			cout << words_done << " / "<< dico_.wordsNumber_<< " words ("<< word <<"), "<< this->nb_grids_found <<" grids done in " << current_time << " secs => remaining " << rest_time << " secs " <<endl;
			*/
		}
	//}//omp parallel
}

void SearchProcess::run(const Workitem & wi,int words_done, double time_done)
{
	try_to_fill_grid(wi.idxBeg,wi.idxEnd,words_done,time_done);

}

SearchProcess::SearchProcess(Dictionnary & dico):dico_(dico),nb_grids_found(0),grid(dico.wordsSize_),words_done(0),tree_rows(dico.wordsSize_),tree_cols(dico.wordsSize_),print_results(false){
	// TODO Auto-generated constructor stub
	for(int i = 0 ; i < tree_cols.size() ; i++)
	{
		tree_cols[i].resize(dico.wordsSize_);
		tree_rows[i].resize(dico.wordsSize_);
	}
	tabu_list.push_back(string("cacarderai"));//ok
	tabu_list.push_back(string("cacardasse"));//ok
	tabu_list.push_back(string("cacarderai"));//ok
	tabu_list.push_back(string("cacardames"));//ok
	tabu_list.push_back(string("accaparees"));//ok
	tabu_list.push_back(string("cacarderas"));//ok
	tabu_list.push_back(string("cacarderez"));//ok
	tabu_list.push_back(string("cascaderas"));//ok
	tabu_list.push_back(string("recarderas"));//ok
	tabu_list.push_back(string("cacardates"));//ok
	tabu_list.push_back(string("caracteres"));//ok
	tabu_list.push_back(string("accedasses"));//ok
	tabu_list.push_back(string("recadreras"));//ok
	tabu_list.push_back(string("accaparais"));//ok
	tabu_list.push_back(string("carapatera"));//ok
	tabu_list.push_back(string("cacabasses"));//ok
	tabu_list.push_back(string("cascaderai"));//ok
	tabu_list.push_back(string("decadreras"));//ok
	tabu_list.push_back(string("carapatees"));//ok
	tabu_list.push_back(string("cascadames"));//ok
	tabu_list.push_back(string("creacieres"));//ok

	tabu_list.push_back(string("accaparais"));//ok
	tabu_list.push_back(string("accaparera"));//ok
	tabu_list.push_back(string("accaparees"));//ok
	tabu_list.push_back(string("accedasses"));//ok
	tabu_list.push_back(string("carapatera"));//ok
	tabu_list.push_back(string("cacabasses"));//ok
	tabu_list.push_back(string("decadreras"));//ok
	tabu_list.push_back(string("carapatees"));//ok
	tabu_list.push_back(string("cascaderai"));//ok
	tabu_list.push_back(string("cascadames"));//ok
	tabu_list.push_back(string("accederait"));//ok
	tabu_list.push_back(string("creacieres"));//ok
	tabu_list.push_back(string("decarcassa"));//ok
	tabu_list.push_back(string("accapareur"));//ok
	tabu_list.push_back(string("crecerelle"));//ok
	tabu_list.push_back(string("caresseras"));//ok
	tabu_list.push_back(string("recadrerai"));//ok
	tabu_list.push_back(string("decarcasse"));//ok
	tabu_list.push_back(string("carcerales"));//ok
	tabu_list.push_back(string("accelerera"));//ok
	tabu_list.push_back(string("accederais"));//ok
	tabu_list.push_back(string("decadrerai"));//ok
	tabu_list.push_back(string("accaparait"));//ok
	tabu_list.push_back(string("recadrames"));//ok
	tabu_list.push_back(string("cacaberais"));//ok
	tabu_list.push_back(string("cascadasse"));//ok
	tabu_list.push_back(string("cadenceras"));//ok
	tabu_list.push_back(string("accretasse"));//ok
	tabu_list.push_back(string("accreteras"));//ok
	tabu_list.push_back(string("cadastrera"));//ok
	tabu_list.push_back(string("recadrerai"));//ok
	tabu_list.push_back(string("cascadates"));//ok
	tabu_list.push_back(string("cacaberait"));//ok
	tabu_list.push_back(string("cascaderez"));//ok
	tabu_list.push_back(string("cascadates"));//ok

	tabu_list.push_back(string("accederent"));//ok
	tabu_list.push_back(string("recadrasse"));//ok
	tabu_list.push_back(string("cadastrees"));//ok
	tabu_list.push_back(string("recerclera"));//ok
	tabu_list.push_back(string("carenceras"));//ok
	tabu_list.push_back(string("accreterai"));//ok
	tabu_list.push_back(string("accretames"));//ok
	tabu_list.push_back(string("recardames"));//ok
	tabu_list.push_back(string("cacaberent"));//ok
	tabu_list.push_back(string("carpaccios"));//ok

	sort(tabu_list.begin(),tabu_list.end());
}

SearchProcess::~SearchProcess() {
	// TODO Auto-generated destructor stub
}
