#pragma once
#include <stdio.h>
#include <iostream>

template <class T> class RBTree_Node
{
public:
	RBTree_Node * l, *r, *dad;
	int color;
	T key;
	RBTree_Node() : l(0), r(0), dad(0), color(0) {}
};

template <class T> class RBTree
{
private:
	RBTree_Node<T> *root;
public:
	RBTree() : root(NULL) {}
	RBTree(const T &x);
	~RBTree() { if (root) clean(root); }
	void clean(RBTree_Node<T> *n);
	void add(const T &x);
	void put(const T &x, RBTree_Node<T> *n);
	void balance(RBTree_Node<T> *n);
	void rotateleft(RBTree_Node<T> *n);
	void rotateright(RBTree_Node<T> *n);
	void del(RBTree_Node<T> *n);
	void delfix(RBTree_Node<T> *n);
	RBTree_Node<T>* findgrst(RBTree_Node<T> *n);
	RBTree_Node<T>* search(const T &x);
	RBTree_Node<T>* find(const T &x, RBTree_Node<T> *n);
	void print(RBTree_Node<T> *);
	void print() { print(root); }
};
