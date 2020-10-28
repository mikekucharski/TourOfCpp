#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
using namespace std;

struct Entry {
  string name;
  int value;
};

void std_vector() {
  vector<Entry> phone_book = {{"David", 123}, {"John", 456}};

  // can access elements with brackets, but no range check
  cout << phone_book[0].name << endl;
  // Access elements with range check. Throws out_of_range exception
  cout << phone_book.at(0).name << endl;

  vector<double> v(5);        // size 5
  vector<double> v2(5, 4.2);  // size 5, default values 4.2
  v.push_back(100);           // add 100 to the end, increases the size by 1

  // copying can be inefficient if the vector is large, prefer move operations
  // and pointers/references.

  // When you add an element to a vector, it actually adds the element itself.
  // Therefore, if you have a class hierarchy that relies on virtual functions,
  // it's better to store a pointer.
  vector<Entry> vs;                // Bad
  vector<Entry*> vps;              // Better
  vector<unique_ptr<Entry>> vups;  // OK
}

void std_list() {
  // Note, prefer vector over list because it's better for traversal
  // (find/count)

  // doubly linked list, useful when we want to add/delete elements without
  // moving other elements.
  list<Entry> phone_book = {{"David", 123}, {"John", 456}};

  // convenient looping
  for (const auto& entry : phone_book) {
    if (entry.name == "mike") {
      // ...
    }
  }

  // loop using iterators. This is what the short-hand loop uses under the hood.
  for (auto p = phone_book.begin(); p != phone_book.end(); ++p) {
    if (p->name == "mike") {
      // ...
    }
  }

  // These work with list<T>::iterator (which are like pinters) so insert adds
  // after the element pointed to, erase removes the element pointed to.
  phone_book.insert(phone_book.begin(), {"mike", 5});
  phone_book.erase(phone_book.begin());  // Removes the element from the list.

  // There is also a singly linked list. Doesn't keep backward pointers OR the
  // size.
  forward_list<Entry> singlyLinkedList = {{"David", 123}, {"John", 456}};
}

// Custom hash function for Entry custom type. Typical impl - just xor the hash
// function results of built-int types.
namespace std {
  template <>
  struct hash<Entry> {
    size_t operator()(const Entry& ref) const {
      return hash<string>()(ref.name) ^ hash<int>()(ref.value);
    }
  };
}  // namespace std

void std_map() {
  // map is assoctiative array/dictionary. Usually implemented with red-black
  // tree.
  map<string, int> phone_book{{"David", 123}, {"John", 456}};

  // Note, subscripting into the map will actually insert a default value into
  // it if not found. You can avoid this behavior by using find() and insert()
  // instead of [].
  cout << phone_book["David"] << endl;

  // Lookup in map is O(logn). If we want a hashmap, use unordered_map.
  unordered_map<string, int> phone_book2{{"David", 123}, {"John", 456}};

  // Hashing for the keys is provided for all built-int types. You need to
  // define your own hash function for custom types.
  unordered_map<Entry, int> customTypeAsKey;
}

int main(int argc, char* argv[]) {
  std_vector();
  std_list();
  std_map();

  // More
  // - deque<T> = double-ended queue
  // - set<T> = set, ie. map with just a key and no value
  // - multiset<T> = set in which the value can appear multiple times.
  // - multimap<T> = map in which the key can appear multiple times (ie.
  // multiple values for the same key)
  // - unordered_multimap = hash map whose key can appear multiple times.
  // - unordered_set<T> = set using hashed lookup
  // - unordered_multiset<T> = set whose values can appear more than once, and
  // uses hashed lookup

  // Common operations on container c
  // - c.begin() = pointer to the first element
  // - c.end() = pointer to the last element
  // - c.size() = number of elements in c
  // - c.empty() = if c is empty
  // - c.capacity() = number of elements that c can hold without a new memory
  // allocation
  // - c.reserve(k) = make the capacity k by allocation new memory
  // - c.resize(k) = make the number of elements k
  // - c[k] = index, no range check
  // - c.at(k) = index, range check
  // - c.push_back(x) = add x to the container, increases size by 1
  // - c.emplace_back(a) = pass the arguments for a new element directly, inside
  // it push_back(T{a})
  // - c.insert(p, x) = insert new element x after pointer p
  // - c.erase(p) = remove element pointed at by p.

  return 0;
}