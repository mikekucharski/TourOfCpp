/** 
 * Chapter 4: Classes
*/
#include<initializer_list>
#include<iostream>

// This is the custom class which we want to wrap with a container.
class Vector {
public:
  // inline constructor using "member initializer list".
  Vector(int s) : elem{new double[s]}, sz{s} {
    for (int i = 0; i < s; i++) {
      elem[i] = 0;
    }
  }

  // constructor which allows inializer list ie. Vector v {1.0, 2.0};
  Vector(std::initializer_list<double>);

  ~Vector() {
    delete[] elem;
  }

  void push_back(double);

  // operator overload. use double& to allow reading and writing.
  double& operator[](int i) {
    return elem[i];
  }
  
  int size() const { return sz; }

private:
  double* elem;
  int sz;
};

Vector::Vector(std::initializer_list<double> list) 
: elem{new double[list.size()]},
  // must use static_vase to int because the std library ie. list.size() uses
  // unsigned int.
  sz{static_cast<int>(list.size())} {
    
  std::copy(list.begin(), list.end(), elem);
}

// Defines a class to serve as an interface.
class Container {
public:

// Note, no constructor! This is common for an abstract class, because
// after all there is no private data to initialize. What would the 
// args even be?

// "virtual" keyword means the implementation is in a derived class. Virtual
// is similar to "abstract" in java.
// Setting a virtual function "=0" means it's a pure virtual function. ie. the
// subclass MUST define the function. If there is even 1 pure virtual function,
// then the class can not exist on it's own. Cannot make a "Container c;"
virtual double& operator[](int) = 0; // pure virtual function

// const member function, const means the func cannot change members of the class.
virtual int size() const = 0;

// destructor. Classes with virtual functions should define virtual destructors.
virtual ~Container() {}              
};

// Implementation of a Container.
class Vector_container : public Container {
public:
  // constructor.
  Vector_container(int s) : v(s) {}
  
  // destructor. note, this overrides the base class destructor ~Container()
  // Invoking this will implicitly invoke the ~Vector() destructor.
  ~Vector_container() {}

  // override to make it explicit & add compiler checks that we have the 
  // correct signature.
  double& operator[](int i) override {
    return v[i];
  }

  int size() const override {
    return v.size();
  }
private:
  Vector v;
};

// Take in a reference to any generic Container (abstract class). Loop over its
// size and invoke the index operator.
void useGeneric(Container& con) {
  for (int i = 0; i < con.size(); ++i) {
    std::cout << con[i];
  }
  std::cout << std::endl;
}


int main(int argc, char* argv[]) {
  Vector v {1.0, 2.0};

  // Flexible but must be manipulated using pointers.
  Container* c = new Vector_container(3);
  (*c)[0] = 1;
  (*c)[1] = 2;
  (*c)[2] = 3;
  useGeneric(*c);

  // similar to instanceof in java, we can check the concrete type by using
  // dynamic_cast<TYPE>(INSTANCE) and seeing if it returns a nullptr (fail)
  if (Vector_container* p = dynamic_cast<Vector_container*>(c)) {
    // If the pointer is valid (not nullptr) we will enter this block.
    // Now we can use speific methods of the subclass.
  } else {
    // nullptr evaluates to false. Not a Vector_container...
  }

  // Can use dynamic_cast but to a reference type, in this case it throws a
  // bad_cast exception if the type is not the exact requested type.
  // This will not throw, because we know *c is a Vector_container.
  Vector_container& enforceMe = dynamic_cast<Vector_container&>(*c);

  // always remember to delete pointers, even these interface ones. Deleting
  // will invoke the lowest-defined destructor, and proceed up the inheritance
  // hierarchies invoking destructors to make sure all resources are freed.
  delete c;

  // Closing note, remembering to delete these pointers is dangerous. You can
  // use unique_ptr<TYPE> in private members of classes. Then, the destructors
  // are automatically created for you, when the unique_ptr goes out of scope.
  // Can also use shared_ptr.

  return 0;
}
