/*
 * Dictionnary.cpp
 *
 *  Created on: 25 nov. 2010
 *      Author: goungy
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Dictionnary.h"

Dictionnary::Dictionnary() {
	// TODO Auto-generated constructor stub

}
Dictionnary::Dictionnary(string filename, int word_size)
{
	wordsNumber_ = 0 ;
	wordsSize_ = word_size ;
	this->readfile(filename);
	sort(this->words_.begin(),this->words_.end());
	this->tree.create_tree(this->words_);
	this->shared_prefix.resize(this->wordsNumber_);
	for (unsigned int i = 0 ; i < this->wordsNumber_ ;i++)
		this->shared_prefix[i].assign(word_size,-1);

	this->analyze_shared_prefix();
/*
	for (unsigned int i = 0 ; i < this->wordsNumber_ ;i++)
	{
		cout <<"["<< i <<"] " << this->words_[i] << " : " ;
		for (unsigned int j = 0 ; j < word_size ; j++)
		//if (this->shared_prefix[i] != -1)
		{
			cout << this->shared_prefix[i][j] << " ";
		}
		cout << endl;
	}

	cout << "OK" << endl;
	*/
	//exit(0);
}

void Dictionnary::analyze_shared_prefix()
{
//REMARK : for last word, everything is set @ -1.
	for (unsigned int i = 0 ; i < this->wordsNumber_-1 ; i++)
	{
		const string &word1 = this->words_[i];
		unsigned int idx = i + 1;
		unsigned int diff = 0;
		unsigned int last_diff = wordsSize_;
		while (idx < this->wordsNumber_ && last_diff > 0)
		{
			diff = 0;
			const string &word2 = this->words_[idx];
			while (diff < wordsSize_)
				if(word2[diff] != word1[diff])
					break;
				else
					diff++;

			for(unsigned int j = diff ; j < last_diff ; j++)
			{
				//cout << word1 << " has " << word2 << " as next diff for " << j << endl;
				this->shared_prefix[i][j] = idx;
			}
			last_diff = diff;
			idx++;
		}

	}
}

inline int Dictionnary::linearSearch(const string & prefix) const
{
	int index = -1;
	unsigned int current = 0;
	//cout << "Trying to find index of prefix : " << prefix << endl;
	while (current < this->wordsNumber_)
	{
		if (prefix <= this->words_[current])
			break;
		current++;
	}
	if (current < this->wordsNumber_)
	{
		string other_prefix = this->words_[current].substr(0,prefix.size());
		if (other_prefix == prefix)
			index = current;
	}
	return index;
}

inline int Dictionnary::binaryTreeSearch(const string & prefix) const
{
	//cout << "Searchin in Tree " << endl;
	return this->tree.getPrefixFirstIndex(prefix);
}

inline int Dictionnary::binarySearch(const string & prefix) const
{
    int range = this->wordsNumber_ / 2;
    int idx = range;
    bool not_found = true;
    while (not_found and range >= 1){
        range /= 2;
        const string & word = this->words_[idx];
        if (prefix > word)
            idx += range;
        else if (prefix < word)
            idx -= range;
        else
            not_found = false;
    }
    if (not_found)
    {
        while (prefix < this->words_[idx] && idx > 0)
		{
        	idx -= 1;
		}
        while (prefix > this->words_[idx] && idx < (this->wordsNumber_-1))
		{
            idx += 1;
		}
    }
    string other_prefix = this->words_[idx].substr(0,prefix.size());
    if (prefix != other_prefix)
    	idx = -1;
    return idx;
}

const Tree * Dictionnary::findFirstNodeOfPrefix(const string & prefix) const
{
	return tree.nodeSearch(prefix);
}

int Dictionnary::findFirstIndexOfPrefix(const string & prefix) const
{
	//int index = linearSearch(prefix);
	//int index = binarySearch(prefix);
	int index = binaryTreeSearch(prefix);
	/*
	if (index != index2)
	{
		cout << "index / index2 " << index << "/" << index2 << endl;
		exit(0);
	}
	*/
	return index;
}

void Dictionnary::str()
{
	for (unsigned int i = 0 ; i < this->words_.size() ; i++)
		cout << i << " : " << this->words_[i] << endl;
}

void Dictionnary::readfile(string filename)
{

	// Charge le fichier dictionnaire et remplit le vecteur words avec les mots de longueur wordSize
	ifstream DICT ;

	string full_filename = "dicts/" + filename;
	//cout << "Loading file : " << full_filename << "..." << endl ;
	DICT.open( full_filename.c_str(), ifstream::in ) ;
	if ( DICT.fail() )
	{
		cerr << "Error : unable to open " << filename << endl ;
		exit(1);
	}

	string word;
	while( getline( DICT, word ) )
	{
		if ( word.size() == wordsSize_ )
		{
			wordsNumber_ += 1 ;
			std::transform(word.begin(), word.end(),word.begin(), ::tolower);
			words_.push_back( word ) ;
		}
	}
	//cout << " containing : " << wordsNumber_ << " " << wordsSize_ << "-letters words" << endl ;


	if ( wordsNumber_ == 0 )
	{
		cout << "Empty dictionnary : stop." << endl ;
		exit(0) ;
	}


	DICT.close() ;
}

Dictionnary::~Dictionnary() {
	// TODO Auto-generated destructor stub
}
