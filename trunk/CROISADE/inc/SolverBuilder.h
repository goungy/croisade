#ifndef SOLVERBUILDER
#define SOLVERBUILDER



#include <vector>
using namespace std;

#include "Dictionnary.h"
#include "Solver.h"

class ISolverBuilder
{
public:
	virtual ~ISolverBuilder(){}
	virtual void read_dictionary(int word_size, const string & filepath) = 0;
	virtual void check_code_version(double version) const = 0;
	virtual double get_my_code_version() const = 0;
	virtual void create_solver(const Parameters & parameters) = 0;
	virtual ISolver &get_solver() = 0;
};

class BaseSolverBuilder : public ISolverBuilder
{
public:
	BaseSolverBuilder(double code_version):my_dico(0),my_code_version(code_version), my_solver(0)
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
		const Dictionnary * my_dico = this->get_my_dictionnary();

		this->my_solver = new StephaneSolver(this->get_my_dictionnary(), parameters);
	}
private:

};

class ArthurSolverBuilder : public BaseSolverBuilder
{
public:
	ArthurSolverBuilder():BaseSolverBuilder(0.1)
	{

	}

	void create_solver(const Parameters & parameters)
	{

		this->my_solver = new ArthurSolver(this->get_my_dictionnary(), parameters);
	}
private:

};
#endif
