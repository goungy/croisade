/*
 * Workitem.cpp
 *
 *  Created on: 3 d�c. 2010
 *      Author: goungy
 */

#include "Workitem.h"

Workitem::Workitem() {
	// TODO Auto-generated constructor stub

}
Workitem::Workitem(int idxBeg,int idxEnd,int possibilities):idxBeg(idxBeg),idxEnd(idxEnd),possibilities(possibilities)
{

}

Workitem::~Workitem() {
	// TODO Auto-generated destructor stub
}

bool Workitem::operator<(const Workitem & other) const
{
	return this->possibilities < other.possibilities;
}
