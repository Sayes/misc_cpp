/*
 * Copyright (c) 2017-2018 S.Y.Z
 *
 * https://github.com/happyfish100/libfastcommon.git
 * https://github.com/happyfish100/fastdfs.git
 *
 * g++ fastdfs_op.cc -std=c++11 -I/usr/include/fastcommon/ -lfastcommon
 * -lfdfsclient -o release/fastdfs_op
 */

#include <fastdfs/fdfs_client.h>
#include <fastdfs/fdfs_global.h>
#include <fastdfs/storage_client1.h>
#include <fastdfs/tracker_types.h>
#include <iostream>

int main(int argc, char* argv[]) {
  int result = -1;

  do {
    char group_name[FDFS_GROUP_NAME_MAX_LEN + 1] = {0};

    char fdfsid[128] = {0};
    int err = 0;

    ConnectionInfo tracker_server_;
    ConnectionInfo* ptracker_server_ = &tracker_server_;
    ConnectionInfo storage_server_;
    int store_path_index_ = 0;

    log_init();
    g_log_context.log_level = LOG_ERR;
    ignore_signal_pipe();

    if (fdfs_client_init("/etc/fdfs/client.conf") != 0) break;

    if (tracker_get_connection_r(ptracker_server_, &err) == nullptr) break;

    if (tracker_query_storage_store(ptracker_server_, &storage_server_,
                                    group_name, &store_path_index_) != 0)
      break;

    tracker_connect_server(&storage_server_, &err);
    if (err != 0) break;

    if (storage_upload_by_filename1(ptracker_server_, &storage_server_,
                                    store_path_index_, "/etc/fdfs/client.conf",
                                    nullptr, nullptr, 0, nullptr, fdfsid) != 0)
      break;
    std::cout << fdfsid << std::endl;

    tracker_disconnect_server(&storage_server_);
    tracker_disconnect_server(ptracker_server_);

    result = 0;
  } while (0);

  fdfs_client_destroy();

  return result;
}
