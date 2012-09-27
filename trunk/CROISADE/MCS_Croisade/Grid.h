/*
 * Grid.h
 *
 *  Created on: 25 nov. 2010
 *      Author: goungy
 */

#ifndef GRID_H_
#define GRID_H_
#include <string>
#include <vector>

using namespace std;
class Grid {
public:
	Grid();
	Grid(int word_size);
	virtual ~Grid();

	bool word_already_in_grid(int rows, int cols,const string & word, int idx);
	void put_word_in_row(const string & word,int idx, int row);
	void put_word_in_col(const string & word,int idx, int col);
	void remove_word_in_row(int row);
	string get_prefix_in_column(int row,int col);
	string get_prefix_in_row(int row,int col);
	void print();

	vector<string> rows;
	vector<string> cols;
	vector<int> rows_idx;
	vector<int> cols_idx;
	int word_size;
};


inline void Grid::put_word_in_row(const string & word, int idx, int row)
{
	this->rows[row] = word;
	this->rows_idx[row] = idx;
	for (int i = 0 ; i < word_size ; i++)
	{
		this->cols[i][row] = word[i];
	}
}

inline void Grid::put_word_in_col(const string & word,int idx, int col)
{
	this->cols[col] = word;
	this->cols_idx[col] = idx;
	for (int i = 0 ; i < word_size ; i++)
	{
		this->rows[i][col] = word[i];
	}
}

#endif /* GRID_H_ */
