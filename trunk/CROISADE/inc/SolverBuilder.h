#ifndef SOLVERBUILDER
#define SOLVERBUILDER

#include "Dictionnary.h"

#include <vector>
using namespace std;
class ISolver
{
public:
	virtual int get_word_size() const = 0;
	virtual void execute_solver() = 0;
};


class CheckManager
{

};

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


class SolverStage
{
public:
	SolverStage():check_manager(0), try_manager(0), next_stage(0)
	{}
	virtual void execute_stage() = 0;
	void link_to_next_stage(SolverStage * stage){ this->next_stage = stage;}
protected:
	CheckManager * check_manager;
	TryManager * try_manager;
	SolverStage * next_stage;
};

class DummyStage : public SolverStage
{
public:
	void execute_stage()
	{
		//cout << "Dummy stage doing nothing" << endl;
	}
};

class SimpleRowWordPlacerStage : public SolverStage
{
public:
	SimpleRowWordPlacerStage(int start_idx, int last_idx, int row):
		start_idx(start_idx), last_idx(last_idx), row(row)
	{

	}
	void execute_stage()
	{
		for(int idx = start_idx ; idx <= last_idx ; idx++)
		{
			if (row == 0) cout << "Placing word " << idx << " in row " << row << endl;
			next_stage->execute_stage();
		}
	}
private:
	int start_idx;
	int last_idx;
	int row;
};

class BaseSolver : public ISolver
{
public:
	BaseSolver(const Dictionnary * dico):my_dico_ptr(dico)
	{}
	int get_word_size() const {return my_dico_ptr->wordsSize_;}
	virtual void execute_solver() = 0;
protected:
	void add_stage(SolverStage* new_stage)
	{
		this->solver_stages.push_back(new_stage);
	}
	void link_solver_stages()
	{
		for(auto it = solver_stages.begin() ; it != (solver_stages.end()-1) ; it++)
			(*it)->link_to_next_stage(*(it+1));
		solver_stages.back()->link_to_next_stage(new DummyStage());
	}
	const Dictionnary * my_dico_ptr;
	vector< SolverStage *> solver_stages;
};

class StephaneSolver : public BaseSolver
{
public:
	StephaneSolver(const Dictionnary * dico, const Parameters & parameters)
	: BaseSolver(dico)
	{
		my_prefix_max_size = atoi(parameters.get_parameter("saved_solver_state_prefix_size").c_str());
		this->check_max_prefix_size();
		int start_idx = 0;
		int last_idx = my_dico_ptr->wordsNumber_-1;
		for (int i = 0 ; i < my_prefix_max_size ; i++)
			this->add_stage(new SimpleRowWordPlacerStage(start_idx, last_idx, i));
		this->link_solver_stages();
	}
	void execute_solver()
	{
		cout << "There are " << this->solver_stages.size() << " solver stages" << endl;
		solver_stages[0]->execute_stage();
	}
protected:
private:
	void check_max_prefix_size() const
	{
		if (my_prefix_max_size > this->get_word_size())
			throw "Asking for a max prefix size larger than word size in Stephane solver";
	}

	int my_prefix_max_size;
};




class ISolverBuilder
{
public:
	virtual void read_dictionary(int word_size, const string & filepath) = 0;
	virtual void check_code_version(double version) const = 0;
	virtual double get_my_code_version() const = 0;
	virtual void create_solver(const Parameters & parameters) = 0;
	virtual ISolver &get_solver() = 0;
};

class BaseSolverBuilder : public ISolverBuilder
{
public:
	BaseSolverBuilder(double code_version):my_code_version(code_version)
	{

	}
	void read_dictionary(int word_size, const string & filepath)
	{
		my_dico = new Dictionnary(filepath, word_size);
	}
	void check_code_version(double version) const
	{
		if (version != this->get_my_code_version())
		{
			cout << "Required version vs code version : " << version << " vs " << this->get_my_code_version() << endl;
			throw "Version of the solver code mismatches required version from input file";
		}
	}
	double get_my_code_version() const { return my_code_version;}
	int get_word_size() const { return my_dico->wordsSize_;}
	Dictionnary * get_my_dictionnary() const { return my_dico; }
	virtual void create_solver(const Parameters & parameters) = 0;
	ISolver& get_solver(){return *my_solver;}
private:
	Dictionnary * my_dico;

protected:
	double my_code_version;
	ISolver * my_solver;

};

class StephaneSolverBuilder : public BaseSolverBuilder
{
public:
	StephaneSolverBuilder():BaseSolverBuilder(0.1)
	{

	}

	void create_solver(const Parameters & parameters)
	{
		this->my_solver = new StephaneSolver(this->get_my_dictionnary(), parameters);
	}
private:

};

class SolverDirector
{
public:
	SolverDirector(const string & filepath);
	ISolver & build_solver();
private:
	Parameters my_parameters;
	ISolverBuilder * my_solver_builder;
	string my_solver_type;
};

SolverDirector::SolverDirector(const string & filepath)
{
	my_parameters.read_parameters_from_txt_file(filepath);
	my_solver_type = my_parameters.get_parameter("solver_type");
	if (my_solver_type == "Stephane")
		my_solver_builder = new StephaneSolverBuilder();
}


ISolver & SolverDirector::build_solver()
{
	my_solver_builder->read_dictionary(
			my_parameters.get_word_size(),
			my_parameters.get_dictionary_file());
	my_solver_builder->check_code_version(
			atof(my_parameters.get_parameter("saved_solver_state_version").c_str()));
	my_solver_builder->create_solver(my_parameters);
	return my_solver_builder->get_solver();
}

#endif
