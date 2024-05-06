#pragma once
#include <boost/asio.hpp>
#include <boost/system/detail/error_code.hpp>

using namespace boost::asio;
using namespace boost;

class ServerConnection {
 public:
  // default server ip 127.0.0.1:30000
  ServerConnection(const unsigned short port = 30000);

  auto is_open() -> bool;
  auto close() -> void;

  // synchronous
  auto accept() -> bool;
  auto read(std::vector<std::byte>& buf, system::error_code& err) -> size_t;
  auto write(
      const std::vector<boost::asio::const_buffer>& msg,
      boost::system::error_code& err
  ) -> void;
  auto write(boost::asio::const_buffer msg, boost::system::error_code& err)
      -> void;

 private:
  io_service service;
  ip::address address;
  ip::tcp::socket socket;
  ip::tcp::endpoint server_ep;
  ip::tcp::endpoint sender_ep;
  ip::tcp::acceptor acceptor;
};
