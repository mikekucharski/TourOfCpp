#include <iostream>
#include <stdexcept>
#include <string>

template <typename T>
class Vector {
 public:
  explicit Vector(int s) {
    if (s < 0) {
      throw std::bad_array_new_length();
    }
    elem = new T[s];
    length = s;
  }

  // Allow initializer list.
  Vector(std::initializer_list<T> list)
      : elem{new T[list.size()]},
        // must use static_vase to int because the std library ie. list.size()
        // uses unsigned int.
        length{static_cast<int>(list.size())} {
    std::copy(list.begin(), list.end(), elem);
  }

  ~Vector() {
    delete[] elem;
  }

  // non-const operations
  T& operator[](int i) {
    return elem[i];
  }

  // for const Vectors.
  const T& operator[](int i) const {
    return elem[i];
  }

  // const means it cannot change members of the class.
  int size() const {
    return length;
  }

  // Enables for-range iterator for const and non-const iterator.
  T* begin() {
    return &elem[0];
  }
  const T* begin() const {
    return &elem[0];
  }
  T* end() {
    return &elem[length];
  }
  const T* end() const {
    return &elem[length];
  }

 private:
  T* elem = nullptr;  // pointer to array of any type of data elements.
  int length = 0;     // size of the array.
};

// Use the templated vector like this.
void display(const Vector<std::string>& vs) {
  for (int i = 0; i < vs.size(); ++i) {
    std::cout << vs[i] << std::endl;
  }
}

void displayWithAutoIterator(Vector<std::string>& vs) {
  for (auto& item : vs) {
    std::cout << item << std::endl;
  }
}

// Templates can apply to functions too.
template <typename T>
int displayGeneric(const Vector<T>& nums) {
  for (int i = 0; i < nums.size(); ++i) {
    std::cout << nums[i] << std::endl;
  }
}

// Templates can be used to create "Function Objects" a.k.a. "Functors".
template <typename T>
class Less_than {
  const T val;

 public:
  // constructor.
  Less_than(const T& v) : val{v} {
  }

  // operator which is the function call.
  bool operator()(const T& x) const {
    return x < val;
  }
};

// Take a functor as an argument for the predicate to make generic alg.
// Note, T must be iterable and P must be callable. I guess the code doesn't
// really enforce this.
template <typename T, typename P>
int count(const Vector<T>& vec, P predicate) {
  int cnt = 0;
  for (const auto& item : vec) {
    if (predicate(item)) {
      cnt++;
    }
  }
  return cnt;
}


// Addtional template features below
// 1. Type Aliases.

// Reasonable Map template.
template <typename Key, typename Value>
class Map {};
// Use "using" aliasing to use name String_map instead of the full
// Map<std::string, Value>.
template <typename Value>
using String_map = Map<std::string, Value>;
String_map<int> m;  // now, m is actually of type Map<string, Value>


// 2. Compile-Time if "if constexpr"
template<typename T>
void update(T& target) {
  // 'value' member will be either true or false depending on type T.
  if constexpr(std::is_enum<T>::value) {
    // T is an enum type.
  }
}

int main(int argc, char* argv[]) {
  // Local scope.
  Vector<std::string> strings(5);
  strings[0] = "one";
  strings[1] = "two";
  strings[2] = "three";
  display(strings);

  // Dynamic, needs to be deleted.
  Vector<std::string>* stringsHandle = new Vector<std::string>(5);
  (*stringsHandle)[0] = "one";
  (*stringsHandle)[1] = "two";
  (*stringsHandle)[2] = "three";
  display(*stringsHandle);

  displayWithAutoIterator(strings);
  displayWithAutoIterator(*stringsHandle);
  delete stringsHandle;

  Vector<int> numbers{5, 10};
  displayGeneric(numbers);

  Vector<double> decimals{5.2, 10.2};
  displayGeneric(decimals);

  Less_than my_functor{5};
  std::cout << my_functor(10) << std::endl;  // 0.
  std::cout << my_functor(2) << std::endl;   // 1.

  // Here, we don't have to give a name to the Less_than function object. We can
  // create an instance of it and pass it in directly.
  Vector<int> nums{1, 2, 3, 4};
  std::cout << "Count less than 3 is: " << count(nums, Less_than{3})
            << std::endl;

  // Can do the same initializer list and function call for dynamic memory.
  Vector<int>* nums_on_heap = new Vector<int>({1, 2, 3, 4});
  std::cout << "Count less than 3 is: " << count(*nums_on_heap, Less_than{3})
            << std::endl;
  delete nums_on_heap;

  // Supports lambda expression!
  // The lamda expression generates a function object under the hood.
  // The variables in the current scope are also available to
  // You can configure the capturing of outside variables (like cutoff).
  // - [] = capture nothing. outside variables cannot be used.
  // - [&] = capture all outside variables by reference.
  // - [=] = capture all local names by value
  // - [=cutoff] = capture only specific variable by value
  // - [&cutoff] = capture only specific variable by reference
  int cutoff = 3;
  std::cout << "Count less than 3 is: "
            << count(nums, [&](int a) { return a < cutoff; }) << std::endl;

  // Lamda expressions can be generic! The previous lamda only accepted ints.
  // This means that we could only pass a vector of ints to our count algorithm.
  // However, now we can pass any type of list, and the 'a' in the lamda will
  // be the same type as the item of the list.
  int cutoffForDoubles = 8;
  std::cout << "Count less than 8 is: " << count(decimals, [&](auto& a) {
    return a < cutoffForDoubles;
  }) << std::endl;

  return 0;
}