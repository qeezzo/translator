#pragma once
#include <qonlinetranslator.h>

#include <boost/asio/query.hpp>
#include <cstddef>
#include <string>

// QUERY CLIENT -> SERVER

enum class QueryType : int {
  undefined       = -1,
  begin           = 0,
  TranslateQuery  = 1,
  DataConfigQuery = 2,
  end
};

struct Query {
  Query()         = default;
  using byte_iter = std::vector<std::byte>::iterator;
  virtual auto from_bytes(byte_iter begin, byte_iter end) -> bool = 0;
};

struct TranslateQuery : public Query {
  using Engine = QOnlineTranslator::Engine;
  using Lang   = QOnlineTranslator::Language;

  static constexpr int query_min_size =
      sizeof(Engine) + sizeof(Lang) * 3 + sizeof(size_t);

  Engine engine;
  Lang source;
  Lang dest;
  Lang ui;

  size_t size;
  std::string data;

  TranslateQuery() = default;

  auto from_bytes(byte_iter begin, byte_iter end) -> bool override;
};

struct DatabaseConfigQuery : public Query {
  std::string user;
  std::string server;
  std::string password;

  DatabaseConfigQuery() = default;

  auto from_bytes(byte_iter begin, byte_iter end) -> bool override;

 private:
  bool valid = true;

  auto get_string(byte_iter& begin, byte_iter end) -> std::string;
};
