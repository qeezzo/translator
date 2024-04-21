#pragma once

#include "ServerConnection.h"
#include "Translator.h"

class Server {
 public:
  Server(int argc, char* argv[], const unsigned short port = 30000);

  auto start() -> void;

 private:
  ServerConnection connection;
  Translator translator;
  bool connection_valid = true;
  std::vector<std::byte> buffer;
  Query query;

  auto recieve_query() -> bool;
  auto send_string(const std::string& str) -> bool;
  auto bytes_to_query(std::vector<std::byte> bytes) -> Query;

  auto new_connection() -> bool;
};
