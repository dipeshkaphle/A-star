#include "BinaryHeap.cpp"
#include "FibonacciHeap.cpp"
#include "Graph.cpp"
#include "Pair.cpp"
#include "utility_classes.cpp"

#include <fstream>
#include <iomanip>

const Node_Data min_possible(-1, -numeric_limits<float>::infinity(),
                             -numeric_limits<float>::infinity(),
                             -numeric_limits<float>::infinity());

unordered_map<int, pair<int, int>> node_to_coord;
unordered_map<int, float> h_map;
void fill_node_to_coord(int num_of_nodes) {
  srand(time(nullptr));
  node_to_coord.clear();
  for (int i = 0; i < num_of_nodes; i++) {
    node_to_coord[i] = {rand() % 1000, rand() % 1000};
  }
}
void fill_h_map(int dest) {
  h_map.clear();
  for (auto &x : node_to_coord) {
    h_map[x.first] = get_h_val(x.first, dest, node_to_coord);
  }
}

namespace PairHeap {
void A_star(Graph<Data> &G, int dest, int num_of_nodes) {

  using id = int;
  vector<PairNode<Node_Data> *> ptr_map(num_of_nodes);
  PairingHeap<Node_Data> PQ;
  ptr_map[0] = PQ.insert(Node_Data(0, h_map[0], 0, h_map[0]));
  for (int i = 1; i < num_of_nodes; i++) {
    ptr_map[i] =
        PQ.insert(Node_Data(i, numeric_limits<float>::infinity(),
                            numeric_limits<float>::infinity(), h_map[i]));
  }
  vector<id> came_from(num_of_nodes, -1);

  while (!PQ.empty()) {
    Node_Data current(PQ.get_min());
    if (current.id == dest) {
      cout << "For V=" << num_of_nodes << "\n";
      cout << "Done\n";
      cout << current.g_val << '\n';
      reconstruct_path(came_from, current.id);
      break;
    }
    PQ.extract_min();
    ptr_map[current.id] = nullptr;
    id src = current.id;
    for (auto &kv : G.adjList[src]) {
      auto &k = kv.first;
      auto &v = kv.second;
      if (ptr_map[k] == nullptr)
        continue;
      float tentative_gscore = current.g_val + v.weight;
      if (tentative_gscore < ptr_map[k]->element.g_val) {
        came_from[k] = src;
        Node_Data replacement =
            ptr_map[k]
                ->element; // copy constructor is defined so we can just do this
        replacement.g_val = tentative_gscore;
        replacement.f_val = replacement.g_val + replacement.h_val;
        PQ.decrease_key(ptr_map[k], replacement);
      }
    }
  }
}
} // namespace PairHeap

namespace BinHeap {
void A_star(Graph<Data> &G, int dest, int num_of_nodes) {
  using id = int;
  BinaryHeap<Node_Data> PQ({});
  vector<Node_Data> node_datas; // used for finding the index in heap
  node_datas.reserve(num_of_nodes);

  node_datas.emplace_back(0, h_map[0], 0, h_map[0]);
  PQ.insert(Node_Data(0, h_map[0], 0, h_map[0]));
  for (int i = 1; i < num_of_nodes; i++) {
    node_datas.emplace_back(i, numeric_limits<float>::infinity(),
                            numeric_limits<float>::infinity(), h_map[i]);
    PQ.insert(node_datas[i]);
  }
  vector<id> came_from(num_of_nodes, -1);
  vector<bool> extracted_already(num_of_nodes, false);

  while (!PQ.empty()) {
    Node_Data current = PQ.get_min();
    if (current.id == dest) {
      cout << "Found\n";
      cout << "For V=" << num_of_nodes << '\n';
      cout << current.g_val << '\n';
      reconstruct_path(came_from, current.id);
      break;
    }
    PQ.extract_min();
    extracted_already[current.id] = true;
    id src = current.id;
    for (auto &kv : G.adjList[src]) {
      auto &k = kv.first; // k is node id
      auto &v = kv.second;
      if (extracted_already[k])
        continue;
      // stuck here
      float tentative_gscore = current.g_val + v.weight;
      if (tentative_gscore < node_datas[k].g_val) {
        came_from[k] = src;
        Node_Data replacement = node_datas[k];
        replacement.g_val = tentative_gscore;
        replacement.f_val = replacement.g_val + replacement.h_val;
        PQ.decrease_key(PQ.obj_to_index[node_datas[k]], replacement);
        node_datas[k] = replacement;
      }
    }
  }
}

} // namespace BinHeap

