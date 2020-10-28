#include <iostream>
#include <regex>
#include <string>
using namespace std;

void strings() {
  string first_name = "Mark";
  string full_name = first_name + " Johnson";

  full_name += '\n';  // strings are mutable.

  // start at pos 0 for length 4, "Mark"
  string substring = full_name.substr(0, 4);

  // start at position 1 for length 3, "Mark" becomes "Matt"
  cout << substring.replace(1, 3, "att") << endl;

  substring[1] = toupper(substring[1]);
  cout << substring << endl;  // "MAtt"

  // - lexigraphical ordering from the comparison operators.
  // - comparison using == and != works by characters
  // - supports iterators
  // - can compare against c-style strings just fine

  // String literals
  auto s = "Cat"s;  // the s suffix declares a std::string.
  auto p = "Dog";   // a C-style string: const char*

  // std::string is actually an alias for basic_string<char> where char is the
  // english character type. There is also basic_type<JChar> for japanese
  // characters etc.
}

void string_views() {
  // To pass a readonly substring, you can use "const string&" and pass the
  // start/end indexes. string_view is an absraction on top of this, where the
  // string can be a std::string or a C-string. They should be thought of like
  // pointers, string views "point to" and underlying string.
  string s = "hello world";
  string_view stringView{&s[6], 5};  // "world"
  cout << stringView << endl;
  string_view stringView2 =
      "tom sawyer"sv;  // sv suffix to convert string literal.
  cout << stringView2 << endl;
}

// Note, raw string literals go inside the parns here - R"()"
// Then you don't need to escape backslashes and quotes, which are common in
// regex.
void regexes() {
  regex pat{R"(\d\w{4}\d)"};

  smatch matches;  // matched strings go here.
  string searchString = "hello 5mike5 world";
  if (regex_search(searchString, matches, pat)) {
    cout << "Regex search found " << matches.size() << " matches." << endl;
    cout << matches[0] << endl;  // "5mike5"

    // If there were sub matches
    if (matches.size() > 1 && matches[1].matched) {
      cout << matches[1] << endl;
    }
  }

  // Character Classes go inside [::]. They are names instead of the symbol.
  // They should be used for portability.
  regex pat2{R"([[:digit:]])"};  // same as \d

  // Iterators.
  // Note, sregex_iterator is an alias for regex_iterator<string>
  string input = "aa bb cc dd ee ff";
  regex spaces_pat{
      R"([^\s]+)"};  // any characters followed by any number of whitespace.
  auto words_begin = sregex_iterator(input.begin(), input.end(), spaces_pat);
  auto words_end = sregex_iterator{};
  cout << "Found " << distance(words_begin, words_end) << " words." << endl;
  for (sregex_iterator p = words_begin; p != words_end; ++p) {
    smatch match = *p;
    cout << match.str() << endl;
  }
}

// Note, return strings by value from functions because they have move
// constructor defined. std::string grows in length, no overflow.
int main(int argc, char* argv[]) {
  strings();
  string_views();
  regexes();
  return 0;
}