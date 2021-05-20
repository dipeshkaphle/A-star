#include "BinaryHeap.cpp"
#include "FibonacciHeap.cpp"
#include "Pair.cpp"
#include "utility_classes.cpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>

using namespace std::placeholders;

int main() {

  std::cout << "Started measuring merge operations.\n";
  std::ofstream joins("joins.txt");
  //

  for (int i = 200; i <= 5000; i += 200) {
    int n = i;
    std::vector<int> all_entries(n);
    std::iota(all_entries.begin(), all_entries.end(), 1);
    int j = 0;
    BinaryHeap<int> b_h1({}), b_h2({});
    FibHeap<int> f_h1(INT32_MIN), f_h2(INT32_MIN);
    PairingHeap<int> p_h1, p_h2;
    for (int x : all_entries) {
      if (j % 2) {
        b_h1.insert(x);
        p_h1.insert(x);
        f_h1.insert(x);
      } else {
        b_h2.insert(x);
        f_h2.insert(x);
        p_h2.insert(x);
      }
      j++;
    }
    vector<int> res_b, res_f, res_p;
    // merges
    auto time_f = timeMyFunction(std::bind(&FibHeap<int>::merge, &f_h1, _1),
                                 std::ref(f_h2));
    auto time_b = timeMyFunction(std::bind(&BinaryHeap<int>::merge, &b_h1, _1),
                                 std::ref(b_h2));
    auto time_p = timeMyFunction(std::bind(&PairingHeap<int>::merge, &p_h1, _1),
                                 std::ref(p_h2));
    joins << std::fixed << std::setprecision(30) << n / 2 << " " << time_f
          << " " << time_b << " " << time_p << '\n';
    // b_h1.merge(b_h2);
    // f_h1.merge(f_h2);
    // p_h1.merge(p_h2);

    while (!b_h1.empty()) {
      res_b.push_back(b_h1.get_min());
      b_h1.extract_min();
      res_f.push_back(f_h1.extract_min());
      res_p.push_back(p_h1.extract_min_and_get_val());
    }
    assert(is_sorted(res_b.begin(), res_b.end()));
    assert(is_sorted(res_f.begin(), res_f.end()));
    assert(is_sorted(res_p.begin(), res_p.end()));
  }

  std::cout
      << "Done. Please run python plot_joins.py to see the comparision plot";
}
