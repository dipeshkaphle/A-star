#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

template <typename T>
class PairNode
{
public:
    T element;
    PairNode *leftChild;
    PairNode *nextSibling;
    PairNode *parent;
    PairNode(T element)
    {
        this->element = element;
        leftChild = nullptr;
        nextSibling = nullptr;
        parent = nullptr;
    }
};

template <typename T>
class PairingHeap
{
private:
    PairNode<T> *root;
    void reclaimMemory(PairNode<T> *t);
    void compareAndLink(PairNode<T> *first, PairNode<T> *second);
    PairNode<T> *combineSiblings(PairNode<T> *firstSibling);
    PairNode<T> *clone(PairNode<T> *t);

public:
    PairingHeap();
    PairingHeap(PairingHeap<T> &rhs);
    ~PairingHeap();
    bool isEmpty();
    bool isFull();
    T &findMin();
    PairNode<T> *Insert(T &x);
    void deleteMin();
    void deleteMin(T &minItem);
    void makeEmpty();
    void decreaseKey(PairNode<T> *p, T &newVal);
    PairingHeap<T> &operator=(PairingHeap<T> &rhs);
};

template <typename T>
PairingHeap<T>::PairingHeap()
{
    root = NULL;
}

template <typename T>
PairingHeap<T>::PairingHeap(PairingHeap<T> &rhs)
{
    root = NULL;
    *this = rhs;
}

template <typename T>
PairingHeap<T>::~PairingHeap()
{
    makeEmpty();
}

template <typename T>
PairNode<T> *PairingHeap<T>::Insert(T &x)
{
    PairNode<T> *newNode = new PairNode<T>(x);
    if (root == NULL)
        root = newNode;
    else
        compareAndLink(root, newNode);
    return newNode;
}

template <typename T>
T &PairingHeap<T>::findMin()
{
    return root->element;
}

template<typename T>
void PairingHeap<T>::deleteMin()
{
    PairNode<T> *oldRoot = root;

    if (root->leftChild == NULL)
        root = NULL;

    else
        root = combineSiblings(root->leftChild);

    delete oldRoot;
}


template<typename T>
void PairingHeap<T>::deleteMin(T &minItem)
{
    if (isEmpty())
    {
        cout << "The Heap is Empty" << endl;
        return;
    }

    minItem = findMin();
    deleteMin();
    cout << "Minimum Element: " << minItem << " deleted" << endl;
}

template<typename T>
bool PairingHeap<T>::isEmpty()
{
    return root == NULL;
}

template<typename T>
bool PairingHeap<T>::isFull()
{
    return false;
}

template<typename T>
void PairingHeap<T>::makeEmpty()
{
    reclaimMemory(root);
    root = NULL;
}

template<typename T>
PairingHeap<T> &PairingHeap<T>::operator=(PairingHeap<T> &rhs)
{
    if (this != &rhs)
    {
        makeEmpty();
        root = clone(rhs.root);
    }
    return *this;
}

template<typename T>
void PairingHeap<T>::reclaimMemory(PairNode<T> *t)
{
    if (t != NULL)
    {
        reclaimMemory(t->leftChild);
        reclaimMemory(t->nextSibling);
        delete t;
    }
}


template<typename T>
void PairingHeap<T>::decreaseKey(PairNode<T>*p, T &newVal)
{
    if (p->element < newVal)
        return;

    p->element = newVal;
    if (p != root)
    {
        if (p->nextSibling != NULL)
            p->nextSibling->parent = p->parent;

        if (p->parent->leftChild == p)
            p->parent->leftChild = p->nextSibling;

        else
            p->parent->nextSibling = p->nextSibling;

        p->nextSibling = NULL;
        compareAndLink(root, p);
    }
}

template<typename T>
void PairingHeap<T>::compareAndLink(PairNode<T> *first, PairNode<T> *second)
{
    if (second == NULL)
        return;
    if (second->element < first->element)
    {
        second->parent = first->parent;
        first->parent = second;
        first->nextSibling = second->leftChild;

        if (first->nextSibling != NULL)
            first->nextSibling->parent = first;

        second->leftChild = first;
        first = second;
    }
    else
    {
        second->parent = first;
        first->nextSibling = second->nextSibling;

        if (first->nextSibling != NULL)
            first->nextSibling->parent = first;

        second->nextSibling = first->leftChild;

        if (second->nextSibling != NULL)
            second->nextSibling->parent = second;

        first->leftChild = second;
    }
}

template<typename T>
PairNode<T> *PairingHeap<T>::combineSiblings(PairNode<T> *firstSibling)
{
    if (firstSibling->nextSibling == NULL)
        return firstSibling;

    static vector<PairNode<T> *> treeArray(5);

    int numSiblings = 0;

    for (; firstSibling != NULL; numSiblings++)
    {
        if (numSiblings == treeArray.size())
            treeArray.resize(numSiblings * 2);

        treeArray[numSiblings] = firstSibling;
        firstSibling->parent->nextSibling = NULL;
        firstSibling = firstSibling->nextSibling;
    }

    if (numSiblings == treeArray.size())
        treeArray.resize(numSiblings + 1);

    treeArray[numSiblings] = NULL;
    int i = 0;

    for (; i + 1 < numSiblings; i += 2)
        compareAndLink(treeArray[i], treeArray[i + 1]);

    int j = i - 2;

    if (j == numSiblings - 3)
        compareAndLink(treeArray[j], treeArray[j + 2]);

    for (; j >= 2; j -= 2)
        compareAndLink(treeArray[j - 2], treeArray[j]);

    return treeArray[0];
}

template<typename T>
PairNode<T> *PairingHeap<T>::clone(PairNode<T> *t)
{
    if (t == NULL)
        return NULL;

    else
    {
        PairNode <T>*p = new PairNode<T>(t->element);
        if ((p->leftChild = clone(t->leftChild)) != NULL)
            p->leftChild->parent = p;

        if ((p->nextSibling = clone(t->nextSibling)) != NULL)
            p->nextSibling->parent = p;

        return p;
    }
}

