#include <boost/asio/error.hpp>

#include "Server.h"

Server::Server(int argc, char* argv[], const unsigned short port)
    : connection(port),
      translator(argc, argv),  // argc, argv required by qt
      buffer(1024*10)
{}

auto Server::start() -> void {
  if (not new_connection()) return;
  while (connection_valid) {

    // check if connection has closed
    if (not recieve_query()) continue;

    translator.translate(query);
    std::string json = translator.json_str();

    // check if connection has closed as well
    if (not send_string(json)) continue;
  }
}

auto Server::new_connection() -> bool {
  return connection.accept() ? true : connection_valid = false;
}

auto Server::recieve_query() -> bool {
  boost::system::error_code err;
  auto bytes_read = connection.read(buffer, err);

  if (!err) {
    query.from_bytes(buffer.begin(), buffer.begin() + bytes_read);
    return true;
  }

  if (err == error::eof)
    new_connection();

  return false;
}

auto Server::send_string(const std::string& str) -> bool {
  boost::system::error_code err;
  connection.write(str, err);

  if (not err) return true;

  if (err == error::connection_aborted) new_connection();
  return false;
}
