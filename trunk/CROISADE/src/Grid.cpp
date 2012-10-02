/*
 * Grid.cpp
 *
 *  Created on: 25 nov. 2010
 *      Author: goungy
 */

#include "Grid.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;

bool Grid::word_already_in_grid(int rows, int cols,const string & word, int idx) const
{

	if (count(this->rows.begin(),this->rows.begin()+rows,word) >= 1)
		return true;
	if (count(this->cols.begin(),this->cols.begin()+cols,word) >= 1)
		return true;
/*
	if (count(this->rows_idx.begin(),this->rows_idx.begin()+rows,idx) >= 1)
		return true;
	if (count(this->cols_idx.begin(),this->cols_idx.begin()+cols,idx) >= 1)
		return true;
		*/
	return false;
}

string Grid::get_prefix_in_row(int row,int size)
{
	return this->rows[row].substr(0,size);
}

string Grid::get_prefix_in_column(int row,int size)
{
	return this->cols[row].substr(0,size);
}

void Grid::remove_word_in_row(int row)
{
    this->rows[row].assign(' ',word_size);
    this->rows_idx[row] = -1;
    for (int cpt=0 ; cpt < word_size ; cpt++)
    {
    	this->cols[cpt].resize(row);
    }
}

void Grid::put_word_in_row(const string & word, int idx, int row)
{
	this->rows[row] = word;
	this->rows_idx[row] = idx;
	for (int i = 0 ; i < word_size ; i++)
	{
		this->cols[i][row] = word[i];
	}
}

void Grid::put_word_in_col(const string & word,int idx, int col)
{
	this->cols[col] = word;
	this->cols_idx[col] = idx;
	for (int i = 0 ; i < word_size ; i++)
	{
		this->rows[i][col] = word[i];
	}
}

void Grid::print()
{
	cout << "================" << endl;
	for(int i = 0 ; i < int(rows.size()) ; i++)
	{
		const string & word = rows[i];
		cout << word << endl;
		/*
		for (int j = 0 ; j < rows.size() ; j++)
		{
			const string & col_word = cols[j];

			if (word[j] != col_word[i])
			{
				cout << "COHERENCE ERROR" << endl;
				cout << "row " << i << " = " << word << endl;
				cout << "col " << j << " = " << col_word << endl;
				exit(0);
			}
		}
		*/
	}
}

Grid::Grid(int word_size):rows(word_size),cols(word_size),rows_idx(word_size),cols_idx(word_size),word_size(word_size) {
	// TODO Auto-generated constructor stub
	const string empty_word(word_size,' ');
	for (int i = 0 ; i < word_size ; i++)
	{
		rows[i] = empty_word;
		cols[i] = empty_word;
	}

}

Grid::Grid() {
	// TODO Auto-generated constructor stub
}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}

