#ifndef _DOUBLY_LINKED_LIST_H
#define _DOUBLY_LINKED_LIST_H

template <class T>
class doublyLinkedList;

template<class T>
class myListIterator
{
	friend class doublyLinkedList<T>;
public:
	myListIterator(dlChainNode<T>* theNode = NULL){
		node = theNode;
	}
	
	//解引用
	T& operator* () const{
		return node->element;
	}
	T* operator->() const{
		return &node->element;
	}

	//迭代加
	myListIterator& operator++() 
	{ 
		node = node->next;
		return *this;
	}
	//迭代减
	myListIterator& operator--() 
	{ 
		node = node->prev;
		return *this;
	}
	myListIterator operator+(int dist){
		dlChainNode<T>* cur = node;
		while(dist-- > 0)
		{	
			cur = cur->next;
		}
		return myListIterator(cur);
	}
	myListIterator operator-(int dist){
		dlChainNode<T>* cur = node;
		while(dist-- > 0)
		{	
			cur = cur->prev;
		}
		return myListIterator(cur);	
	}


	//相等校验
	bool operator==(const myListIterator& iter) const
	{
		return node == iter.node;
	}

	bool operator!=(const myListIterator& iter) const
	{
		return node != iter.node;
	}

protected:
	dlChainNode<T>* node;
};

//带有头节点的双向循环链表
template<class T>
class doublyLinkedList: public linearList<T> 
{
public:
	typedef myListIterator<T> iterator;
	doublyLinkedList(int capacity){
		listSize = 0;
		headNode = new dlChainNode<T>();
		headNode->next = headNode;
		headNode->prev = headNode;
	}
	 ~doublyLinkedList() {
		dlChainNode<T>* cur = headNode->next;
		while(listSize-- > 0)
		{
			dlChainNode<T>* next = cur->next;
			delete[] cur;
			cur = next;
		}
		delete[] headNode;
	 }
	bool empty() const{
		return listSize == 0;
	}
	int size() const{
		return listSize;
	}
	T& get(int index) const{
        checkIndex(index);
        dlChainNode<T>* node = getNodeByIndex(index);
        return node->element;
	}
	int indexof(const T& theElement) const;
	void erase(int index);
	void insert(int index, const T& theElement);
	void output(ostream& os) const;
	
	T& operator[](int index) const;
	iterator begin(){
		return iterator(headNode->next);
	}
	iterator end(){
		return iterator(headNode);
	}
	iterator left(const iterator& iter){
		if(listSize == 0)
		{
			ostringstream s;
			s << "listSize must be > 0" << endl;
			throw illegalParameterValue(s.str());
		}
		dlChainNode<T>* cur = iter.node->prev;
		if(cur == headNode) cur = cur->prev;
		return iterator(cur);
	}
	iterator right(const iterator& iter){
		if(listSize == 0)
		{
			ostringstream s;
			s << "listSize must be > 0" << endl;
			throw illegalParameterValue(s.str());
		}
		dlChainNode<T>* cur = iter.node->next;
		if(cur == headNode) cur = cur->next;
		return iterator(cur);
	}
	void erase(iterator& iter){
		iterator deletePos = iter;
		dlChainNode<T>* deleteNode = iter.node;
		dlChainNode<T>* prevNode = deleteNode->prev;
		++iter;
		//删除之前的节点
		prevNode->next = deleteNode->next;
		prevNode->next->prev = prevNode;
		listSize--;
		delete deleteNode;
	}
private:
	void checkIndex(int index) const;
	dlChainNode<T>* getNodeByIndex (int index) const{
		if(index == -1 || index == listSize)
			return headNode;
		dlChainNode<T>* node;
        int count;
        if(index <= listSize / 2)//从头部搜索
        {
            count = index;
            node = headNode->next;
            while(count-- > 0)
            {
                node = node->next;
            }
            return node;
        }
        else//从尾部搜索
        {
            count = listSize - 1 - index;
            node = headNode->prev;
            while(count--)
            {
                node = node->prev;
            }
            return node;
        }
	}
	dlChainNode<T> *headNode;//头节点
	int listSize;
};

template<class T>
int doublyLinkedList<T>::indexof(const T& theElement) const
{
	int index = 0;
	dlChainNode<T> *cur = headNode->next;
	while(index < listSize)
	{
		if(cur->element == theElement)
			return index;
		cur = cur->next;
		index++;
	}
	return -1;
}

template<class T>
void doublyLinkedList<T>::erase(int index)
{
	checkIndex(index);
	dlChainNode<T>* prevNode = getNodeByIndex(index - 1);
	dlChainNode<T>* deleteNode = prevNode->next;
	prevNode->next = deleteNode->next;
	prevNode->next->prev = prevNode;
	listSize--;
	delete deleteNode;
}

template<class T>
void doublyLinkedList<T>::insert(int index, const T& theElement)
{
	if(index < 0 || index > listSize)
	{
		ostringstream s;
		s << "index = " << index << " size = " << listSize;
        throw illegalIndex(s.str());
	}
	dlChainNode<T>* prevNode = getNodeByIndex(index - 1);
	dlChainNode<T>* newNode = new dlChainNode<T>(theElement, prevNode, prevNode->next);	
	listSize++;
}

template<class T>
void doublyLinkedList<T>::output(ostream& out) const
{
	dlChainNode<T> *node = headNode->prev;
	while(node != headNode)
	{
		out << " " << node->element;
		node = node->prev;
	}
	out << endl;
}

template<class T>
void doublyLinkedList<T>::checkIndex(int index) const
{
	if(index < 0 || index >= listSize)
	{
		ostringstream s;
		s << "index = " << index << " size = " << listSize;
        throw illegalIndex(s.str());
	}
}

template<class T>
T& doublyLinkedList<T>::operator[](int index) const
{
	checkIndex(index);
	return get(index);
}

#endif