#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

template <typename EdgeData> class Graph {

public:
  // the index of the vector represents node's name
  // and the vector in that index will have be all the nodes
  // to that node
  vector<unordered_map<int, EdgeData>> adjList;

  Graph() {}
  Graph(unsigned n) { adjList.reserve(n); }
  Graph(const Graph &G) { this->adjList = G.adjList; }
  Graph &operator=(const Graph &G) {
    this->adjList = G.adjList;
    return *this;
  }

  // inserts Edge to the graph. Directed graph edge insertion
  // single edge insertion
  void insertEdge(int srcNode, int destNode, EdgeData data = EdgeData(),
                  bool isUnDirected = false) {
    if (srcNode == destNode) // wont allow loops
      return;
    int x = max(srcNode, destNode);
    if ((unsigned)x >= adjList.size()) {
      int def = x - adjList.size() + 1;
      while (def--)
        this->addNode();
    }
    (adjList[srcNode]).insert({destNode, data});
    if (isUnDirected)
      (adjList[destNode]).insert({srcNode, data});
  }

  // inserts multiple edges. Basically an edge list insertion
  void insertEdges(int srcNode, const vector<int> &nodeList,
                   const vector<EdgeData> &edge_datas,
                   bool isUnDirected = false) {

    for (unsigned i = 0; i < nodeList.size(); i++)
      this->insertEdge(srcNode, nodeList[i], edge_datas[i], isUnDirected);
  }
  void insertEdges(int srcNode, const vector<int> &nodeList,
                   bool isUnDirected = false) {

    for (unsigned i = 0; i < nodeList.size(); i++)
      this->insertEdge(srcNode, nodeList[i], EdgeData(), isUnDirected);
  }

  // adds new Nodes
  bool addNode() {
    // vector<int> *tmp = new vector<int>();
    // adjList.push_back(tmp);
    adjList.emplace_back();
    return true;
  }
  // checks if an edge is present in the adjacency list of node
  bool isEdgePresent(int srcNode, int destNode) {
    int x = max(srcNode, destNode);
    if ((unsigned)x >= adjList.size())
      return false;
    return adjList[srcNode].find(destNode) != adjList[srcNode].end();
  }
  // int getWeight(int srcNode, int destNode) {
  //   if (isEdgePresent(srcNode, destNode))
  //     return adjList[srcNode][destNode];
  //   return std::numeric_limits<int>::max();
  // }
};
