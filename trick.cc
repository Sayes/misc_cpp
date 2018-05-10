/*
 * Copyright (c) 2017-2018 S.Y.Z
 *
 * g++ trick.cc -std=c++11 -o release/trick
 */

#include <iostream>
#include <string>
#include <vector>

// support try_catch yes or not, from boost
#if !(defined BOOST_NO_EXCEPTIONS)
#define BOOST_TRY \
  {               \
    try
#define BOOST_CATCH(x) catch (x)
#define BOOST_RETHROW throw;
#define BOOST_CATCH_END }
#else
#define BOOST_TRY \
  {               \
    if (true)
#define BOOST_CATCH(x) else if (false)
#define BOOST_RETHROW
#define BOOST_CATCH_END }
#endif

int main(int argc, char* argv[]) {
  // use do_while for gather or release resource when func failed
  char* p = new char[100];
  do {
    // do something here and failed
    break;
  } while (0);
  delete p;
  p = nullptr;

  // connect item with separator
  std::string wantedstr;
  std::vector<std::string> lst = {"one", "two", "three", "four"};
  std::vector<std::string>::iterator it = lst.begin();
  for (; it != lst.end();) {
    wantedstr += *it;
    if (it + 1 != lst.end()) {
      wantedstr += ",";
    }
    it++;
  }
  std::cout << wantedstr << std::endl;
}
