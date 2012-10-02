/*
 * SolverMediator.h
 *
 *  Created on: 1 oct. 2012
 *      Author: jd219546
 */

#ifndef SOLVERMEDIATOR_H_
#define SOLVERMEDIATOR_H_

#include <vector>
using namespace std;

#include "SolverStage.h"

class SimpleRowWordPlacerStage;
class SimpleRowWordTryAndCheckStage;
class ColWordPrefixTryStage;
class ColWordPrefixTryAndCheckStage;
class GridCheckerStage;

class ISolverMediator
{
public:
	virtual ~ISolverMediator(){}
	virtual void notify_stage_finished(const SimpleRowWordPlacerStage * solver_stage) = 0;
	virtual void notify_stage_finished(const SimpleRowWordTryAndCheckStage * solver_stage) = 0;
	virtual void notify_stage_finished(const ColWordPrefixTryStage * solver_stage) = 0;
	virtual void notify_stage_finished(const ColWordPrefixTryAndCheckStage * solver_stage) = 0;

	virtual void register_stage(SimpleRowWordPlacerStage * stage)=0;
	virtual void register_stage(SimpleRowWordTryAndCheckStage * stage)=0;
	virtual void register_stage(ColWordPrefixTryStage * stage) =0;
	virtual void register_stage(ColWordPrefixTryAndCheckStage * stage)=0;
};

class StephaneSolverMediator : public ISolverMediator
{
public:
	StephaneSolverMediator(int prefix_size, int word_size);
	void notify_stage_finished(const SimpleRowWordPlacerStage * solver_stage);
	void notify_stage_finished(const SimpleRowWordTryAndCheckStage * solver_stage);
	void notify_stage_finished(const ColWordPrefixTryStage * solver_stage);
	void notify_stage_finished(const ColWordPrefixTryAndCheckStage * solver_stage);
	void register_stage(SimpleRowWordPlacerStage * stage);
	void register_stage(SimpleRowWordTryAndCheckStage * stage);
	void register_stage(ColWordPrefixTryStage * stage);
	void register_stage(ColWordPrefixTryAndCheckStage * stage);
	void register_stage(GridCheckerStage * stage)
	{
		this->check_stage = stage;
	}

private:
	SimpleRowWordPlacerStage * first_row_stage;
	vector <SimpleRowWordTryAndCheckStage *> second_to_k_row_stages;
	ColWordPrefixTryStage * first_col_stage;
	vector <ColWordPrefixTryAndCheckStage *> col_stages;
	GridCheckerStage * check_stage;
};


#endif /* SOLVERMEDIATOR_H_ */
