#include<iostream>
#include<map>

// Note: Recommendation is to NOT put 'using' declarations in header files.
using namespace std;

// Defines it's own scope.
namespace My_code {
  int x = 5;

  int bestNumber() {
    return 5;
  }
}

// Can separate a class interface from the impl. Better to put the interface
// into a .h file and include it in the .cpp implementation.
class Foo {
  public:
    int getFooNum();
};

int Foo::getFooNum() {
  return 7;
}

void namespaces() {
  
  Foo foo;
  cout << foo.getFooNum() << endl;

  // reference by namespace. This is preferred because there can easily be colisions 
  // esp in std lib code.
  cout << My_code::bestNumber() << My_code::x << endl;
}

// How to define a custom exception.
struct MyException : public exception {
   const char* what() const throw () {
      return "my custom C++ Exception";
   }
};

void funcWhichThrows() {
  throw out_of_range("out of range exception!");
}

// functions can also be marked with noexcept. It means that exception must not
// be thrown from it, and if they are then terminate() is called.
//
// In C++ we should handle errors by choosing between throwing an expception, 
// terminating the program with terminate()/abort()/exit(), and returning an 
// error code.
void handleException() {
  try {
    funcWhichThrows();
  } catch (out_of_range& err) {
    cout << "caught exception" << err.what() << endl;
  } catch (MyException& err) {
    cout << "caught custom exception" << err.what() << endl;
  }
}

void defaultValue(int x = 100) {
  cout << x << endl;
}

class Matrix {};

// const pass-by-ref because matrices could be large, and adding should not modify them.
// But what about return value? It will be copied on return. 2 options:
// 1) Move consttructor (discussed later in Essential Operators chapter)
// 2) OLD WAY: return a pointer to the object on the heap. Major source of bugs :(. You 
// also have to remember to delete the returned pointer. Easily forgotten. Prefer option 1.
Matrix operator+(const Matrix& m1, const Matrix& m2) {
  Matrix result;
  return result;
}

// Return type is inferred. Dangerous.
auto bestNumber() {
  return 5;
}

void functionArgsAndReturnValues() {
  // Function Arguments
  // Note, for function arguments, prefer pass-by-value for smaller types 
  // (2-3 pointers). Prefer pass-by-reference (using & operator) for large 
  // values which you don't want to copy. If you don't need to modify the 
  // value, use const. Basically always prefer constant pass-by-ref.

  // Function Return Values
  // Note, usually you return-by-value. However, it's possible to return by
  // reference. An example could be a vector get(int pos) method. The elements 
  // of the vector exist regardless of the method call to get an element. Local
  // variables disappear after the function is returned, so returning refs to 
  // those variables won't work.

  // Expensive return values
  Matrix m1, m2;
  Matrix m3 = m1 + m2;

  cout << bestNumber() << endl; // type inferred to be int. Dangerous...
}

class Entry {
public:
  string key;
  int value;
};

// C++17 feature. Cool, but recommended to prefer structured return types.
void structuredBindings() {
  Entry entry;
  entry.key = "key1";
  entry.value = 5;

  // You can unpack members of the class into local variables.
  auto [k,v] = entry;
  cout << k << v << endl;

  map<string, int> myMap {
    {"key1", 1},
    {"key2", 2}
  };
  // Bind k,v as references as to modify the count.
  for (auto& [k,v] : myMap) {
    ++v;
  }
  // Bind k,v to values from the map.
  // can use const when using structured binding
  for (const auto [k,v] : myMap) {
    cout << k << " -> " << v << endl;
  }
}

int main(int argc, char* argv[]) {
  namespaces();
  handleException();
  defaultValue();
  functionArgsAndReturnValues();
  structuredBindings();
  return 0;
}