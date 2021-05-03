#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

template <typename T> class PairNode {
public:
  T element;
  PairNode<T> *leftChild;
  PairNode<T> *nextSibling;
  PairNode<T> *parent;
  PairNode(const T &element) : element(element) {
    leftChild = nullptr;
    nextSibling = nullptr;
    parent = nullptr;
  }
};

template <typename T> class PairingHeap {
private:
  PairNode<T> *root;
  void reclaimMemory(PairNode<T> *t);
  void compareAndLink(PairNode<T> *&first, PairNode<T> *second);
  PairNode<T> *combineSiblings(PairNode<T> *firstSibling);
  PairNode<T> *clone(PairNode<T> *t);

public:
  PairingHeap();
  PairingHeap(PairingHeap<T> &rhs);
  ~PairingHeap();
  bool isEmpty();
  const T &get_min();
  PairNode<T> *insert(const T &x);
  void extract_min();
  void extract_min_and_get_val(T &minItem);
  void makeEmpty();
  void decrease_key(PairNode<T> *p, T &newVal);
  PairingHeap<T> &operator=(PairingHeap<T> &rhs);
};

template <typename T> PairingHeap<T>::PairingHeap() { root = nullptr; }

template <typename T> PairingHeap<T>::PairingHeap(PairingHeap<T> &rhs) {
  root = nullptr;
  *this = rhs;
}

template <typename T> PairingHeap<T>::~PairingHeap() { makeEmpty(); }

template <typename T> PairNode<T> *PairingHeap<T>::insert(const T &x) {
  PairNode<T> *newNode = new PairNode<T>(x);
  if (root == nullptr)
    root = newNode;
  else
    compareAndLink(root, newNode);
  return newNode;
}

template <typename T> const T &PairingHeap<T>::get_min() {
  return root->element;
}

template <typename T> void PairingHeap<T>::extract_min() {
  PairNode<T> *oldRoot = root;

  if (root->leftChild == nullptr)
    root = nullptr;

  else
    root = combineSiblings(root->leftChild);

  delete oldRoot;
}

template <typename T> void PairingHeap<T>::extract_min_and_get_val(T &minItem) {
  if (isEmpty()) {
    cout << "The Heap is Empty" << endl;
    return;
  }

  minItem = get_min();
  extract_min();
}

template <typename T> bool PairingHeap<T>::isEmpty() { return root == nullptr; }

template <typename T> void PairingHeap<T>::makeEmpty() {
  reclaimMemory(root);
  root = nullptr;
}

template <typename T>
PairingHeap<T> &PairingHeap<T>::operator=(PairingHeap<T> &rhs) {
  if (this != &rhs) {
    makeEmpty();
    root = clone(rhs.root);
  }
  return *this;
}

template <typename T> void PairingHeap<T>::reclaimMemory(PairNode<T> *t) {
  if (t != nullptr) {
    reclaimMemory(t->leftChild);
    reclaimMemory(t->nextSibling);
    delete t;
  }
}

template <typename T>
void PairingHeap<T>::decrease_key(PairNode<T> *p, T &newVal) {
  if (!p)
    return;

  if (p->element < newVal)
    return;

  p->element = newVal;
  if (p != root) {
    if (p->nextSibling != nullptr)
      p->nextSibling->parent = p->parent;

    if (p->parent->leftChild == p)
      p->parent->leftChild = p->nextSibling;

    else
      p->parent->nextSibling = p->nextSibling;

    p->nextSibling = nullptr;
    compareAndLink(root, p);
  }
}

template <typename T>
void PairingHeap<T>::compareAndLink(PairNode<T> *&first, PairNode<T> *second) {
  if (second == nullptr)
    return;
  if (second->element < first->element) {
    second->parent = first->parent;
    first->parent = second;
    first->nextSibling = second->leftChild;

    if (first->nextSibling != nullptr)
      first->nextSibling->parent = first;

    second->leftChild = first;
    first = second;
  } else {
    second->parent = first;
    first->nextSibling = second->nextSibling;

    if (first->nextSibling != nullptr)
      first->nextSibling->parent = first;

    second->nextSibling = first->leftChild;

    if (second->nextSibling != nullptr)
      second->nextSibling->parent = second;

    first->leftChild = second;
  }
}

template <typename T>
PairNode<T> *PairingHeap<T>::combineSiblings(PairNode<T> *firstSibling) {
  if (firstSibling->nextSibling == nullptr)
    return firstSibling;

  static vector<PairNode<T> *> treeArray(5);

  int numSiblings = 0;

  for (; firstSibling != nullptr; numSiblings++) {
    if (numSiblings == treeArray.size())
      treeArray.resize(numSiblings * 2);

    treeArray[numSiblings] = firstSibling;
    firstSibling->parent->nextSibling = nullptr;
    firstSibling = firstSibling->nextSibling;
  }

  if (numSiblings == treeArray.size())
    treeArray.resize(numSiblings + 1);

  treeArray[numSiblings] = nullptr;
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

template <typename T> PairNode<T> *PairingHeap<T>::clone(PairNode<T> *t) {
  if (t == nullptr)
    return nullptr;

  else {
    PairNode<T> *p = new PairNode<T>(t->element);
    if ((p->leftChild = clone(t->leftChild)) != nullptr)
      p->leftChild->parent = p;

    if ((p->nextSibling = clone(t->nextSibling)) != nullptr)
      p->nextSibling->parent = p;

    return p;
  }
}
