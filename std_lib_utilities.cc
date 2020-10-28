#include <any>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>
using namespace std;

class Foo {};

class Entry {
 public:
  Entry(string k, int v) : key{k}, value{v} {
  }

  string getKey() {
    return key;
  }
  int getValue() {
    return value;
  }

 private:
  string key;
  int value;
};

void smart_pointers() {
  // Traditional free-store pointer. Error-prone.
  Foo* p = new Foo();
  delete p;

  // explciitly pass a new instance of Foo to the unique_pointer constructor.
  // Now, fp works just like a Foo* pointer. The only difference is that when fp
  // falls out of scope (method return OR exception thrown, anything) it will be
  // destroyed.
  unique_ptr<Foo> fp{new Foo()};

  // unique_pointer is unique in the sense that you cannot just assign it to
  // another variable. The fp pointer is the only one that can refer to the
  // instance now. If you want to give up ownership, you have to explicitly move
  // it. Move doesn't actually move anything, it casts the type to an "rvalue".
  // Like all casting, use with caution.
  //
  // unique_ptr<Foo> fp2 = fp; // compile-error. Cannot reassign fp.
  unique_ptr<Foo> fp2 = move(fp);  // now fp is nullptr.

  // shared pointer can have multiple copies, it is copied on assignment. The
  // data is only deleted when the underlying object is not referred to by any
  // more shared pointers.
  shared_ptr<Foo> sp{new Foo()};

  // Making unique/shared ptrs is easier & less error prone like this:
  auto p1 = make_unique<Entry>("mike", 5);
  auto p2 = make_shared<Entry>("john", 10);
  cout << p1->getKey() << p2->getKey() << endl;

  // works with primitives too
  auto number = make_unique<int>(5);
  auto decimal = make_unique<double>(5.2);
}

void specialized_containers() {
  // - array<T, N> = solves some problems with T[N]. array<int, 3> a1 = {1,2,3};
  // Then if you need to use it as an old C-style array, you can do so with
  // a1.data() or &a1[0]. This is allocated on the stack.
  // - bitset<N> = fixed size sequence of N bits
  // - vector<bool> = sequence of bits stored in a specialization of vector
  // - pair<T, U> = pair of elements
  // - tuple<T...> = sequence of arbitrary number of elements of arbitrary type.
  // - basic_string<C> = this is what std::string is under the hood

  vector<string> list{"foo", "bar"};
  pair p1{list.begin(),
          2};  // one way to make pair<vector<string>::iterator, int>
  auto p2 = make_pair(list.begin(), 2);  // another way

  tuple<string, int, double> t1{"shark", 123, 5.2};
  auto t2 = make_tuple("shark", 123, 5.2);  // another way
  tuple t3{"shark", 123, 5.2};  // C++17 deduce the types from the arguments

  // access tuple by index, error prone.
  cout << get<0>(t1) << get<1>(t1) << get<2>(t1) << endl;
  // access tuple by types. Throws compile time error if the type is not
  // correct. I think if there are 2 types the same it does not work.
  cout << get<string>(t1) << get<int>(t1) << get<double>(t1) << endl;
}

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};  // (1)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;  // (2)

optional<string> maybeGetString() {
  if (1 == 1) {
    return "foo";
  }
  return {};  // empty optional.
}

void alternatives() {
  // variant - representes one of a specified set of alternatives
  // optional - value is populated with specified type, or has no value
  // any - represents one of an unbounded numbrs of types

  variant<string, int> myVariant;
  if (1 == 1) {
    myVariant = "foo";  // string type. Probably invokes the 1-arg constructor.
  } else {
    myVariant = 5;  // number type. Probably invokes the 1-arg constructor.
  }
  if (holds_alternative<string>(myVariant)) {
    // check if string type assigned
    cout << "variant holds a string!" << endl;
  } else if (holds_alternative<int>(myVariant)) {
    // check if int type assigned
    cout << "variant holds a string!" << endl;
  }
  // another way to test using lamdas, but requires the weird template
  // declaration above.
  visit(overloaded{
            [](int x) { cout << "variant holds an int!" << endl; },
            [](string s) { cout << "variant holds a string!" << endl; },
        },
        myVariant);

  // use optional. However, trying to use an optional by dereference * without
  // checking existence (the if statement) does not throw an exception like in
  // java. It has undefined as the result.
  if (auto maybeString = maybeGetString()) {
    cout << "optional has a value and it is " << *maybeString << endl;
  }

  // works just like an unconstrained version of variant above.
  any myAny = "any string";  // now it's a string.
  myAny = 5;                 // now it's a number
  // access the value of the any by type. throws bad_any_access if not that
  // type.
  int x = any_cast<int>(myAny);
}

int main(int argc, char* argv[]) {
  smart_pointers();
  specialized_containers();
  alternatives();
  // There is an allocators concept to allocate large amounts of space at once
  // from the free store as opposed to individual objects using new/delete. All
  // of the std lib containers support being constructed with an allocator pool
  // to use that instead. The default remains new/delete.

  // use <chrono> package for time.

  // use external library called "span" as opposed to 2 separate method
  // arguments "pointer + length of elements" convention. Span tries to reduce
  // errors by checking the range, so programmers can't accidentally give a
  // mismatch number that doesn't match the actual # of elements. which is being
  // integrated into the standard lib.
  return 0;
}