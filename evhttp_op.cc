/*
 * g++ evhttp_op.cc -std=c++11 -I/usr/local/libevent/include
 * -L/usr/local/libevent/lib -levent -lpthread
 * -I/home/syz/workshop/opensource/omega/include
 */

#include <arpa/inet.h>
#include <event.h>
#include <event2/http.h>
#include <evhttp.h>
#include <fcntl.h>
#include <linux/if_link.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <exception>
#include <string>
#include "ohm/json.h"

class TestJson : public ohm::JSONObject {
 public:
  using self = TestJson;
  JSONField(self, std::string, pipe_id, true);
};

int32_t httpserver_bindsocket(uint16_t port, int backlog) {
  int32_t err = -1;
  do {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
      printf("socket() failed\n");
      break;
    }

    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(int)) !=
        0) {
      printf("setsockopt() failed\n");
      break;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
      printf("bind() failed, check if port %d is occupided\n", port);
      break;
    }
    if (listen(fd, backlog) != 0) {
      printf("listen() failed\n");
      break;
    }

    int flags;
    if ((flags = fcntl(fd, F_GETFL, 0)) < 0 ||
        fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
      printf("fcntl() failed\n");
      break;
    }
    return fd;
  } while (false);
  return err;
}

void req_handle(struct evhttp_request* req, void* arg) {
  int err = -1;
  switch (evhttp_request_get_command(req)) {
    case EVHTTP_REQ_GET:
      break;

    case EVHTTP_REQ_POST: {
      const char* url = evhttp_request_get_uri(req);
      char path[1024];
      memset(path, 0, sizeof(path));
      sscanf(url, "%[^?]", path);

      if (strcmp("/v1/pipes/aip/unbind", path) == 0) {
        evbuffer* pevbuf = evhttp_request_get_input_buffer(req);
        std::string post_data = std::string((char*)EVBUFFER_DATA(pevbuf),
                                            evbuffer_get_length(pevbuf));

        try {
          auto tmpobj = ohm::json::from_string(post_data);
          TestJson testjson;
          testjson.parse(tmpobj);
          printf("%s\n", testjson.pipe_id.c_str());
        } catch (const std::exception& e) {
          struct evbuffer* evbuf = evbuffer_new();
          if (evbuf == nullptr) {
            break;
          }
          printf("================================\n");
          evbuffer_add_printf(evbuf, "%s", post_data.c_str());
          evhttp_send_reply(req, 200, "OK", evbuf);
          evbuffer_free(evbuf);

          break;
        }

        struct evbuffer* evbuf = evbuffer_new();
        if (evbuf == nullptr) {
          break;
        }
        evbuffer_add_printf(evbuf, "%s", post_data.c_str());
        evhttp_send_reply(req, 200, "OK", evbuf);
        evbuffer_free(evbuf);
      }
    } break;

    case EVHTTP_REQ_DELETE:
      break;
    default:
      break;
  }
}

int run(int port) {
  int err = -1;
  do {
    int fd = httpserver_bindsocket(port, 128);
    if (fd < 0) {
      break;
    }

    struct event_base* base;
    struct evhttp* httpd;

    base = event_base_new();
    if (!base) {
      break;
    }
    httpd = evhttp_new(base);
    if (!httpd) {
      break;
    }
    if (evhttp_accept_socket(httpd, fd) != 0) {
      break;
    }
    evhttp_set_gencb(httpd, req_handle, base);
    pthread_t pid;
    err = pthread_create(&pid, nullptr, (void* (*)(void*))event_base_dispatch,
                         base);
    if (err != 0) {
#if defined(RKNN)
      pthread_kill(pid, SIGALRM);
#else
      pthread_cancel(pid);
#endif
      evhttp_free(httpd);
      event_base_free(base);
    }
    pthread_join(pid, nullptr);
    err = 0;
  } while (false);
  return err;
}

int main(int argc, char** argv) {
  run(9999);
  return 0;
}
