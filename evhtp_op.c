/*******************************************
 *
 * Copyright (c) 2017-2018 SYZ
 *
 * https://github.com/criticalstack/libevhtp
 *
   gcc evhtp_op.c \
      -I/home/syz/workshop/opensource/libevhtp/include \
      -I/home/syz/workshop/opensource/libevhtp/build/include \
      -L/home/syz/workshop/opensource/libevhtp/build -levhtp \
      -L/usr/local/libevent/lib \
      -levent -pthread -lcrypto -levent_openssl \
      -levent_core -levent_extra -levent_core -lssl -o release/evhtp_op
 *
 *******************************************/

#include <errno.h>
#include <evhtp/evhtp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testcb(evhtp_request_t* req, void* a) {
  const char* str = a;

  evbuffer_add_printf(req->buffer_out, "%s %lu", str, strlen(str));
  evhtp_send_reply(req, EVHTP_RES_OK);
}

void issue161cb(evhtp_request_t* req, void* a) {
  struct evbuffer* b = evbuffer_new();

  if (evhtp_request_get_proto(req) == EVHTP_PROTO_10) {
    evhtp_request_set_keepalive(req, 0);
  }

  evhtp_send_reply_start(req, EVHTP_RES_OK);

  evbuffer_add(b, "foo", 3);
  evhtp_send_reply_body(req, b);

  evbuffer_add(b, "bar\n\n", 5);
  evhtp_send_reply_body(req, b);

  evhtp_send_reply_end(req);

  evhtp_safe_free(b, evbuffer_free);
}

int main(int argc, char** argv) {
  evbase_t* evbase = event_base_new();
  evhtp_t* htp = evhtp_new(evbase, NULL);

  evhtp_set_cb(htp, "/simple/", testcb, "simple");
  evhtp_set_cb(htp, "/1/ping", testcb, "one");
  evhtp_set_cb(htp, "/1/ping.json", testcb, "two");
  evhtp_set_cb(htp, "/issue161", issue161cb, NULL);
#ifndef EVHTP_DISABLE_EVTHR
  evhtp_use_threads_wexit(htp, NULL, NULL, 8, NULL);
#endif
  evhtp_bind_socket(htp, "0.0.0.0", 8081, 2048);

  event_base_loop(evbase, 0);

  evhtp_unbind_socket(htp);
  evhtp_safe_free(htp, evhtp_free);
  evhtp_safe_free(evbase, event_base_free);

  return 0;
}
