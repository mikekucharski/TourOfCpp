#include <pthread.h>

#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;

// Accept a read-only list of numbers, print them. Also, sum them up and add the
// result to the output pointer, guarded by the lock.
void printListAndSum(const vector<double>& input, double* output, mutex& mux) {
  for (auto i : input) {
    cout << i << endl;
  }

  // Instead of manually lock() and unlock() below, pass the mutex to a
  // scoped_lock which automatically releases itself when the function ends
  // (even if exception). scoped_lock lck { mux };

  mux.lock();
  for (auto i : input) {
    *output += i;
  }
  mux.unlock();
}

// function to do something with v.
void f(vector<double>& v, double* res, mutex& m) {
  printListAndSum(v, res, m);
}

// function object to do something with v.
struct F {
  vector<double>& v;
  double* result;
  mutex& mux;
  // Constructor which accepts the list as input and remember it's reference.
  F(vector<double>& input, double* res, mutex& m)
      : v(input), result(res), mux(m) {
  }
  // operator () is the "function call", "call", or "application" operator.
  void operator()() {
    printListAndSum(v, result, mux);
  };
};

// Two ways: using function or using a Function Object, ie. struct with operator
// () overloaded.
void threads() {
  vector<double> vec1{1, 2, 3};
  vector<double> vec2{4, 5, 6};
  double* result = new double;
  mutex muxInstance;

  // f(vec1) executes in another thread. The first argument is the method to
  // run, and the next are a variable length array of arguments to give to the
  // function.
  thread t1{f, ref(vec1), result, ref(muxInstance)};

  // F() executes in another thread, and the v is stored inside of F already.
  thread t2{F{vec2, result, muxInstance}};

  // lambda executes in another thread.
  thread t3{[]() { cout << "hello" << endl; }};

  // Block until these threads are done running.
  t1.join();
  t2.join();
  t3.join();

  // Notice how the console output from the 3 threads is in a different sequence
  // each time. Calls to cout are not synchornized.

  // Ok threads are guaranteed to be done after join(). Now what about return
  // values? We can pass a non-const refernce object to the threads for them to
  // write to, or a pointer for it to populate the data in. We need to make sure
  // that access to write operations are synchronized, using a lock.
  cout << "The total is: " << *result << endl;

  // Cool! the result 21 is correct. However, manually locking/unlocking the
  // mutex is error prone. You can use wrappers for mutex like this:
  // - shared_lock lck {m}; // Multiple readers can aquire this lock, but when a
  // writer aquires the unique lock, made with the same mutex m, the writer gets
  // exclusive access!
  // - unique_lock lck {m}; // only 1 access at a time. Writers.
  // - scoped_lock lck {m1, m2, m3} // acquire multiple locks.

  // There is a std::condition_variable that you can use to make one thread wait
  // for a condition from the other thread.
}

void futures() {
  // Use futures to set a value from a thread. Can hold the value or an
  // exception.
  promise<string> myPromise;
  myPromise.set_value("any string");
  // Note, doesn't compile if the successful value was already set.
  // myPromise.set_exception(current_exception());

  future<string> myFuture;
  try {
    // Note, this example doesn't compile because myFuture was not assigned. To
    // actually create the future, use std::packaged_task to do
    // packaged_task::getFuture() string result = myFuture.get();
  } catch (bad_cast& ex) {
    // catch a bad_cast, for example.
  }
}

int main(int argc, char* argv[]) {
  threads();
  futures();
  return 0;
}