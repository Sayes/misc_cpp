/*
 * Copyright (c) 2017-2018 S.Y.Z
 *
 * g++ trick.cc -std=c++11 -o release/trick_cc
 */

#include <stdio.h>
#include <string.h>
#include <boost/container/flat_map.hpp>
#include <iostream>
#include <string>
#include <vector>

// support try_catch yes or not, from boost
#if !(defined EUPU_NO_EXCEPTIONS)
#define EUPU_TRY \
  {              \
    try
#define EUPU_CATCH(x) catch (x)
#define EUPU_RETHROW throw;
#define EUPU_CATCH_END }
#else
#define EUPU_TRY \
  {              \
    if (true)
#define EUPU_CATCH(x) else if (false)
#define EUPU_RETHROW
#define EUPU_CATCH_END }
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

  // read tables
  // line1 aa bb cc dd
  // line2 ee ff gg hh
  FILE* fp = fopen("test_trick_table.txt", "r");
  if (fp == nullptr) return 0;
  char* line = nullptr;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, fp)) != -1) {
    char* p = line;
    char* token;
    char* saveptr;
    for (int k = 0;; ++k, p = nullptr) {
      token = strtok_r(p, " \t\n", &saveptr);
      if (token == nullptr) {
        printf("\n");
        break;
      }
      printf("%s\t", token);
    }
  }

  // boost::container::flat_map
  boost::container::flat_map<int, std::string> map1;
  map1[0] = "kk";
  map1[1] = "ss";
  map1[2] = "uu";

  for (auto it : map1) {
    printf("%d %s\n", it.first, it.second.c_str());
  }

  return 0;
}
