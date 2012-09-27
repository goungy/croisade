/*
 * TryManager.h
 *
 *  Created on: 27 sept. 2012
 *      Author: jd219546
 */

#ifndef TRYMANAGER_H_
#define TRYMANAGER_H_

#include <vector>
using namespace std;

class TryManager
{
public:
	void execute_all_tries()
	{
		for (auto it = this->try_operations.begin() ; it != this->try_operations.end() ; it++)
			(*it)->execute_try();
	}
	TryOperation * get_operation(int idx) const { return try_operations[idx];}
protected:
	vector< TryOperation *> try_operations;
};

class SimpleRowWordPlacerTrier : public TryManager
{
public:
	SimpleRowWordPlacerTrier(int start_idx, int last_idx, int row)
	{
		this->try_operations.push_back(new SimpleRowWordTryerOperation(start_idx, last_idx, row));
	}
};


#endif /* TRYMANAGER_H_ */
