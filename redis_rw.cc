/*
 * Copyright (c) 2017-2018 S.Y.Z
 *
 * https://github.com/redis/hiredis.git
 *
 * g++ redis_rw.cc -std=c++11 -I$HIREDIS_HOME/include -L$HIREDIS_HOME/lib
 * -lhiredis -o release/redis_rw
 *
 * ./redis_rw localhost key value
 * ./redis_rw localhost key
 */

#include <hiredis/hiredis.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("usage: redis_rw localhost key value\n");
  }
  const char* host = argv[1];
  const char* key = argv[2];
  const char* value = argv[3];
  int32_t port = 6379;
  struct timeval timeout = {1, 500000};

  redisContext* prc_ = nullptr;
  do {
    std::string cmd;
    redisReply* reply = nullptr;
    if (argc > 3) {
      cmd = "SET ";
      cmd += key;
      cmd += " ";
      cmd += value;
    } else {
      cmd = "GET ";
      cmd += key;
    }

    prc_ = redisConnectWithTimeout(host, port, timeout);
    if (prc_ == nullptr) break;
    if (prc_->err) {
      std::cout << prc_->errstr << std::endl;
      break;
    }

    reply = static_cast<redisReply*>(redisCommand(prc_, cmd.c_str()));
    std::cout << reply->str << std::endl;

    freeReplyObject(reply);
  } while (0);
  redisFree(prc_);
  return 0;
}
