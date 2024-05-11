#pragma once
#include <string>
#include <utility>

#include "Table.h"
#include "connection.h"

namespace Tables {

class Language : public Table {
 public:
  using value_type = std::pair<int, std::string>;

  Language(mysqlpp::Connection& conn, std::string name);

  auto get() -> std::vector<value_type>;

  auto insert(const value_type&) -> bool;
  auto create() -> bool;
};

}  // namespace Tables
