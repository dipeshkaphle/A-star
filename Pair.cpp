#ifndef PAIRING_HEAP_CPP
#define PAIRING_HEAP_CPP
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

template <typename T> class PairNode {
public:
  T element;
  PairNode<T> *leftChild;
  PairNode<T> *nextSibling;
  PairNode<T> *prev;
  PairNode(const T &element) : element(element) {
    leftChild = nullptr;
    nextSibling = nullptr;
    prev = nullptr;
  }
};

template <typename T> class PairingHeap {
private:
  PairNode<T> *root;

  void clear(PairNode<T> *t) {
    if (t != nullptr) {
      vector<PairNode<T> *> stck;
      stck.push_back(t);
      while (!stck.empty()) {
        auto curr = stck.back();
        stck.pop_back();
        if (curr->nextSibling != nullptr)
          stck.push_back(curr->nextSibling);
        if (curr->leftChild != nullptr)
          stck.push_back(curr->leftChild);
        delete curr;
      }
    }
    // if (t != nullptr) {
    //   clear(t->leftChild);
    //   clear(t->nextSibling);
    //   delete t;
    // }
  }

  void compareAndLink(PairNode<T> *&first, PairNode<T> *second);
  PairNode<T> *combineSiblings(PairNode<T> *firstSibling);
  PairNode<T> *clone(PairNode<T> *t);

public:
  PairingHeap() : root(nullptr) {}
  PairingHeap(PairingHeap<T> &rhs) {
    root = nullptr;
    *this = rhs;
  }
  PairingHeap<T> &operator=(PairingHeap<T> &rhs) {
    if (this != &rhs) {
      clear();
      root = clone(rhs.root);
    }
    return *this;
  }

  ~PairingHeap() { this->clear(); }

  bool empty() const { return this->root == nullptr; }

  const T &get_min() {
    if (this->root != nullptr)
      return this->root->element;
    throw std::runtime_error(
        "Empty heap. Cant get the min element in empty heap");
  }

  PairNode<T> *insert(const T &x);

  void extract_min();

  T extract_min_and_get_val();

  void clear() {
    clear(root);
    root = nullptr;
  }
  void decrease_key(PairNode<T> *p, const T &newVal);
};

template <typename T> PairNode<T> *PairingHeap<T>::insert(const T &x) {
  PairNode<T> *newNode = new PairNode<T>(x);
  if (root == nullptr)
    root = newNode;
  else
    compareAndLink(root, newNode);
  return newNode;
}

template <typename T> void PairingHeap<T>::extract_min() {
  if (this->root == nullptr)
    throw std::runtime_error("Cant perform extract min on empty heap");
  PairNode<T> *oldRoot = root;
  if (root->leftChild == nullptr)
    root = nullptr;
  else
    root = combineSiblings(root->leftChild);
  delete oldRoot;
}

// the type T must have a copy or move constructor
template <typename T> T PairingHeap<T>::extract_min_and_get_val() {
  if (this->empty()) {
    cout << "The Heap is Empty" << endl;
    throw std::runtime_error("Empty heap");
  }
  auto ret = get_min();
  extract_min();
  return ret;
}

template <typename T>
void PairingHeap<T>::decrease_key(PairNode<T> *p, const T &replacement) {
  if (p == nullptr)
    return;
  if (p->element < replacement)
    return;

  p->element = replacement;
  if (p != root) {
    if (p->nextSibling != nullptr)
      p->nextSibling->prev = p->prev;
    if (p->prev->leftChild == p)
      p->prev->leftChild = p->nextSibling;
    else
      p->prev->nextSibling = p->nextSibling;
    p->nextSibling = nullptr;
    compareAndLink(root, p);
  }
}

template <typename T>
void PairingHeap<T>::compareAndLink(PairNode<T> *&first, PairNode<T> *second) {
  if (second == nullptr)
    return;
  if (second->element < first->element) {
    second->prev = first->prev;
    first->prev = second;
    first->nextSibling = second->leftChild;
    if (first->nextSibling != nullptr)
      first->nextSibling->prev = first;
    second->leftChild = first;
    first = second;
  } else {
    second->prev = first;
    first->nextSibling = second->nextSibling;
    if (first->nextSibling != nullptr)
      first->nextSibling->prev = first;
    second->nextSibling = first->leftChild;
    if (second->nextSibling != nullptr)
      second->nextSibling->prev = second;
    first->leftChild = second;
  }
}

template <typename T>
PairNode<T> *PairingHeap<T>::combineSiblings(PairNode<T> *first_sibling) {
  if (first_sibling->nextSibling == nullptr)
    return first_sibling;

  vector<PairNode<T> *> tree_arr(5);

  int num_siblings = 0;

  for (first_sibling; first_sibling != nullptr; num_siblings++) {
    if (num_siblings == tree_arr.size())
      tree_arr.resize(num_siblings * 2);

    tree_arr[num_siblings] = first_sibling;
    first_sibling->prev->nextSibling = nullptr;
    first_sibling = first_sibling->nextSibling;
  }

  if (num_siblings == tree_arr.size())
    tree_arr.resize(num_siblings + 1);

  tree_arr[num_siblings] = nullptr;
  int i = 0;

  for (; i + 1 < num_siblings; i += 2)
    compareAndLink(tree_arr[i], tree_arr[i + 1]);

  int j = i - 2;

  if (j == num_siblings - 3)
    compareAndLink(tree_arr[j], tree_arr[j + 2]);

  for (; j >= 2; j -= 2)
    compareAndLink(tree_arr[j - 2], tree_arr[j]);

  return tree_arr[0];
}

template <typename T> PairNode<T> *PairingHeap<T>::clone(PairNode<T> *t) {
  if (t == nullptr)
    return nullptr;
  PairNode<T> *p = new PairNode<T>(t->element);
  if ((p->leftChild = clone(t->leftChild)) != nullptr)
    p->leftChild->prev = p;
  if ((p->nextSibling = clone(t->nextSibling)) != nullptr)
    p->nextSibling->prev = p;
  return p;
}

#endif
