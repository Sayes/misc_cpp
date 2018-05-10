/*
 * Copyright (c) 2017-2018 S.Y.Z
 * https://github.com/mongodb/mongo-cxx-driver
 *
 * g++ mongodb_conn.cc -std=c++11 -I/usr/local/include/libmongoc-1.0
 * -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi
 * -I/usr/local/include/libbson-1.0 -lbsoncxx -lmongocxx -o release/mongodb_conn
 */

#include <stdio.h>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/cursor.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

int main(int argc, char* argv[]) {
  int result = -1;
  do {
    try {
      mongocxx::uri uri_("mongodb://localhost:27017");
      mongocxx::instance inst_;
      mongocxx::client client_;
      mongocxx::database db_;

      client_ = mongocxx::client(uri_);
      db_ = client_["local"];

      mongocxx::options::find opts;
      bsoncxx::builder::stream::document order_builder;
      order_builder << "id_" << -1;
      opts.sort(order_builder.view());
      bsoncxx::builder::stream::document filter;
      filter << "pid" << bsoncxx::builder::stream::open_document << "$gt"
             << 10000 << bsoncxx::builder::stream::close_document;
      filter << "pid" << bsoncxx::builder::stream::open_document << "$lt"
             << 20000 << bsoncxx::builder::stream::close_document;

      mongocxx::cursor cursor_ = db_["startup_log"].find(filter.view(), opts);

      mongocxx::cursor::iterator it = cursor_.begin();
      for (; it != cursor_.end(); ++it) {
        bsoncxx::document::view view = *it;
        bsoncxx::document::element element{view["pid"]};
        printf("%lu\n", element.get_int64().value);
      }

      result = 0;
    } catch (mongocxx::query_exception e) {
    }
  } while (0);
  return result;
}
