#include <boost/system/detail/error_code.hpp>

#include "ServerConnection.h"

ServerConnection::ServerConnection(unsigned short port)
    : address(ip::address_v4::loopback()),
      socket(service),
      server_ep(address, port),
      acceptor(service, server_ep) {
  std::clog << "Socket created" << std::endl;
}

auto ServerConnection::accept() -> bool {
  std::clog << "Accepting ... ";

  if (socket.is_open()) socket.close();

  boost::system::error_code err;
  if (acceptor.accept(socket, sender_ep, err)) {
    std::clog << " Erorr occured\n" << err;
    return false;
  }

  std::clog << sender_ep.address().to_string() << ":" << sender_ep.port()
            << " accepted\n";
  return true;
}

auto ServerConnection::read(std::vector<std::byte>& buf,
                            system::error_code& err) -> size_t {
  std::clog << "Reading ... ";
  size_t bytes = socket.receive(buffer(buf), 0, err);
  std::clog << bytes << " read\n";
  return bytes;
}

auto ServerConnection::is_open() -> bool { return socket.is_open(); }
