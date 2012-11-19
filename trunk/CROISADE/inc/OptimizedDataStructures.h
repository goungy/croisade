/*
 * OptimizedDataStructures.h
 *
 *  Created on: 9 oct. 2012
 *      Author: jd219546
 */

#ifndef OPTIMIZEDDATASTRUCTURES_H_
#define OPTIMIZEDDATASTRUCTURES_H_
#include <iostream>
using namespace std;
class FixedLetter
{
public:
	FixedLetter(int pos, char letter):my_position(pos), my_letter(letter)
	{}
	int get_position() const { return my_position; }
	char get_letter() const { return my_letter;}
	void display() const
	{
		cout << "[" << get_position() << "," << get_letter() << "]" << endl;
	}
private:
	int my_position;
	char my_letter;
};

#include <vector>
using namespace std;

class FixedLettersSequence
{
public:
	FixedLettersSequence(const string & word, vector<int> positions)
	{
		for(auto it = positions.begin() ; it != positions.end() ; it++)
			add_fixed_letter(FixedLetter(*it, word[*it]));
	}
	void set_fixed_letter_at_sequence_index( const FixedLetter & fixed_letter, int index)
	{
		my_fixed_letters_sequence[index] = fixed_letter;
	}
	const FixedLetter & get_fixed_letter_from_sequence_index(int index) const
	{
		return my_fixed_letters_sequence[index];
	}
	void add_fixed_letter(const FixedLetter & fixed_letter)
	{
		my_fixed_letters_sequence.push_back(fixed_letter);
	}
	void remove_last_fixed_letter(){ my_fixed_letters_sequence.pop_back();}
	void display() const
	{
		for (auto it = my_fixed_letters_sequence.begin() ; it != my_fixed_letters_sequence.end() ; it++)
			it->display();
	}
private:
	vector<FixedLetter> my_fixed_letters_sequence;
};

class FixedLettersNode
{
public:

private:
	vector< vector < FixedLettersNode *> > my_nodes;
	vector<int> word_indexes;
};

class FixedLettersSequenceBuilder
{
public:
	FixedLettersSequenceBuilder(int max_fixed_letters, int word_size):
		word_size(word_size),max_fixed_letters(max_fixed_letters),
		internal_state(max_fixed_letters,0)
	{
		this->reset();
		max_indexes.resize(max_fixed_letters, 0);
		for (unsigned int i = 0 ; i < max_indexes.size() ; i++)
			max_indexes[i] = word_size - max_fixed_letters + i;
	}

	void reset()
	{
		for (int i = 0 ; i < max_fixed_letters ; i++)
			internal_state[i] = i;
		internal_state[max_fixed_letters-1]--;
	}

	void find_next(int index_to_increase)
	{
		if (index_to_increase < 0)
			return;
		if(internal_state[index_to_increase] < (word_size-1))
		{
			if (internal_state[index_to_increase]+1 > max_indexes[index_to_increase])
				find_next(index_to_increase-1);

			internal_state[index_to_increase]++;
			for (int i = index_to_increase + 1 ; i < max_fixed_letters ; i++)
			{
				internal_state[i] = internal_state[i-1] + 1;
				if (internal_state[i] == word_size)
					return;
			}
		}
		else
			find_next(index_to_increase-1);
	}

	bool has_next() const
	{
		for (unsigned int i = 0 ; i < internal_state.size() ; i++)
			if (internal_state[i] != max_indexes[i])
				return true;
		return false;
	}

	vector<int> get_next()
	{
		find_next(max_fixed_letters-1);
		return internal_state;
	}
private:
	int word_size;
	int max_fixed_letters;
	vector<int> internal_state;
	vector<int> max_indexes;
};

class FixedLettersRootNode
{
public:
	FixedLettersRootNode(const Dictionnary & dico, int fixed_letters):
		fixed_sequence_builder(fixed_letters, dico.wordsNumber_)
	{
		add_word_to_tree(dico.get_word_from_index(0),0);
		/*
		for (int i = 0 ; i < dico.wordsNumber_ ; i++)
			add_word_to_tree(dico.get_word_from_index(i),i);
			*/
	}
	void add_word_to_tree(const string & word, int word_index)
	{
		fixed_sequence_builder.reset();
		//while (fixed_sequence_builder.has_next())
		{
			add_word_to_tree(word, word_index, fixed_sequence_builder.get_next());
			add_word_to_tree(word, word_index, fixed_sequence_builder.get_next());
		}
	}
private:
	void add_word_to_tree(const string & word, int word_index, const vector<int> & fixed_letters_position)
	{
		FixedLettersSequence fixed_sequence(word, fixed_letters_position);
		cout << "Word & idx : " << word << " " << word_index << endl;
		fixed_sequence.display();
	}
	FixedLettersSequenceBuilder fixed_sequence_builder;
};

#endif /* OPTIMIZEDDATASTRUCTURES_H_ */
