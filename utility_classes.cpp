#include <cmath>

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
