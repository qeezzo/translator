#pragma once
#include "connection.h"
#include "query.h"

namespace Tables {

template <typename T>
class Table {
 public:
  Table(mysqlpp::Connection& conn, std::string name) : conn(conn), name(name) {}

  virtual auto insert(const T&) -> bool = 0;
  auto clear() -> bool {
    return (conn.query() << "delete from " << name).exec();
  }
  auto exist() -> bool {
    auto query          = conn.query("show tables");
    auto tables         = query.store();
    const char* cname = name.c_str();

    for (auto table : tables)
      if (strcmp(table[0], cname) == 0) return true;

    return false;
  }
  auto table() -> const std::string& { return name; }

 protected:
  mysqlpp::Connection& conn;
  std::string name;
};

}  // namespace Tables
