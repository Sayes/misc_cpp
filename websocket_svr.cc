/*
 * Copyright (c) 2017-2018 SYZ
 *
 * https://github.com/zaphoyd/websocketpp.git
 * v0.7.0
 *
 * g++ websocket_svr.cc -std=c++11 -D_WEBSOCKETPP_CPP11_STL_ -DASIO_STANDALONE
 * -I$WEBSOCKETPP070_HOME/include -lssl -lcrypto -o release/websocket_svr
 *
 */

#include <iostream>
#include <websocketpp/config/asio.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

template <class T>
void processMessage(
    T* server, const websocketpp::connection_hdl& hdl,
    const websocketpp::server<websocketpp::config::asio>::message_ptr& msg);

template <class T>
void OnOpen(T* server, websocketpp::connection_hdl hdl) {
  std::cout << "OnOpen() called" << std::endl;
}

template <class T>
void OnClose(T* server, websocketpp::connection_hdl hdl) {
  std::cout << "OnClose() called" << std::endl;
}

template <class T>
void OnMessage(
    T* server, websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg) {
  processMessage<T>(server, hdl, msg);
}

template <class T>
void OnFail(T* server, websocketpp::connection_hdl hdl,
            websocketpp::server<websocketpp::config::asio>::message_ptr msg) {
  std::cout << "OnFail() called" << std::endl;
}

template <class T>
void on_http(T* s, websocketpp::connection_hdl hdl) {
  websocketpp::server<websocketpp::config::asio_tls>::connection_ptr con =
      s->get_con_from_hdl(hdl);
  con->set_body("Hello World!");
  con->set_status(websocketpp::http::status_code::ok);
}

std::string get_password() { return "test"; }

enum tls_mode { MOZILLA_INTERMEDIATE = 1, MOZILLA_MODERN = 2 };

websocketpp::lib::shared_ptr<asio::ssl::context> on_tls_init(
    tls_mode mode, websocketpp::connection_hdl hdl) {
  std::cout << "on_tls_init called with hdl: " << hdl.lock().get() << std::endl;
  std::cout << "using TLS mode: "
            << (mode == MOZILLA_MODERN ? "Mozilla Modern"
                                       : "Mozilla Intermediate")
            << std::endl;
  websocketpp::lib::shared_ptr<asio::ssl::context> ctx =
      websocketpp::lib::make_shared<asio::ssl::context>(
          asio::ssl::context::sslv23);

  try {
    if (mode == MOZILLA_MODERN) {
      // Modern disables TLSv1
      ctx->set_options(
          asio::ssl::context::default_workarounds |
          asio::ssl::context::no_sslv2 | asio::ssl::context::no_sslv3 |
          asio::ssl::context::no_tlsv1 | asio::ssl::context::single_dh_use);
    } else {
      ctx->set_options(asio::ssl::context::default_workarounds |
                       asio::ssl::context::no_sslv2 |
                       asio::ssl::context::no_sslv3 |
                       asio::ssl::context::single_dh_use);
    }
    ctx->set_password_callback(bind(&get_password));
    ctx->use_certificate_chain_file("server.pem");
    ctx->use_private_key_file("server.pem", asio::ssl::context::pem);

    // Example method of generating this file:
    // `openssl dhparam -out dh.pem 2048`
    // Mozilla Intermediate suggests 1024 as the minimum size to use
    // Mozilla Modern suggests 2048 as the minimum size to use.
    ctx->use_tmp_dh_file("dh.pem");

    std::string ciphers;

    if (mode == MOZILLA_MODERN) {
      ciphers =
          "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-"
          "AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-"
          "SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-"
          "SHA256:"
          "ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-"
          "SHA:"
          "ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-"
          "SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:"
          "DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-"
          "RSA-AES256-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK";
    } else {
      ciphers =
          "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-"
          "AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-"
          "SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-"
          "SHA256:"
          "ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-"
          "SHA:"
          "ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-"
          "SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:"
          "DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-"
          "RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA256:"
          "AES256-SHA256:AES128-SHA:AES256-SHA:AES:CAMELLIA:DES-CBC3-SHA:!"
          "aNULL:!"
          "eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!aECDH:!EDH-DSS-DES-CBC3-SHA:!EDH-"
          "RSA-DES-CBC3-SHA:!KRB5-DES-CBC3-SHA";
    }

    if (SSL_CTX_set_cipher_list(ctx->native_handle(), ciphers.c_str()) != 1) {
      std::cout << "Error setting cipher list" << std::endl;
    }
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
  }
  return ctx;
}

int main(int argc, char* argv[]) {
  int result = -1;
  do {
    asio::io_service ios;

    // ws
    websocketpp::server<websocketpp::config::asio> server;
    server.set_access_channels(websocketpp::log::alevel::all);
    server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    server.init_asio(&ios);
    server.set_open_handler(
        bind(&OnOpen<websocketpp::server<websocketpp::config::asio>>, &server,
             websocketpp::lib::placeholders::_1));
    server.set_close_handler(
        bind(&OnClose<websocketpp::server<websocketpp::config::asio>>, &server,
             websocketpp::lib::placeholders::_1));
    server.set_message_handler(
        bind(&OnMessage<websocketpp::server<websocketpp::config::asio>>,
             &server, websocketpp::lib::placeholders::_1,
             websocketpp::lib::placeholders::_2));
    server.listen(8090);
    server.start_accept();

    // wss
    websocketpp::server<websocketpp::config::asio_tls> server_tls;
    server_tls.init_asio(&ios);
    server_tls.set_message_handler(
        bind(&OnMessage<websocketpp::server<websocketpp::config::asio_tls>>,
             &server_tls, websocketpp::lib::placeholders::_1,
             websocketpp::lib::placeholders::_2));
    server_tls.set_http_handler(
        bind(&on_http<websocketpp::server<websocketpp::config::asio_tls>>,
             &server_tls, websocketpp::lib::placeholders::_1));
    server_tls.set_tls_init_handler(bind(&on_tls_init, MOZILLA_INTERMEDIATE,
                                         websocketpp::lib::placeholders::_1));
    server_tls.listen(8091);
    server_tls.start_accept();

    ios.run();

    result = 0;
  } while (0);
  return result;
}

template <class T>
void processMessage(
    T* server, const websocketpp::connection_hdl& hdl,
    const websocketpp::server<websocketpp::config::asio>::message_ptr& msg) {
  std::string str_query = msg->get_payload();
  try {
    server->send(hdl, str_query, websocketpp::frame::opcode::text);
  } catch (const websocketpp::lib::error_code& e) {
    std::cout << "WebSocketpp server send() failed" << std::endl;
  }
}
