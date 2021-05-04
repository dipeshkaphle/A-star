#ifndef UTILITIES_CPP
#define UTILITIES_CPP

#include "BinaryHeap.cpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <ostream>
#include <unordered_map>
#include <vector>

struct Data {
  int weight;
  Data() : weight(0) {}
  Data(int wt) : weight(wt) {}
};

struct Node_Data {
  int id;
  float f_val;
  float g_val;
  float h_val;
  bool operator<(const Node_Data &other) const { return f_val < other.f_val; }
  bool operator>(const Node_Data &other) const { return f_val > other.f_val; }
  bool operator==(const Node_Data &other) const {
    return f_val == other.f_val && id == other.id;
  }
  bool operator>=(const Node_Data &other) const {
    return ((*this) > other) || (other == (*this));
  }
  bool operator<=(const Node_Data &other) const {
    return ((*this) < other) || (other == (*this));
  }
  Node_Data(int node_num, float f_val, float g_val, float h_val)
      : id(node_num), f_val(f_val), g_val(g_val), h_val(h_val) {}

  Node_Data(const Node_Data &other) = default;
  Node_Data &operator=(const Node_Data &other) {
    id = other.id;
    f_val = other.f_val;
    g_val = other.g_val;
    h_val = other.h_val;
    return *this;
  }
};

std::ostream &operator<<(std::ostream &os, Node_Data const &m) {
  return os << "(" << m.id << "," << m.g_val << ")";
}

float get_h_val(int src, int dest,
                std::unordered_map<int, std::pair<int, int>> &node_to_coord) {
  int a = node_to_coord[src].first;
  int b = node_to_coord[src].second;
  int c = node_to_coord[dest].first;
  int d = node_to_coord[dest].second;
  return sqrt((c - a) * (c - a) + (d - b) * (d - b));
}

void reconstruct_path(std::vector<int> &came_from, int current) {
  std::vector<int> path = {current};
  while (came_from[current] != -1) {
    current = came_from[current];
    path.insert(path.begin(), current);
  }
  for (int x : path)
    std::cout << x << ' ';
  std::cout << '\n';
}

template <typename Func, typename... Args>
double timeMyFunction(Func func, Args &&...args) {
  auto start_time = std::chrono::steady_clock::now();
  func(std::forward<Args>(args)...);
  auto end_time = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_time =
      std::chrono::duration_cast<std::chrono::duration<double>>(end_time -
                                                                start_time);
  return elapsed_time.count();
}

void printMap(BinaryHeap<int> &pq) {
  for (auto x : pq.obj_to_index) {
    std::cout << x.first << ':' << x.second << ' ';
  }
  std::cout << '\n';
}

template <> struct std::hash<Node_Data> {
  std::size_t operator()(Node_Data const &key) const {
    return std::hash<int>()(key.id);
  }
};

#endif
