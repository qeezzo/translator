#pragma once
#include <boost/asio/query.hpp>
#include <cstddef>
#include <qonlinetranslator.h>
#include <string>

// QUERY CLIENT -> SERVER

struct Query {
  using Engine = QOnlineTranslator::Engine;
  using Lang = QOnlineTranslator::Language;
  using byte_iter = std::vector<std::byte>::iterator;

  bool is_valid() const { return valid; }

  Engine engine;
  Lang source;
  Lang dest;
  Lang ui;

  size_t size;
  std::string data;

  static constexpr int query_min_size =
      sizeof(Engine) + sizeof(Lang) * 3 + sizeof(size_t);

  Query() = default;
  Query(byte_iter begin, byte_iter end);
  
  auto from_bytes(byte_iter begin, byte_iter end) -> void;

private:
  bool valid = true;
};
