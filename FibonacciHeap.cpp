#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::endl;

template <typename T> class FibHeap {
public:
  FibHeap(const T &min_possible_key_of_T)
      : min_possible_key_of_type_T(min_possible_key_of_T){};

  FibHeap(FibHeap<T> &other) = delete;
  FibHeap(FibHeap<T> &&other) { *this = other; }
  ~FibHeap() { this->clear(); }

  // node class starts
  struct node {
    T data;
    node *next = nullptr;
    node *prev = nullptr;
    node *child = nullptr;
    node *parent = nullptr;
    size_t degree = 0;
    bool mark = false;
    node(T val) : data(val) {
      prev = this;
      next = this;
    }

    // merge two circular linked lists
    void merge(node *other) {
      if (other == nullptr)
        return;
      this->prev->next = other;
      other->prev->next = this;
      node *tmp = other->prev;
      other->prev = this->prev;
      this->prev = tmp;
    }

    void print() {
      node *tmp = this;
      if (tmp != nullptr) {
        cout << tmp->data << ' ';
        tmp = tmp->next;
      }
      while (tmp != this) {
        cout << tmp->data << ' ';
        tmp = tmp->next;
      }
    }

    std::vector<node *> get_all_nodes() {
      std::vector<node *> ret;
      node *tmp = this;
      if (tmp != nullptr) {
        ret.push_back(tmp);
        tmp = tmp->next;
      }
      while (tmp != this) {
        ret.push_back(tmp);
        tmp = tmp->next;
      }
      return ret;
    }
  }; // node class ends

  bool empty() noexcept { return min == nullptr; }

  FibHeap<T>::node *insert(T data) {
    node *x = new node(data);
    if (min == nullptr) {
      min = x;
    } else {
      min->merge(x);
      if (min->data > x->data) {
        min = x;
      }
    }
    this->n = this->n + 1;
    return x;
  }

  bool contains(T data) { return find_node(data) != nullptr; }

  bool delete_key(const T &curr_key) {
    if (decrease_key(curr_key, this->min_possible_key_of_type_T)) {
      extract_min();
      return true;
    }
    return false;
  }
  bool decrease_key(const T &curr_key, const T &new_key) {
    if (new_key > curr_key) {
      return false;
    }
    node *containing_node = this->find_node(curr_key);
    if (containing_node == nullptr)
      return false;
    return this->decrease_key(containing_node, new_key);
  }

  void merge(FibHeap<T> &other) {
    if (min == nullptr) {
      min = other.min;
    } else {
      min->merge(other.min);
      if (min->data > other.min->data) {
        this->min = other.min;
      }
    }
    this->n = this->n + other.n;
    other.n = 0;
    other.min = nullptr;
  }

  void print(bool show_level_wise = true) {
    // min->print();
    // cout << '\n';
    auto to_str = [](auto const &x) {
      std::stringstream s;
      s << x;
      return s.str();
    };
    std::vector<std::pair<node *, size_t>> all_nodes = get_the_entire_tree(min);
    int level = -1;
    for (const auto &x : all_nodes) {
      if (((int)x.second) > level) {
        if (show_level_wise)
          cout << '\n' << "Level " << x.second << ": ";
        level++;
      }
      cout << "(" << x.first->data << ",L:" << x.second
           << ",D:" << x.first->degree << ",P:"
           << ((x.first->parent == nullptr) ? "NULL"
                                            : (to_str(x.first->parent->data)))
           << ")" << (show_level_wise ? "  " : "\n");
    }
    cout << '\n';
    return;
  }

  T extract_min() {
    T ret_val = this->get_min();
    node *z = min;
    std::vector<node *> children = min->child->get_all_nodes();
    for (node *child : children) {
      child->parent = nullptr;
      make_node_point_to_itself(child);
      min->merge(child);
    }
    remove_connection_from_prev_and_next(min);
    if (z == z->next) {
      min = nullptr;
      delete z;
    } else {
      min = z->next;
      consolidate();
      delete z;
    }
    this->n -= 1;
    return ret_val;
  }

  T get_min() {
    if (this->min == nullptr) {
      throw std::out_of_range("Empty heap");
    }
    return this->min->data;
  }

  bool decrease_key(node *x, const T &new_key) {
    x->data = new_key;
    node *xParent = x->parent;
    if (xParent != nullptr && x->data < xParent->data) {
      cut(x, xParent);
      cascading_cut(xParent);
    }
    if (x->data < min->data) {
      min = x;
    }
    return true;
  }

  FibHeap<T> &operator=(const FibHeap<T> &other) = delete;

  FibHeap<T> &operator=(FibHeap<T> &other) = delete;

  FibHeap<T> &operator=(FibHeap<T> &&other) {
    if (this->min != other.min) {
      this->clear();
      this->min = other.min;
      other.min = nullptr;
      this->min_possible_key_of_type_T = other.min_possible_key_of_type_T;
      this->n = other.n;
      other.n = 0;
    }
    return (*this);
  }

  void clear() {
    if (min != nullptr) {
      for (auto &x : get_the_entire_tree(min)) {
        delete x.first;
      }
    }
  }

private:
  // attribute of Fib Heap
  node *min = nullptr;
  T min_possible_key_of_type_T;
  size_t n = 0;

  void fib_heap_link(node *y, node *x) {
    y->prev->next = y->next;
    y->next->prev = y->prev;
    y->next = y;
    y->prev = y;
    if (x->child) {
      x->child->merge(y);
    } else {
      x->child = y;
    }
    y->parent = x;
    x->degree++;
    y->mark = false;
  }

  void remove_connection_from_prev_and_next(node *x) {
    if (x == nullptr)
      return;
    x->prev->next = x->next;
    x->next->prev = x->prev;
  }

  void make_node_point_to_itself(node *x) {
    x->next = x;
    x->prev = x;
  }

  void cut(node *x, node *y) {
    // y is parent and x is child
    // wont remove connections from x , just removes connections to x
    remove_connection_from_prev_and_next(x);
    y->degree--;
    if (y->degree != 0) {
      y->child = x->next;
    } else {
      y->child = nullptr;
    }
    // removes next and prev of x, and makes it point to itself
    make_node_point_to_itself(x);
    min->merge(x);
    x->parent = nullptr;
    x->mark = false;
  }

  void cascading_cut(node *y) {
    node *ysParent = y->parent;
    if (ysParent) {
      // when one child has been cut
      if (y->mark == false) {
        y->mark = true;
      } else {
        // when one was already cut this is 2nd one, then we do this
        // we cut current node from its parent and move it to the rootlist
        // and do this recursively going up until we reach a node which is root
        // or hasnt been marked yet
        cut(y, ysParent);
        cascading_cut(ysParent);
      }
    }
  }

  node *find_node(T data) {
    std::vector<std::pair<node *, size_t>> all_nodes = get_the_entire_tree(min);
    for (std::pair<node *, size_t> &x : all_nodes) {
      if (x.first->data == data) {
        return x.first;
      }
    }
    return nullptr;
  }

  // will be used in printing and search
  std::vector<std::pair<node *, size_t>> get_the_entire_tree(node *root) {
    if (root == nullptr) {
      return {};
    }
    std::unordered_map<node *, int> level_map;
    level_map[root] = 0;
    std::queue<node *> Q;
    for (node *sibling : root->get_all_nodes()) {
      Q.push(sibling);
      level_map[sibling] = 0;
    }
    std::vector<std::pair<node *, size_t>> ret;
    node *to_be_popped;
    while (!Q.empty()) {
      to_be_popped = Q.front();
      ret.emplace_back(to_be_popped, level_map[to_be_popped]);
      Q.pop();
      if (to_be_popped->child) {
        for (node *sibling : to_be_popped->child->get_all_nodes()) {
          Q.push(sibling);
          level_map[sibling] = level_map[sibling->parent] + 1;
        }
      }
    }
    return ret;
  }

  // this'll be used to reinstate the heap property
  void consolidate() {
    std::vector<node *> A(ceil(log2(this->n)), nullptr);
    std::vector<node *> rootlist_nodes = min->get_all_nodes();
    node *cur_node;
    node *same_deg_node;
    size_t deg;
    for (node *w : rootlist_nodes) {
      cur_node = w;
      deg = w->degree;
      while (A[deg] != nullptr) {
        same_deg_node = A[deg];
        if (cur_node->data > same_deg_node->data) {
          std::swap(cur_node, same_deg_node);
        }
        this->fib_heap_link(same_deg_node, cur_node);
        A[deg] = nullptr;
        deg++;
      }
      A[deg] = cur_node;
    }
    min = nullptr;
    for (size_t i = 0; i < A.size(); i++) {
      if (A[i] != nullptr) {
        if (min == nullptr) {
          min = A[i];
          make_node_point_to_itself(min);
        } else {
          make_node_point_to_itself(A[i]);
          min->merge(A[i]);
          if (min->data > A[i]->data) {
            min = A[i];
          }
        }
      }
    }
  }
};
