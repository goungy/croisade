/*
 * Dictionnary.h
 *
 *  Created on: 25 nov. 2010
 *      Author: goungy
 */

#ifndef DICTIONNARY_H_
#define DICTIONNARY_H_


#include <vector>
#include <string>
#include "Tree.h"

using namespace std;

class Dictionnary {
public:
	Dictionnary();
	Dictionnary(string filename, int word_size);
	void readfile(string filename);
	void str();

	int findFirstIndexOfPrefix(const string & prefix) const;
	const Tree * findFirstNodeOfPrefix(const string & prefix) const;
	inline int linearSearch(const string & prefix) const;
	inline int binarySearch(const string & prefix) const;
	inline int binaryTreeSearch(const string & prefix) const;
	void analyze_shared_prefix();

	virtual ~Dictionnary();

	vector<string> words_;
	vector<vector<int> > shared_prefix;
	unsigned int wordsSize_;
	unsigned int wordsNumber_;
	Tree tree;
};

#endif /* DICTIONNARY_H_ */
