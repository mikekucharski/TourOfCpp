#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <vector>
using namespace std;

void algs() {
  vector<int> vec;
  list<int> lst;

  // sort in place.
  sort(vec.begin(), vec.end());
  // remove duplicates by copying the values uniquely. Note, it just takes the
  // start of lst and starts overwriting left to right.
  unique_copy(vec.begin(), vec.end(), lst.begin());
  // You can use back_inserter() to dynamically keep appending to lst.
  unique_copy(vec.begin(), vec.end(), back_inserter(lst));

  // find() returns an iterator/pointer to the element, and ::end() if not
  // found.
  string letters = "abcd";
  bool has_letter = find(letters.begin(), letters.end(), 'a') != letters.end();
  cout << has_letter << endl;
}

// Generic method which iterates through a container C, looking for all values
// V. returns a vector of pointers to all elements v.
template <typename C, typename V>
vector<typename C::iterator> find_all(C& c, V v) {
  vector<typename C::iterator> res;
  for (auto p = c.begin(); p != c.end(); ++p) {
    if (*p == v) {
      res.push_back(p);
    }
  }
  return res;
}

void iterators() {
  // Note, iterator impls are kinda specific to each container, but the API
  // works the same always (begin, end, *). So we should just use the API and
  // not care much about impl.
  // ::iterator
  // ::const_iterator

  // Generic alg works on strings.
  string m = "Mary had a little lamb";
  for (const auto p : find_all(m, 'a')) {
    cout << *p;
  }
  cout << endl;

  // Works on vector.
  vector<int> v{1, 2, 3, 2, 1};
  for (const auto p : find_all(v, 2)) {
    cout << *p;
  }
  cout << endl;

  // Btw, you can use predicate, a lambda func to do this is in the standard lib
  // Predicates should not modify the elements.
  map<string, int> entries = {{"one", 1}, {"two", 2}};
  auto p = find_if(entries.begin(), entries.end(),
                   [](const pair<string, int>& ref) { return ref.second > 1; });
}

void std_algs() {
  // Let b = begin pointer, e = end pointer, f = lamdba f(x)
  // - for_each(b, e, f) // apply f to each element
  // - find_if(b, e, f)
  // - count(b, e, f)
  // - count_if(b, e, f)
  // - replace(b, e, v, v2) // replaces v with v2 for all in range [b:e)
  // - replace_if(b, e, f, v2) // if the element matches f(e), then replace it
  // with v2
  // - copy(b, e, out)
  // - copy_if(b, e, out, f)
  // - move(b, e, f)
  // - unique_copy(b, e, out)
  // - sort(b, e)
  // - sort(b, e, f) // uses f as the sorting criteria.

  // You can also request these algs to be parrallelized to take advantage of
  // hardware. Parallel means multiple threads, often running on several
  // processor cores Vectorized SIMD "Single Instruction Multiple Data" means
  // tasks are done on a single thread.
  // - seq = sequential (default), par = parrallel, par_unseq = parallel and/or
  // vectorized
  // - sort(seq, b, e);
}

// Note, you can make short syntax for the algs above like this
namespace Estd {
using namespace std;

template <typename C>
void sort(C& c) {
  sort(c.begin(), c.end());
}

template <typename C, typename Pred>
void sort(C& c, Pred p) {
  sort(c.begin(), c.end(), p);
}
}  // namespace Estd

int main(int argc, char* argv[]) {
  algs();
  iterators();
  std_algs();
  return 0;
}
