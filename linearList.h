#ifndef _LINEAR_LIST_H
#define _LINEAR_LIST_H

template<class T>
class linearList
{
public:
	virtual ~linearList() {};
	virtual bool empty() const = 0;
	virtual int size() const = 0;
	virtual T& get(int index) const = 0;
	virtual int indexof(const T& theElement) const = 0;
	virtual void erase(int index) = 0;
	virtual void insert(int index, const T& theElement) = 0;
	virtual void output(ostream& os) const = 0;
};

#endif