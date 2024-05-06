#include "Server.h"

#include <boost/asio/error.hpp>
#include <cstdio>
#include <expected>

using boost::asio::buffer;
using boost::asio::const_buffer;

Server::Server(int argc, char* argv[], const unsigned short port)
    : connection(port),
      translator(argc, argv),  // argc, argv required by qt
      buf(1024 * 10) {}

auto Server::start() -> void {
  while (true) {
    boost::system::error_code err;
    if (not connection.is_open() and not new_connection()) return;

    auto type = recieve_query_type();
    if (not type) {
      check_error(type.error());
      continue;
    }

    if (type.value() == QueryType::undefined) {
      write_error_back("undefined type");
      continue;
    }

    auto handle = handles_for_query_types[type.value()];
    auto res    = (this->*handle)();

    if (not res)
      write_error_back(res.error());
    else
      write(buffer(res.value()), err);

    check_error(err);
  }
}

auto Server::write(const_buffer msg, error_code& err) -> void {
  const int32_t value = 0;
  std::vector<const_buffer> buffers;
  buffers.push_back(buffer(&value, sizeof(value)));
  buffers.push_back(msg);

  connection.write(buffers, err);
}

auto Server::write_error_back(const std::string& msg) -> void {
  std::clog << "[data log] " << msg << std::endl;
  if (not connection.is_open()) return;
  error_code err;
  int32_t size = msg.size();

  std::vector<const_buffer> buffers;
  buffers.push_back(buffer(&size, sizeof(size)));
  buffers.push_back(buffer(msg));

  connection.write(buffers, err);
  check_error(err);
}

// TODO: verification of type recieved
auto Server::recieve_query_type() -> std::expected<QueryType, error_code> {
  error_code err;
  bytes_read = connection.read(buf, err);

  if (err or buf.size() < sizeof(int32_t)) return std::unexpected(err);

  int value = *reinterpret_cast<int32_t*>(&*buf.begin());
  if ((int)QueryType::begin < value and value < (int)QueryType::end)
    return (QueryType)value;

  return QueryType::undefined;
}

auto Server::handle_translate() -> handle_result {
  std::clog << "Translate query" << std::endl;
  TranslateQuery query;
  auto begin = buf.begin();
  if (not query.from_bytes(begin + sizeof(int32_t), begin + bytes_read))
    return std::unexpected("wrong query data");

  // if (history.connected()) {
  //   std::clog << "searching to history ... ";
  //   auto res = history.search(
  //       query.data, query.source, translator.translation(), query.dest,
  //       query.ui, query.engine
  //     );
  //   std::clog << (res ? "" : "not ") << "found" << std::endl;
  //   if (not res)
  //     std::clog << history.error() << std::endl;
  // }

  translator.translate(query);
  std::string json = translator.json();

  if (history.connected()) {
    std::clog << "saving to history ... ";
    auto res = history.save(
        query.data, query.source, translator.translation(), query.dest,
        query.ui, query.engine
        );
    std::clog << (res ? "" : "not ") << "saved" << std::endl;
    if (not res)
      std::clog << history.error() << std::endl;
  }

  std::vector<std::byte> bytes(
      (std::byte*)json.data(), (std::byte*)json.data() + json.size()
  );

  return bytes;
}
auto Server::handle_database_config() -> handle_result {
  std::clog << "Database config query" << std::endl;
  DatabaseConfigQuery query;
  auto begin = buf.begin();
  if (not query.from_bytes(begin + sizeof(int32_t), begin + bytes_read))
    return std::unexpected("wrong query data");

  std::clog << query.user << " " << query.server << " " << query.password
            << std::endl;

  history.connect(
      database, query.server.c_str(), query.user.c_str(), query.password.c_str()
  );

  if (not history.connected()) return std::unexpected(history.error());

  if (not history.setup(translator.engines(), translator.languages()))
    return std::unexpected(history.error());

  // get history and parse it into byte vector
  auto full_history = history.get_history();
  if (full_history.empty())
    return std::unexpected(history.error());

  std::vector<std::byte> bytes;
  int size = static_cast<int>(full_history.size());
  bytes.insert(
      bytes.end(), (std::byte*)&size, (std::byte*)&size + sizeof(size)
  );

  for (auto& [id, source] : full_history) {
    // id
    std::byte* id_bytes = reinterpret_cast<std::byte*>(&id);
    bytes.insert(bytes.end(), id_bytes, id_bytes + sizeof(id));

    // source size
    int size = static_cast<int>(source.size());
    bytes.insert(
        bytes.end(), (std::byte*)&size, (std::byte*)&size + sizeof(size)
    );

    // source data
    std::byte* source_bytes = reinterpret_cast<std::byte*>(source.data());
    bytes.insert(bytes.end(), source_bytes, source_bytes + source.size());
  }

  return bytes;
}

auto Server::new_connection() -> bool {
  if (not connection.accept()) return false;
  return true;
}

auto Server::check_error(const error_code& err) -> bool {
  if (not err) return false;
  std::cerr << "[conn log] " << err.what() << "\n";

  if (err == boost::asio::error::eof) connection.close();

  return true;
}
