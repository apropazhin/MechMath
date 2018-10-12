#include "RBTree.h"

template <class T>
RBTree<T>::RBTree(const T &x)
{
	root = new RBTree_Node<T>;
	root->key = x;
}

template <class T>
void RBTree<T>::clean(RBTree_Node<T> *n)
{
	if (n->l)
	{
		clean(n->l);
	}
	if (n->r)
	{
		clean(n->r);
	}
	delete n;
}

template <class T>
void RBTree<T>::add(const T &x)
{
	if (!root)
	{
		root = new RBTree_Node<T>;
		root->key = x;
	}
	else
	{
		put(x, root);
	}
}

template <class T>
void RBTree<T>::put(const T &x, RBTree_Node<T> *n)
{
	if (n->key > x)
	{
		if (!n->l)
		{
			n->l = new RBTree_Node<T>;
			n->l->color = 1;
			n->l->key = x;
			n->l->dad = n;
			balance(n->l);
		}
		else
		{
			put(x, n->l);
		}
	}
	else
	{
		if (!n->r)
		{
			n->r = new RBTree_Node<T>;
			n->r->color = 1;
			n->r->key = x;
			n->r->dad = n;
			balance(n->r);
		}
		else
		{
			put(x, n->r);
		}		
	}
}

template <class T>
void RBTree<T>::balance(RBTree_Node<T> *n)
{
	if (!n->dad)
	{
		n->color = 0;
		return;
	}
	if (n->dad->color == 0)
	{
		return;
	}
	if ((n->dad->dad->color == 0) &&
		(n->dad == n->dad->dad->l) && (n->dad->dad->r) && (n->dad->dad->r->color == 1))
	{
		n->dad->dad->color = 1;
		n->dad->dad->r->color = 0;
		n->dad->color = 0;
		balance(n->dad->dad);
		return;
	}
	if ((n->dad->dad->color == 0) &&
		(n->dad == n->dad->dad->r) && ((n->dad->dad->l))
		&& (n->dad->dad->l->color == 1))
	{
		n->dad->dad->color = 1;
		n->dad->dad->l->color = 0;
		n->dad->color = 0;
		balance(n->dad->dad);
		return;
	}
	if ((n == n->dad->r) && (n->dad == n->dad->dad->l))
	{
		rotateleft(n->dad);
		balance(n->l);
		return;
	}
	if ((n == n->dad->l) && (n->dad == n->dad->dad->r))
	{
		rotateright(n->dad);
		balance(n->r);
		return;
	}
	if ((n == n->dad->l) && (n->dad->dad->l == n->dad))
	{
		n->dad->color = 0;
		n->dad->dad->color = 1;
		rotateright(n->dad->dad);
		return;
	}
	if ((n == n->dad->r) && (n->dad->dad->r == n->dad))
	{
		n->dad->color = 0;
		n->dad->dad->color = 1;
		rotateleft(n->dad->dad);
		return;
	}
}

template <class T>
void RBTree<T>::rotateleft(RBTree_Node<T> *n)
{
	RBTree_Node<T> *p = n->r;
	p->dad = n->dad;
	if (n->dad)
	{
		if (n->dad->l == n)
		{
			n->dad->l = p;
		}
		else
		{
			n->dad->r = p;
		}
	}
	else
	{
		root = p;
	}
	n->r = p->l;
	if (p->l)
	{
		p->l->dad = n;
	}
	n->dad = p;
	p->l = n;
}

template <class T>
void RBTree<T>::rotateright(RBTree_Node<T> *n)
{
	RBTree_Node<T> *p = n->l;
	p->dad = n->dad;
	if (n->dad)
	{
		if (n->dad->l == n)
		{
			n->dad->l = p;
		}
		else
		{
			n->dad->r = p;
		}
	}
	else
	{
		root = p;
	}
	n->l = p->r;
	if (p->r)
	{
		p->r->dad = n;
	}
	n->dad = p;
	p->r = n;
}

