/*
 * TryOperation.h
 *
 *  Created on: 27 sept. 2012
 *      Author: jd219546
 */

#ifndef TRYOPERATION_H_
#define TRYOPERATION_H_

#include <iostream>
using namespace std;

class TryOperation
{
public:
	virtual void execute_try() = 0;
};

class SimpleRowWordTryerOperation : public TryOperation
{
public:
	SimpleRowWordTryerOperation(int start_idx, int last_idx, int row):
		start_idx(start_idx), last_idx(last_idx), row(row), current_idx(start_idx)
	{
		cout << "Creating a simple row word tryer operation : " << start_idx << " " << last_idx << " " << row << endl;
	}
	void execute_try()
	{
		cout << "Should try word " << current_idx << endl;
		current_idx++;
	}
private:
	int start_idx;
	int last_idx;
	int row;
	int current_idx;
};

#endif /* TRYOPERATION_H_ */
