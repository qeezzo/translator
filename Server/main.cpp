#include <boost/asio.hpp>
#include <boost/asio/query.hpp>
#include <iostream>

#include "Server.h"

using namespace boost::asio;

//          scenario
// ** connected **
// client -> database_config -> server
// server -> history -> client
// while (connection is valid)
//    client -> query -> server

int main(int argc, char* argv[]) {

  try {

    std::clog << "Server is ON" << std::endl;

    // Synchronous
    Server server(argc, argv, 30000);
    server.start();

  } catch (boost::system::system_error e) {
    std::cerr << e.what() << std::endl;
  }

  std::clog << "Server is OFF" << std::endl;
}
