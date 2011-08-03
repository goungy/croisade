/*
 * Workitem.h
 *
 *  Created on: 3 d�c. 2010
 *      Author: goungy
 */

#ifndef WORKITEM_H_
#define WORKITEM_H_

class Workitem {
public:
	Workitem();
	Workitem(int idxBeg,int idxEnd,int possibilities);
	virtual ~Workitem();
	int idxBeg;
	int idxEnd;
	int possibilities;
	bool operator<(const Workitem & other) const;
};

#endif /* WORKITEM_H_ */
