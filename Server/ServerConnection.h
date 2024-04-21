#pragma once
#include <boost/asio.hpp>
#include <boost/system/detail/error_code.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost;

class ServerConnection {
 public:
  // default server ip 127.0.0.1:30000
  ServerConnection(const unsigned short port = 30000);

  auto is_open() -> bool;

  // synchronous
  auto accept() -> bool;
  auto read(std::vector<std::byte>& buf, system::error_code& err) -> size_t;

  template <typename T>
  auto write(T msg, boost::system::error_code& err) -> void {
    std::clog << "Writing ... ";
    int bytes = socket.write_some(buffer(msg), err);
    std::clog << bytes << " written\n";
  }

 private:
  io_service service;
  ip::address address;
  ip::tcp::socket socket;
  ip::tcp::endpoint server_ep;
  ip::tcp::endpoint sender_ep;
  ip::tcp::acceptor acceptor;
};