template <class T>
void RBTree<T>::del(RBTree_Node<T> *n)
{
	if ((n->l) && (n->r))
	{
		RBTree_Node<T> *t = findgrst(n->l);
		n->key = t->key;
		del(t);
		return;
	}
	if (n->color == 1)
	{
		RBTree_Node<T> *tmp = n;
		if ((n->r) || (n->l))
		{ 
			if (n->r)
			{
				if (n == n->dad->r)
				{
					n->dad->r = n->r;
					n->r->dad = n->dad;
				}
				else
				{
					n->dad->l = n->r;
					n->r->dad = n->dad;
				}
			}
			else
			{
				if (n == n->dad->r)
				{
					n->dad->r = n->l;
					n->l->dad = n->dad;
				}
				else
				{
					n->dad->l = n->l;
					n->l->dad = n->dad;
				}
			}
		}
		else
		{
			if (n == n->dad->r)
			{
				n->dad->r = 0;
			}
			else
			{
				n->dad->l = 0;
			}
		}
		delete tmp;
		return;
	}
	if ((n->color == 0) && (n->r) && (n->r->color == 1))
	{
		n->r->color = 0;
		RBTree_Node<T> *tmp = n;
		if (n->dad)
		{
			if (n == n->dad->r)
			{
				n->dad->r = n->r;
				n->r->dad = n->dad;
			}
			else
			{
				n->dad->l = n->r;
				n->r->dad = n->dad;
			}
		}
		else
		{
			root = n->r;
			n->r->dad = 0;
		}
		delete tmp;
		return;
	}
	if ((n->color == 0) && (n->l) && (n->l->color == 1))
	{
		n->l->color = 0;
		RBTree_Node<T> *tmp = n;
		if (n->dad)
		{
			if (n == n->dad->r)
			{
				n->dad->r = n->l;
				n->l->dad = n->dad;
			}
			else
			{
				n->dad->l = n->l;
				n->l->dad = n->dad;
			}
		}
		else
		{
			root = n->l;
			n->l->dad = 0;
		}
		delete tmp;
		return;
	}


	delfix(n);

	RBTree_Node<T> *tmp = n;
	if (!n->dad)
	{
		root = 0;
	}
	else
	{
		if (n == n->dad->r)
		{
			n->dad->r = 0;
		}
		else
		{
			n->dad->l = 0;
		}
		delete tmp;
	}

}

template <class T>
void RBTree<T>::delfix(RBTree_Node<T> *n)
{
	if (!n->dad)
		return;
	

	RBTree_Node<T>* bro;
	if (n == n->dad->l)
	{
		bro = n->dad->r;
	}
	else
	{
		bro = n->dad->l;
	}
	//case2
	if (bro->color == 1)
	{
		n->dad->color = 1;
		bro->color = 0;
		if (n == n->dad->l)
		{
			rotateleft(n->dad);
		}
		else
		{
			rotateright(n->dad);
		}
	}
	// case3
	if (n == n->dad->l)
	{
		bro = n->dad->r;
	}
	else
	{
		bro = n->dad->l;
	}
	if ((bro->color == 0) && (n->dad->color == 0) && (!bro->l || bro->l->color == 0) &&
		(!bro->r || bro->r->color == 0))
	{
		bro->color = 1;

		delfix(n->dad);
		return;
	}

	//case4
	if (n == n->dad->l)
	{
		bro = n->dad->r;
	}
	else
	{
		bro = n->dad->l;
	}
	if ((bro->color == 0) && (n->dad->color == 1) && (!bro->l || bro->l->color == 0) &&
		(!bro->r || bro->r->color == 0))
	{
		bro->color = 1;
		n->dad->color = 0;
		return;
	}
	//case5
	if (n == n->dad->l)
	{
		bro = n->dad->r;
	}
	else
	{
		bro = n->dad->l;
	}
	if (bro->color == 0)
	{

		if ((n == n->dad->l) && (bro->l && bro->l->color == 1) &&
			(!bro->r || bro->r->color == 0))
		{
			bro->color = 1;
			bro->l->color = 0;
			rotateright(bro);
		}
		else
		{
			if ((n == n->dad->r) && (bro->r && bro->r->color == 1) &&
				(!bro->l || bro->l->color == 0))
			{
				bro->color = 1;
				bro->r->color = 0;
				rotateleft(bro);
			}
		}
	}
	//case6
	if (n == n->dad->l)
	{
		bro = n->dad->r;
	}
	else
	{
		bro = n->dad->l;
	}

	bro->color = n->dad->color;
	n->dad->color = 0;

	if (n == n->dad->l)
	{
		bro->r->color = 0;
		rotateleft(n->dad);
	}
	else
	{
		bro->l->color = 0;
		rotateright(n->dad);
	}
}

template <class T>
RBTree_Node<T>* RBTree<T>::findgrst(RBTree_Node<T> *n)
{
	if (n->r)
	{
		return findgrst(n->r);
	}
	else
	{
		return n;
	}
}

template <class T>
RBTree_Node<T>* RBTree<T>::search(const T &x)
{
	if (root) return find(x, root);
	return 0;
}

template <class T>
RBTree_Node<T>* RBTree<T>::find(const T &x, RBTree_Node<T> *n)
{
	if (n->key == x) return n;
	if (x < n->key)
	{
		if (n->l) return find(x, n->l);
		return 0;
	}
	else
	{
		if (n->r) return find(x, n->r);
		return 0;
	}
}

template <class T>
void RBTree<T>::print(RBTree_Node<T> *n)
{
	if (n == NULL) return;
	if (n->dad == NULL) std::cout << "Root\t";
	else if (n->color) std::cout << "Red\t";
	else std::cout << "Black\t";
	std::cout << n->key << std::endl;
	print(n->l);
	print(n->r);
}