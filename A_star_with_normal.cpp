#include "./Graph.cpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename T, class Compare = std::greater<T>>
struct BinaryHeap
{
public:
  std::vector<T> heap;
  std::unordered_map<T, size_t> obj_to_index;
  Compare cmp;
  BinaryHeap(std::vector<T> &&vec)
  {
    heap = std::move(vec);
    cmp = Compare();
    std::make_heap(heap.begin(), heap.end(), cmp);
    for_each(heap.begin(), heap.end(), [i = 0, this](auto const &k) mutable {
      this->obj_to_index[k] = i;
      i++;
    });
  }
  BinaryHeap(const std::vector<T> &vect)
  {
    auto vec = vect;
    BinaryHeap(std::move(vec));
  }

  bool empty() const { return this->heap.empty(); }

  T const &get_min() { return this->heap.front(); }

  void insert(const T &key)
  {
    heap.push_back(key);
    obj_to_index[key] = heap.size() - 1;
    this->shift_up(heap.size() - 1);
  }

  void extract_min()
  {
    if (!this->empty())
    {
      std::swap(this->heap.front(), this->heap.back());
      this->obj_to_index[this->heap.front()] = 0;
      this->obj_to_index.erase(this->heap.back());
      this->heap.pop_back();
      this->shift_down(0);
      return;
    }
    throw std::runtime_error("Attempted to extract an empty heap");
  }

  long long get_index(T const &key)
  {
    return this->obj_to_index.find(key) == this->obj_to_index.end()
               ? -1
               : this->obj_to_index[key];
  }

  void decrease_key(size_t index, T const &new_val)
  {
    if (cmp(this->heap[index], new_val))
    {
      this->obj_to_index.erase(this->heap[index]);
      this->obj_to_index[new_val] = index;
      this->heap[index] = new_val;
      shift_up(index);
      return;
    }
    throw std::runtime_error("Invalid decrease key operation");
  }

private:
  void shift_up(size_t index)
  {
    if (index == 0)
      return;
    size_t parent = (index - 1) / 2;
    // keeps track of the map while  shift_up
    if (!cmp(this->heap[index], this->heap[parent]))
    {
      std::swap(this->heap[index], this->heap[parent]);
      this->obj_to_index[this->heap[index]] = index;
      this->obj_to_index[this->heap[parent]] = parent;
      shift_up(parent);
    }
  }
  void shift_down(size_t index)
  {
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
    if (to_be_swapped != index)
    {
      std::swap(this->heap[index], this->heap[to_be_swapped]);
      this->obj_to_index[this->heap[index]] = index;
      this->obj_to_index[this->heap[to_be_swapped]] = to_be_swapped;
      shift_down(to_be_swapped);
    }
  }
};

void printMap(BinaryHeap<int> &pq)
{
  for (auto x : pq.obj_to_index)
  {
    std::cout << x.first << ':' << x.second << ' ';
  }
  std::cout << '\n';
}

struct Data
{
  int weight;
  Data() : weight(0) {}
  Data(int wt) : weight(wt) {}
};

struct Node_Data
{
  int id;
  float f_val;
  float g_val;
  float h_val;
  bool operator<(const Node_Data &other) const { return f_val < other.f_val; }
  bool operator>(const Node_Data &other) const { return f_val > other.f_val; }
  bool operator==(const Node_Data &other) const
  {
    return f_val == other.f_val && id == other.id;
  }
  bool operator>=(const Node_Data &other) const
  {
    return ((*this) > other) || (other == (*this));
  }
  bool operator<=(const Node_Data &other) const
  {
    return ((*this) < other) || (other == (*this));
  }
  Node_Data(int node_num, float f_val, float g_val, float h_val)
      : id(node_num), f_val(f_val), g_val(g_val), h_val(h_val) {}

  Node_Data(const Node_Data &other) = default;
  Node_Data &operator=(const Node_Data &other)
  {
    id = other.id;
    f_val = other.f_val;
    g_val = other.g_val;
    h_val = other.h_val;
    return *this;
  }
};

template <>
struct std::hash<Node_Data>
{
  std::size_t operator()(Node_Data const &key) const
  {
    return std::hash<int>()(key.id);
  }
};

float get_h_val(int src, int dest,
                unordered_map<int, pair<int, int>> &node_to_coord)
{
  int a = node_to_coord[src].first;
  int b = node_to_coord[src].second;
  int c = node_to_coord[dest].first;
  int d = node_to_coord[dest].second;
  return sqrt((c - a) * (c - a) + (d - b) * (d - b));
}

void reconstruct_path(unordered_map<int, int> &came_from, int current)
{
  vector<int> path = {current};
  while (came_from[current] != -1)
  {
    current = came_from[current];
    path.insert(path.begin(), current);
  }
  for (int x : path)
    cout << x << ' ';
  cout << '\n';
}

int main()
{
  Graph<Data> G;
  G.insertEdges(0, {1, 2}, {Data(3), Data(2)});
  G.insertEdges(2, {3}, {Data(2)});
  G.insertEdges(1, {4}, {Data(1)});
  G.insertEdges(3, {4}, {Data(1)});

  int dest = 4;

  unordered_map<int, pair<int, int>> node_to_coord{
      {0, {0, 0}}, {1, {1, 1}}, {2, {2, 1}}, {3, {2, 3}}, {4, {3, 1}}};

  unordered_map<int, float> h_map;
  for (auto &x : node_to_coord)
  {
    h_map[x.first] = get_h_val(x.first, dest, node_to_coord);
  }

  using id = int;

  int num_of_nodes = 4;
  BinaryHeap<Node_Data> PQ({});

  vector<Node_Data> node_datas; // used for finding the index in heap

  node_datas.push_back(Node_Data(0, h_map[0], 0, h_map[0]));
  PQ.insert(Node_Data(0, h_map[0], 0, h_map[0]));
  for (int i = 1; i <= num_of_nodes; i++)
  {
    node_datas.push_back(Node_Data(i, numeric_limits<float>::infinity(),
                                   numeric_limits<float>::infinity(),
                                   h_map[i]));
    PQ.insert(node_datas[i]);
  }

  unordered_map<id, id> came_from;
  came_from[0] = -1;

  while (!PQ.empty())
  {
    Node_Data current = PQ.get_min();
    if (current.id == dest)
    {
      cout << "Done\n";
      cout << current.g_val << '\n';
      reconstruct_path(came_from, current.id);
      break;
    }
    PQ.extract_min();
    id src = current.id;
    for (auto &kv : G.adjList[src])
    {
      auto &k = kv.first; // k is node id
      auto &v = kv.second;
      // stuck here
      float tentative_gscore = current.g_val + v.weight;
      if (tentative_gscore < node_datas[k].g_val)
      {
        came_from[k] = src;
        Node_Data replacement = node_datas[k];
        replacement.g_val = tentative_gscore;
        replacement.f_val = replacement.g_val + replacement.h_val;
        PQ.decrease_key(PQ.obj_to_index[node_datas[k]], replacement);
        node_datas[k] = replacement;
      }
    }
  }
  return 0;
}
