#include<string>
#include<iostream>

// These operators are essential to make sure resources are freed (memory,
// release lock, etc). They are automatically generated, you can explicitly
// enable some with "= default;" but then automatic generation stops. Or, you
// can use "= delete;" to explicitly turn off just one method generation. The
// default generated constructor just does member-wise construction/copy/move
// which is NOT likely what you want to do with pointer type members, because
// copying a pointer will leave the new pointer pointing to the same object.
class MyType {
 public:
  // Constructor.
  MyType();
  // Copy constructor.
  MyType(const MyType&);
  // Move constructor. Does not take in a const argument, because once the data
  // is copied into this new object, we actually want to remove the value of
  // the argument.
  MyType(MyType&&);
  // "conversion" constructor. If a constructor takes a single arg, it can be
  // directly "assigned". Then you can do MyType mt = 5; This is not intuitive,
  // so always use "explicit" which suppresses this behavior unless wanted.
  explicit MyType(int x);

  // copy assignment.
  MyType& operator=(const MyType&);

  // move assignment.
  MyType& operator=(const MyType&&);

  // Destructor.
  ~MyType();
};

// let a "_km" suffix appear after a long double. It will call this function,
// and return a new long_double.
long double operator "" _km(long double arg) {
  return arg;
}

// Silly example to ignore the input string and returns a different std::string
// completely. The operator "" means you want to define a suffix. There are 
// restrictions on the input types but the output types can be anything. This 
// function is called if the suffix is found on any literal of correct input type.
std::string operator "" custom_suffix(char const* arg, size_t size) {
  return "foo";
}

// RAII = Resource Acquisition Is Initialization. Garbage
// collection only covers memory. What about cleanup of other system resources?
// The main idea of RAII is to use resource handles. Let each resource have an
// owner in some scope and by default be released at the end of it's owners
// scope. Types of resources include:
// - memory (string, vector, map)
// - files (ifstream, ofstream)
// - threads (thread)
// - locks (lock_gard, unique_lock)
// - general objects (through unique_ptr / shared_ptr)


// You can overload different kinds of operators
// - Comparisons (==, !=, <, <=, >, >=)
// - Container operators (size(), begin(), eng())
// - I/O operators ( <<, >> )
// - User-defined literals (above)
// - std::swap() and std::hash<>

int main(int argc, char* argv[]) {
  long double distance = 1.2_km;
  std::string name = "mike"custom_suffix;
  std::cout << distance << name << std::endl;
  return 0;
}