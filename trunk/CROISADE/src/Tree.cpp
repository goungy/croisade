/*
 * Tree.cpp
 *
 *  Created on: 3 d�c. 2010
 *      Author: goungy
 */

#include "Tree.h"
#include <iostream>

using namespace std;

Tree::Tree() {
	// TODO Auto-generated constructor stub
	for (int i = 0 ; i < 26 ; i++)
		this->children[i] = NULL;
	this->isLeaf = 1;
	this->isWord = -1;
	this->nb_words_sub_tree = 0;
}

Tree * Tree::get_node(const char & letter) const
{
    int index = (int)(letter)- (int)('a');
    if (index>=0 && index <26)
    	return this->children[index];
    else
    	return 0;
}

void Tree::add_word_to_tree(int idx,const string & word,int deepness)
{
	//cout << deepness << " : " << word[deepness] << endl;
    int word_size = word.size();
    if (word_size == 0)
        return;
    this->nb_words_sub_tree++;
    this->isLeaf = 0;
    int letter_ord = ((int)word[deepness]) - ((int) 'a');
    if (this->children[letter_ord] == NULL)
    	this->children[letter_ord] = new Tree();
    Tree * subnode = this->children[letter_ord];
    if (deepness == word_size-1)
        subnode->isWord = idx;
    else
    {
    	//cout << subnode->isWord << endl;
        if (subnode->isWord == -1)
        {
            subnode->isWord = idx;
        }
        this->children[letter_ord]->add_word_to_tree( idx, word, deepness+1 );
    }

}
void Tree::create_tree(const vector<string> & words)
{
    for (int cpt = 0 ; cpt < int(words.size()) ; cpt++)
    {
    	const string & word = words.at(cpt);
    	//cout << "adding [" << word << "] to dictionnary" <<endl;
        this->add_word_to_tree(cpt,word,0);
    }
}

const Tree * Tree::nodeSearch(const string & prefix) const
{
    if (prefix.size() == 0)
        return NULL;
    const Tree * subnode = this;
    for (int i = 0 ; i < int(prefix.size()); i++)
    {
    	subnode = subnode->no_check_get_node(prefix[i]);
    	//subnode = subnode->get_node(prefix[i]);
    	if (subnode == NULL)
    		break;
    }
    return subnode;
}

int Tree::getPrefixFirstIndex(const string & prefix) const
{
    if (prefix.size() == 0)
        return -1;
    const Tree * subnode = this;
    for (int i = 0 ; i < int(prefix.size()); i++)
    {
    	subnode = subnode->get_node(prefix[i]);
    	if (subnode == NULL)
    		return -1;
    }
    return subnode->isWord;
}
Tree::~Tree() {
	// TODO Auto-generated destructor stub
/*
	cout << "Deleting Tree" << endl;
	for (int i = 0 ; i < 26 ; i++)
		if (this->children[i] != NULL)
			delete this->children[i];
*/
}
