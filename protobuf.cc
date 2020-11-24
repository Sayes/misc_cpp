/*
 * Copyright (c) 2017-2018 SYZ
 *
 * https://github.com/protocolbuffers/protobuf.git
 * protoc msg.proto --cpp_out=./
 *
 * g++ -std=c++14 protobuf.cc msg.pb.cc -I. -I/usr/local/protobuf/include -L/usr/local/protobuf/lib -lprotobuf
 *
 */

#include <fstream>
// generate with msg.proto, protoc msg.proto --cpp_out=./
#include "msg.pb.h"

int write() {
  Data::FinalMsg finalMsg;
  finalMsg.set_username("abcdefg");
  finalMsg.set_password("good");
  finalMsg.set_msgtype(Data::MsgType::Vec);
  finalMsg.set_allocated_vector(new Data::Vector);
  auto vec = finalMsg.mutable_vector();
  vec->set_vx(1);
  vec->set_vy(2);
  vec->set_vz(3);
  finalMsg.set_allocated_point(new Data::Point);
  auto pnt = finalMsg.mutable_point();
  pnt->set_px(4);
  pnt->set_py(5);
  pnt->set_pz(6);

  std::ofstream f("msg.txt");
  if (f.is_open()) {
    finalMsg.SerializeToOstream(&f);
    f.close();
  }

  return 0;
}

int read() {
  Data::FinalMsg finalMsg;
  std::ifstream f("msg.txt", std::ios::in);
  if (f.is_open()) {
    finalMsg.ParseFromIstream(&f);
    std::cout << finalMsg.username() << std::endl;
    std::cout << finalMsg.password() << std::endl;
    std::cout << finalMsg.vector().vx() << std::endl;
    std::cout << finalMsg.vector().vy() << std::endl;
    std::cout << finalMsg.vector().vz() << std::endl;
    std::cout << finalMsg.point().px() << std::endl;
    std::cout << finalMsg.point().py() << std::endl;
    std::cout << finalMsg.point().pz() << std::endl;
  }
  return 0;
}

int main(int argc, char** argv) {
  write();
  read();
  return 0;
}
