/*
 * https://github.com/open-source-parsers/jsoncpp.git
 * v1.8.4
 *
 * g++ json_parse.cc -std=c++11 -I$JSONCPP184_HOME/include
 * -L$JSONCPP184_HOME/lib -ljsoncpp -o release/json_parse
 */

#include <json/json.h>
#include <fstream>
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
  int result = -1;
  do {
    std::ifstream f;
    f.open("./test.json");
    if (!f.is_open()) break;
    f.seekg(0, std::ios::end);
    int64_t flen = f.tellg();
    f.seekg(0, std::ios::beg);

    std::unique_ptr<char> buf(new char[flen]);
    f.read(buf.get(), flen);

    Json::CharReaderBuilder crb;
    crb["collectComments"] = false;
    std::unique_ptr<Json::CharReader> pcr(crb.newCharReader());
    Json::Value v;
    JSONCPP_STRING errs;

    try {
      if (!pcr->parse(buf.get(), buf.get() + flen, &v, &errs)) break;
      std::cout << v["key1"].asString() << std::endl;
    } catch (const std::exception& e) {
      std::cout << "Json::CharReader::parse(" << buf << ") error" << std::endl;
    }

    result = 0;
  } while (0);
  return result;
}
