#include <iostream>
#include <vector>
using namespace std;

/** Read a sequence of ints, until the terminator. If the */
vector<int> read_ints(istream& is, const string& terminator) {
  vector<int> res;

  // This workds because the >> operator returns the stream (for chaining) and
  // testing a stream for true will fail if the input is not of the right input
  // type (int in this case).
  for (int i; is >> i;) {
    res.push_back(i);
  }

  if (is.eof()) {
    cout << "eof detected" << endl;
    return res;
  }

  if (is.fail()) {
    is.clear();  // reset the input stream state to good.
    is.unget();  // put the non-digit back in the stream.
    string s;
    // Again, returned cin will be false when the input was not a string.
    // Can compare string with ==
    if (cin >> s && (s == terminator)) {
      cout << "terminator chars detected" << endl;
      return res;
    }

    // Force the cin to a fail state if it's not the right string.
    is.setstate(ios_base::failbit);  // add fail() to cin's state
  }
  return res;
}

struct Entry {
  string key;
  int number;
};

/** serialize an Entry to string. */
ostream& operator<<(ostream& os, const Entry& e) {
  return os << "{\"" << e.key << "\", " << e.number << "}";
}

// Note, >> skips whitespace but is::get does not
string getValueInQuotes(istream& is) {
  string result;
  char c;
  if (is >> c && c == '"') {
    // read 1 char at a time until we hit the closing quote.
    while (is.get(c) && c != '"') {
      result += c;
    }
  }
  return result;
}

/** deserialize an Entry from string. */
istream& operator>>(istream& is, Entry& e) {
  char c, c2;
  if (is >> c && c == '{') {
    string key = getValueInQuotes(is);
    cout << "got key!" << key;
    if (is >> c2 && c2 == ',') { 
      int number = 0;
      if (is >> number >> c && c == '}') {
        // Only set the key and number if both were successfully read.
        e = {key, number};
        return is; 
      }
    }
  }
  is.setstate(ios_base::failbit); // fail state.
  return is;
}

int main(int argc, char* argv[]) {
  cout << "give me a sequence of ints, or \"stop\" to finish" << endl;
  auto ints = read_ints(cin, "stop");

  int sum = 0;
  for (auto& x : ints) {
    sum += x;
  }
  cout << "Sum is " << sum << endl;

  cin.clear();
  
  Entry e1 {"mike", 1};
  // Invokes the operator define on the User-Defined type (Entry).
  cout << e1 << endl;

  cout << "give me an Entry like this: {\"mike\": 5}: ";
  Entry e2;
  cin >> e2;
  cout << "Got " << e2 << endl;


  // Formatters
  constexpr double d = 123.456;
  cout << d << " : "
       << scientific << d << " : "
       << hexfloat << d << " : "
       << fixed << d << " : "
       << defaultfloat << d << endl;

  // Support for filestystem stuff:
  // path - directory path
  // filesystem_error - fs exception
  // diretory_iterator - iterate over a directory
  // recursive_directory_terator - dir and sub dirs
   
  return 0;
}