#ifndef BINARY_HEAP
#define BINARY_HEAP

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename T, class Compare = std::greater<T>> struct BinaryHeap {
public:
  std::vector<T> heap;
  std::unordered_map<T, size_t> obj_to_index;
  Compare cmp;
  BinaryHeap(std::vector<T> &&vec) {
    heap = std::move(vec);
    cmp = Compare();
    std::make_heap(heap.begin(), heap.end(), cmp);
    for_each(heap.begin(), heap.end(), [i = 0, this](auto const &k) mutable {
      this->obj_to_index[k] = i;
      i++;
    });
  }
  BinaryHeap(const std::vector<T> &vect) {
    auto vec = vect;
    BinaryHeap(std::move(vec));
  }

  bool empty() const { return this->heap.empty(); }

  T const &get_min() { return this->heap.front(); }

  void insert(const T &key) {
    heap.push_back(key);
    obj_to_index[key] = heap.size() - 1;
    this->shift_up(heap.size() - 1);
  }

  void extract_min() {
    if (!this->empty()) {
      std::swap(this->heap.front(), this->heap.back());
      this->obj_to_index[this->heap.front()] = 0;
      this->obj_to_index.erase(this->heap.back());
      this->heap.pop_back();
      this->shift_down(0);
      return;
    }
    throw std::runtime_error("Attempted to extract an empty heap");
  }

  long long get_index(T const &key) {
    return this->obj_to_index.find(key) == this->obj_to_index.end()
               ? -1
               : this->obj_to_index[key];
  }

  void decrease_key(size_t index, T const &new_val) {
    if (cmp(this->heap[index], new_val)) {
      this->obj_to_index.erase(this->heap[index]);
      this->obj_to_index[new_val] = index;
      this->heap[index] = new_val;
      shift_up(index);
      return;
    }
    throw std::runtime_error("Invalid decrease key operation");
  }

  void merge(const BinaryHeap<T, Compare> &other) {
    this->heap.insert(this->heap.end(), other.heap.begin(), other.heap.end());
    std::make_heap(this->heap.begin(), this->heap.end(), this->cmp);
  }

private:
  void shift_up(size_t index) {
    if (index == 0)
      return;
    size_t parent = (index - 1) / 2;
    // keeps track of the map while  shift_up
    if (!cmp(this->heap[index], this->heap[parent])) {
      std::swap(this->heap[index], this->heap[parent]);
      this->obj_to_index[this->heap[index]] = index;
      this->obj_to_index[this->heap[parent]] = parent;
      shift_up(parent);
    }
  }
  void shift_down(size_t index) {
    if (index >= this->heap.size())
      return;
    size_t lft = 2 * index + 1;
    size_t rgt = lft + 1;
    size_t to_be_swapped = index;
    if (lft < this->heap.size() && !cmp(this->heap[lft], this->heap[index]))
      to_be_swapped = lft;
    if (rgt < this->heap.size() &&
        !cmp(this->heap[rgt], this->heap[to_be_swapped]))
      to_be_swapped = rgt;
    if (to_be_swapped != index) {
      std::swap(this->heap[index], this->heap[to_be_swapped]);
      this->obj_to_index[this->heap[index]] = index;
      this->obj_to_index[this->heap[to_be_swapped]] = to_be_swapped;
      shift_down(to_be_swapped);
    }
  }
};

#endif
