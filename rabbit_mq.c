#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <amqp.h>
#include <amqp_tcp_socket.h>

int main(int argc, const char* argv[]) {
  const char* hostname;
  int port;
  int status;
  const char* exchange;
  const char* bindingkey;
  const char* queue;
  amqp_socket_t* socket = NULL;
  amqp_connection_state_t conn;

  if (argc < 6) {
    fprintf(stderr, "Usage: amqp_bind host port exchange bindingkey queue\n");
    return 1;
  }

  hostname = argv[1];
  port = atoi(argv[2]);
  exchange = argv[3];
  bindingkey = argv[4];
  queue = argv[5];

  conn = amqp_new_connection();
  socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    // die("createing TCP socket");
  }

  status = amqp_socket_open(socket, hostname, port);
  if (status) {
    // die("opening TCP socket");
  }
  /*
    die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0,
    AMQP_SASL_METHOD_PLAIN, "test_boy",
                                */
}
