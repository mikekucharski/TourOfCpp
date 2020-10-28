#include<iostream>
#include<complex>
#include<vector>

using namespace std;

// Regular function
double square(double x) {
  return x*x;
}

// Overload
void print(double x) {
  cout << x << endl;
}

// Overload
void print(int x) {
  cout << x << endl;
}

// Some basic types
void types() {
  bool b = true;
  char c = 'G';
  int i = 5;
  float f = 3.2;
  double d = 10.0;
  unsigned u = 2; // non-negative integer. Recommended for bit manipulation only.
  long l = 3.14'159'26535'000; // can use ' to add readability

  complex<double> comp1 = 1;
  complex<double> comp2 {1.5, 2.0}; // equals is optional when using {}
  comp2 = {1.5, 2.0};

  vector<int> list {1,2,3};

  auto b2 = false; // auto can infer the type from the initialization!
  auto c2 = 'H';
  auto list2 = {1,2,3};
}

/**
 * A note on scopes:
 * 1. local = only exists in the method/lambda
 * 2. class = exists for all code in the class
 * 3. namespace = exists until the closing curly of the namespace
 * 4. global = defined not inside any of the above constructs
 */

// Returns a constant. Accepts 3 types of pointers.
constexpr int immutability(
  int* const x,   // pointer which itself cannot be changed (locally, because of pass by ref)
  const int* x2,  // pointer whose contents cannot be changed
  const int* const x3 // BOTH
  ) { 
  int localInt = 50;

  // x = &localInt;   // FAIL, cannot change the pointer.
  *x = 100;           // OK, can change the value of the pointer.

  x2 = &localInt;   // OK, changes the pointer.
  // *x2 = 100;     // FAIL, cannot change the value of the pointer.

  // x3 = &localInt;  // FAIL, cannot change the pointer.
  // *x3 = 100; // FAIL, cannot change the value of the pointer.

  return *x3;
}

void immutability() {
  const double promiseToBeImmutable = sqrt(20); // value can be set at RUNTIME. Good for interfaces.
  constexpr double staticImmutable = 5; // placed in readonly memory. evaluated by COMPILER.
  constexpr int p = 5;
  int p2 = 10;
  int i = immutability(&p2, &p, &p); // note, cannot pass &p as the first argument!
  cout << "Immutable i is: " << i << endl;
}

void arrays() {
  char v[6]; // array of 6 characters. v is a pointer to the first character.
  char* v2;  // Same thing, pointer to a character.

  v2 = &v[4];        // point v2 to the 5th element of v
  char output = *v2; // dereference - get the value of the 5th element of v through the pointer v2

  // Loop over array
  for (auto i = 0; i < 5; ++i) {}

  // For-range (for-in) loop over array. For-range uses iterators under the hood.
  for (auto x : {1, 2, 3}) {
    cout << x;
  }
  cout << endl;

  // For-range loop over array using auto reference. For-range uses iterators under the hood.
  int nums[] = {1, 2, 3};
  for (auto& x : nums) {
    ++x; // add 1 to each element of v
  }
  cout << "Incremented: " << nums[1] << endl;
}

// This is a common way to specify an argument. & will pass by ref, not a copy. 
// const will make it not modifiable.
void pass_by_ref(const int& ref) {
  cout << "const reference: " << ref << endl;
}

void pointers() {
  int x = 5;
  int *p = &x;
  x = *p;      // dereference

  pass_by_ref(x);

  p = nullptr;
  vector<int>* v = nullptr;
  while (p) {} // never runs because it compares p==nullptr by default.
  // Note, there is NO null reference, references must always be defined.

  // Refernces:
  int x = 2;
  int y = 3;
  int& r1 = x;
  int& r2 = y;
  r1 = r2; // Actually reads y through r2 and writes to x through r1. now x == y.
}

void branching() {
  vector<int> v {1,2,3};

  // Limit scope of variable n to the if and else blocks.
  if (auto n = v.size(); n > 10) {}

  // equivalent to adding "n != 0" statement after assignment.
  if (auto n = v.size()) {}
}

int main(int argc, char* argv[]) {
  cout << "Hello world " << square(2) << " " << endl;
  immutability();
  arrays();
  pointers();
  branching();
  return 0;
}
