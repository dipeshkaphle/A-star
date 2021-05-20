#include "BinaryHeap.cpp"
#include "FibonacciHeap.cpp"
#include "Pair.cpp"
#include "utility_classes.cpp"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>

using namespace std::placeholders;

int main() {

  int n = 3000000;
  std::ofstream inserts("inserts.txt");
  std::ofstream get_min("get_min.txt");
  std::ofstream extract_mins("extract_mins.txt");
  std::ofstream decrease_key("decrease_key.txt");

  std::vector<int> all_entries(n);
  std::iota(all_entries.begin(), all_entries.end(), 1);
  std::random_shuffle(all_entries.begin(), all_entries.end());
  FibHeap<int> PQ_fib(INT32_MIN);
  BinaryHeap<int> PQ_bin({});
  PairingHeap<int> PQ_pair;
  unordered_map<int, FibHeap<int>::node *> fib_map;
  unordered_map<int, PairNode<int> *> pair_map;
  int decrease_to = -1;
  int to_insert = n + 1;
  int check_points = 10000;
  for (int j = 0; j < n; j++) {
    int i = j + 1;
    if (i % check_points == 0) {
      /// get min timings
      {
        auto fib_time =
            timeMyFunction(std::bind(&FibHeap<int>::get_min, &PQ_fib));
        auto bin_time =
            timeMyFunction(std::bind(&BinaryHeap<int>::get_min, &PQ_bin));
        auto pairing_time =
            timeMyFunction(std::bind(&PairingHeap<int>::get_min, &PQ_pair));
        get_min << std::fixed << std::setprecision(30) << i << " " << fib_time
                << " " << bin_time << " " << pairing_time << '\n';
      }

      // insert
      {
        auto fib_time = timeMyFunction(
            std::bind(&FibHeap<int>::insert, &PQ_fib, _1), to_insert);

        auto bin_time = timeMyFunction(
            std::bind(&BinaryHeap<int>::insert, &PQ_bin, _1), to_insert);

        auto pairing_time = timeMyFunction(
            std::bind(&PairingHeap<int>::insert, &PQ_pair, _1), to_insert);
        inserts << std::fixed << std::setprecision(30) << i << " " << fib_time
                << " " << bin_time << " " << pairing_time << '\n';
        to_insert++;
      }

      // extract_min timings
      {
        auto fib_time =
            timeMyFunction(std::bind(&FibHeap<int>::extract_min, &PQ_fib));

        auto bin_time =
            timeMyFunction(std::bind(&BinaryHeap<int>::extract_min, &PQ_bin));

        auto pairing_time =
            timeMyFunction(std::bind(&PairingHeap<int>::extract_min, &PQ_pair));
        extract_mins << std::fixed << std::setprecision(30) << i << " "
                     << fib_time << " " << bin_time << " " << pairing_time
                     << '\n';
      }

      // decrease_keys
      {
        auto x = all_entries[j - 1];
        auto fib_time =
            timeMyFunction(std::bind<bool (FibHeap<int>::*)(
                               FibHeap<int>::node *, const int &)>(
                               &FibHeap<int>::decrease_key, &PQ_fib, _1, _2),
                           fib_map[x], decrease_to);

        auto bin_time = timeMyFunction(
            std::bind(&BinaryHeap<int>::decrease_key, &PQ_bin, _1, _2),
            PQ_bin.get_index(x), decrease_to);

        auto pairing_time = timeMyFunction(
            std::bind(&PairingHeap<int>::decrease_key, &PQ_pair, _1, _2),
            pair_map[x], decrease_to);
        decrease_to--;
        decrease_key << std::fixed << std::setprecision(30) << i << " "
                     << fib_time << " " << bin_time << " " << pairing_time
                     << '\n';
      }

      // check_points *= 2;
    } else {
      fib_map[all_entries[j]] = PQ_fib.insert(all_entries[j]);
      pair_map[all_entries[j]] = PQ_pair.insert(all_entries[j]);
      PQ_bin.insert(all_entries[j]);
    }
  }
}
