/*
 * Grid.h
 *
 *  Created on: 25 nov. 2010
 *      Author: goungy
 */

#ifndef GRID_H_
#define GRID_H_
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Grid {
public:
	Grid();
	Grid(int word_size);
	virtual ~Grid();
	bool check_no_doublons_in_grid() const
	{
		bool no_doublon = true;
		cout << "CHECK NO DOUBLONS" << endl;
		for (int i = 0 ; i < int(get_word_size()) && no_doublon; i++)
		{
			cout << "For i = " << i << endl;
			const string & word = cols[i];
			cout << "Checking " << word << endl;
			no_doublon = !this->word_already_in_grid(get_word_size(), get_word_size(), word , cols_idx[i]);
		}
		return no_doublon;
	}
	bool word_already_in_grid(int rows, int cols,const string & word, int idx) const;
	void put_word_in_row(const string & word,int idx, int row);
	void put_word_in_col(const string & word,int idx, int col);
	void remove_word_in_row(int row);
	string get_prefix_in_column(int row,int col);
	int get_word_index_in_column(int col){return cols_idx[col];}
	int get_word_index_in_row(int row){return rows_idx[row];}
	string get_prefix_in_row(int row,int col);
	int get_word_size() const{return word_size;}
	void print();

	vector<string> rows;
	vector<string> cols;
	vector<int> rows_idx;
	vector<int> cols_idx;
	int word_size;
};

#endif /* GRID_H_ */
