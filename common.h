#ifndef _COMMON_H
#define _COMMON_H

//exception class
class illegalParameterValue
{
public:
	illegalParameterValue(string s):message(s){};
private:
	string message;
};

class illegalIndex
{
public:
	illegalIndex(string s):message(s){};
private:
	string message;
};

class stackEmpty
{
public:
	stackEmpty(){};
};

class queueEmpty
{
public:
	queueEmpty(){};
};


template <class T>
void changeLength1D(T* &element, int oldLength, int newLength)
{
	if(newLength < 0)
	{
		throw illegalParameterValue("new length must be > 0");
	}
	T *newElement = new T[newLength];
	int copyLength = min(oldLength, newLength);
	copy(element, element + copyLength, newElement);
	delete[] element;
	element = newElement;
}


template<class T>
struct chainNode
{
	chainNode(const T& element){
		this->element = element;
		next = NULL;
	}
	chainNode(const T& element, chainNode<T>* next){
		this->element = element;
		this->next = next;
	}
	T element;
	chainNode<T> * next;
};

template<class T>
struct dlChainNode
{
	dlChainNode(){
		next = NULL;
		prev = NULL;
	}
	dlChainNode(const T& element){
		this->element = element;
		next = NULL;
		prev = NULL;
	}
	dlChainNode(const T& element, dlChainNode<T>* prev, dlChainNode<T>* next){
		this->element = element;
		this->next = next;
		this->prev = prev;
		prev->next = this;
		next->prev = this;
	}
	T element;
	dlChainNode<T>* next;
	dlChainNode<T>* prev;
};


#include<sys/time.h>
inline double seconds()
{
    struct timeval tp;
    struct timezone tzp;
    int i = gettimeofday(&tp, &tzp);
    return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}

#endif