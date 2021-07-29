/*
 *
 * Copyright (c) 2017-2018 SYZ
 *
 * https://github.com/yhirose/cpp-httplib.git
 *
 * g++ httplib_op.cc -std=c++11 -I/home/syz/workshop/opensource/cpp-httplib
 * -L/home/syz/workshop/opensource/cpp-httplib/out/build_20210417_x86_64
 * -lhttplib -o release/httplib_op_cc
 *
 */

#include <httplib.h>
#include <iostream>

int main(int argc, char** argv) {
  httplib::Client cli_get("127.0.0.1", 8139);
  auto res_get = cli_get.Get(
      "/v1/channel/image/yuv/raw?channel_id=800f032d34e9449ba89fd3b921fb1365");
  if (res_get) {
    std::cout << res_get->get_header_value("content-type") << std::endl;
    std::cout << res_get->get_header_value("accept-ranges") << std::endl;
    std::cout << res_get->get_header_value("content-length") << std::endl;
    std::cout << res_get->get_header_value("yuv-width") << std::endl;
    std::cout << res_get->get_header_value("yuv-height") << std::endl;
    std::cout << res_get->body << std::endl;
  } else {
    std::cout << "res_get null" << std::endl;
  }

  std::multimap<std::string, std::string, httplib::detail::ci> headers;
  headers.emplace("Content-Type", "application/json; charset=utf-8");
  httplib::Client cli_post("127.0.0.1", 8080);
  auto res_post = cli_post.Post("/", headers, R"({"val":20})",
                                "text/html; charset=ISO-8859-1");
  if (res_post) {
    std::cout << res_post->body << std::endl;
  } else {
    std::cout << "res_post null" << std::endl;
  }

  return 0;
}