namespace Fib {
void A_star(Graph<Data> &G, int dest, int num_of_nodes) {
  using id = int;
  vector<FibHeap<Node_Data>::node *> ptr_map(num_of_nodes);

  FibHeap<Node_Data> PQ(min_possible);
  ptr_map[0] = PQ.insert(Node_Data(0, h_map[0], 0, h_map[0]));
  for (int i = 1; i < num_of_nodes; i++) {
    ptr_map[i] =
        PQ.insert(Node_Data(i, numeric_limits<float>::infinity(),
                            numeric_limits<float>::infinity(), h_map[i]));
  }

  vector<id> came_from(num_of_nodes, -1);

  while (!PQ.empty()) {
    // PQ.print();
    Node_Data current = PQ.extract_min();
    ptr_map[current.id] = nullptr;
    // cout << "Extracted " << current.id << endl;
    // PQ.print();
    // cout << endl;
    if (current.id == dest) {
      cout << "Found\n";
      cout << "For V=" << num_of_nodes << '\n';
      cout << current.g_val << '\n';
      reconstruct_path(came_from, current.id);
      break;
    }
    // PQ.extract_min();
    // cout << "Extracted" << current.id << '\n';
    id src = current.id;
    for (auto &kv : G.adjList[src]) {
      auto &k = kv.first;
      auto &v = kv.second;
      if (ptr_map[k] == nullptr)
        continue;

      float tentative_gscore = current.g_val + v.weight;

      if (tentative_gscore < ptr_map[k]->data.g_val) {
        came_from[k] = src;
        Node_Data replacement =
            ptr_map[k]
                ->data; // copy constructor is defined so we can just do this
        replacement.g_val = tentative_gscore;
        replacement.f_val = replacement.g_val + replacement.h_val;
        // cout << "Replacing " << k << endl;
        PQ.decrease_key(ptr_map[k], replacement);
        // cout << "Replaced " << k << endl;
        // PQ.print();
        // cout << endl;
      }
    }
  }
}
} // namespace Fib

Graph<Data> generate_random_graph(int V) {
  srand(time(NULL));
  Graph<Data> G;
  for (int i = 0; i < V; i++) {
    for (int j = i + 1; j < V; j++) {
      // if (rand() % 2)
      G.insertEdge(i, j, Data(rand() % 10000));
    }
  }
  return G;
}

void print_graph(Graph<Data> &G) {
  for (size_t i = 0; i < G.adjList.size(); i++) {
    cout << i << " : ";
    for (auto &x : G.adjList[i]) {
      cout << "(" << x.first << "," << x.second.weight << ") ";
    }
    cout << '\n';
  }
  cout << '\n';
}

int main() {

  int size{10};
  Graph<Data> G;
  G.insertEdges(0, {1, 2, 3}, {10, 11, 20});
  G.insertEdges(1, {8, 4}, {15, 16});
  G.insertEdges(2, {5}, {Data(20)});
  G.insertEdges(3, {2, 4}, {12, 18});
  G.insertEdges(4, {8, 5}, {8, 18});
  G.insertEdges(5, {8, 6}, {25, 8});
  G.insertEdges(6, {7, 8}, {6, 25});
  G.insertEdges(7, {9}, {Data(9)});
  G.insertEdges(8, {9}, {Data(30)});
  G.insertEdges(9, {5, 6}, {29, 7});
  // G.add
  cout << "Graph G\n";
  print_graph(G);
  cout << "\n";
  int dest = size - 1;
  fill_node_to_coord(size);
  cout << "Node and their co-ordinates:\n";
  for_each(node_to_coord.begin(), node_to_coord.end(), [](const auto &x) {
    cout << x.first << ":"
         << "(" << x.second.first << "," << x.second.second << ")\n";
  });
  cout << "\n";
  fill_h_map(dest);
  cout << "Node and their distances from destination(Node " << dest << "):\n";
  for_each(h_map.begin(), h_map.end(), [](const auto &x) {
    cout << x.first << ":" << x.second << " units\n";
  });
  cout << "\n";

  // src for all A_star functions will be 0
  cout << "Binary Heap A*\n";
  auto time_elapsed1 = timeMyFunction(BinHeap::A_star, std::ref(G), dest, size);
  cout << '\n';
  cout << "Fibonacci Heap A*\n";
  auto time_elapsed2 = timeMyFunction(Fib::A_star, std::ref(G), dest, size);
  cout << '\n';
  cout << "Pairing Heap A*\n";
  auto time_elapsed3 =
      timeMyFunction(PairHeap::A_star, std::ref(G), dest, size);
  cout << '\n';
  // bin << fixed << setprecision(30) << size << ":" << time_elapsed1 << '\n';
  // fib << fixed << setprecision(30) << size << ":" << time_elapsed2 << '\n';
  // pairing << fixed << setprecision(30) << size << ":" << time_elapsed3 <<
  // '\n'; cout << '\n';

  return 0;
}
