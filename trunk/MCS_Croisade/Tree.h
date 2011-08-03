/*
 * Tree.h
 *
 *  Created on: 3 d�c. 2010
 *      Author: goungy
 */

#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <string>

using namespace std;

class Tree {
public:
	Tree();
	virtual ~Tree();

	int nb_words_sub_tree;
    int isWord;
    int isLeaf;
    Tree * children[26];
    void create_tree(const vector<string> & words);
    void add_word_to_tree(int idx,const string & word,int deepness=0);
    Tree * get_node(const char & letter) const;
    const Tree * nodeSearch(const string & prefix) const;
    int getPrefixFirstIndex(const string & prefix) const;
};

#endif /* TREE_H_ */
