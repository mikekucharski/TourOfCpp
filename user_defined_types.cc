#include<iostream>
#include<variant>
using namespace std;

struct Vector1 {
  int sz;
  double* elem; // pointer to elements
};

void vector_init(Vector1& v, int size) {
  v.elem = new double[size];
  v.sz = size;
}

void pass_struct(Vector1 v, Vector1& rv, Vector1* vp) {
  cout << "Size of vector is: " << v.sz << rv.sz << vp->sz << endl;
}

void structs() {
  Vector1 v;
  vector_init(v, 5);
  pass_struct(v, v, &v);
}

class Vector {
  public:
    // inline constructor using "member initializer list".
    Vector(int s) :elem{new double[s]}, sz{s} {}

    // operator overload. use double& to allow reading and writing.
    double& operator[](int i) { return elem[i]; }
    
    int size() { return sz; }
  private:
    double* elem;
    int sz;
};

void classes() {
  Vector v(5);
  cout << v[3]; // 0

  double d = v[3];
  d = 6;
  cout << v[3]; // 0
  cout << d; // 6

  v[3] = 6;
  cout << v[3]; // 6

  cout << endl;
}

enum Type {ptr, num};
// Union is a struct, but all members are allocated starting at the same address.
// It's basically an either-or. You can either have the integer OR the pointer.
// This means it occupies only as much space as it's biggest member.
union Value {
  int number;
  void* randomPointer;
};
// Key value storage. The key is a string, the type of Value is known by looking at the Type t.
// NOTE: maintaining this mapping between type and union is ERROR PRONE. You can encapsulate the 
// two using a class and make sure the public funcs keep these two in sync, or you can just 
// use the standard lib type variant.
struct Entry {
  string key;
  Type t;
  Value v;

  variant<void*, int> v2;
};

void unions() {
  Entry entry;
  entry.key = "key1";
  entry.t = num;
  entry.v = Value {5};

  // only try to access the number if the type is num.
  if (entry.t == num) {
    cout << entry.v.number << endl; 
  }
}

// Note, declaring as "class" makes the enum values scoped to the Enum name ie. Color.
// Color::red is now different than Traffic_light::red.
enum class Color {red, green, blue};
enum class Traffic_light {green, yellow, red};

void enums() {
  Color col = Color::red;
  // Color col2 = Traffic_light::red; // compile error.
  // Color col2 = 5; // compile error.
  Color col2 = Color{2};
  Color col3 {1};
}

int main(int argc, char* argv[]) {
  structs();
  classes();
  unions();
  enums();
  return 0;
}