#include "FibonacciHeap.cpp"
#include "Graph.cpp"

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

const Node_Data min_possible(-1, -numeric_limits<float>::infinity(),
                             -numeric_limits<float>::infinity(),
                             -numeric_limits<float>::infinity());

// template <> struct hash<Data> {
//   std::size_t operator()(const Data &key) const {
//     return std::hash<int>()(key.weight);
//   }
// };

float get_h_val(int src, int dest,
                unordered_map<int, pair<int, int>> &node_to_coord) {
  int a = node_to_coord[src].first;
  int b = node_to_coord[src].second;
  int c = node_to_coord[dest].first;
  int d = node_to_coord[dest].second;
  return sqrt((c - a) * (c - a) + (d - b) * (d - b));
}

void reconstruct_path(unordered_map<int, int> &came_from, int current) {
  vector<int> path = {current};
  while (came_from[current] != -1) {
    current = came_from[current];
    path.insert(path.begin(), current);
  }
  for (int x : path)
    cout << x << ' ';
  cout << '\n';
}

int main() {
  Graph<Data> G;
  G.insertEdges(0, {1, 2}, {Data(3), Data(2)});
  G.insertEdges(2, {3}, {Data(2)});
  G.insertEdges(1, {4}, {Data(1)});
  G.insertEdges(3, {4}, {Data(1)});

  int dest = 4;

  unordered_map<int, pair<int, int>> node_to_coord{
      {0, {0, 0}}, {1, {1, 1}}, {2, {2, 1}}, {3, {2, 3}}, {4, {3, 1}}};

  unordered_map<int, float> h_map;
  for (auto &x : node_to_coord) {
    h_map[x.first] = get_h_val(x.first, dest, node_to_coord);
  }

  using id = int;
  unordered_map<id, FibHeap<Node_Data>::node *> ptr_map;

  int num_of_nodes = 4;
  FibHeap<Node_Data> PQ(min_possible);
  ptr_map[0] = PQ.insert(Node_Data(0, h_map[0], 0, h_map[0]));
  for (int i = 1; i <= num_of_nodes; i++) {
    ptr_map[i] =
        PQ.insert(Node_Data(i, numeric_limits<float>::infinity(),
                            numeric_limits<float>::infinity(), h_map[i]));
  }

  unordered_map<id, id> came_from;
  came_from[0] = -1;

  while (!PQ.empty()) {
    Node_Data current = PQ.get_min();
    if (current.id == dest) {
      cout << "Done\n";
      cout << current.g_val << '\n';
      reconstruct_path(came_from, current.id);
      break;
    }
    PQ.extract_min();

    id src = current.id;
    for (auto &kv : G.adjList[src]) {
      auto &k = kv.first;
      auto &v = kv.second;

      float tentative_gscore = current.g_val + v.weight;

      if (tentative_gscore < ptr_map[k]->data.g_val) {
        came_from[k] = src;
        Node_Data replacement =
            ptr_map[k]
                ->data; // copy constructor is defined so we can just do this
        replacement.g_val = tentative_gscore;
        replacement.f_val = replacement.g_val + replacement.h_val;
        PQ.decrease_key(ptr_map[k], replacement);
      }
    }
  }
  return 0;
}
