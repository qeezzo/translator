#pragma once
#include <string>
#include <utility>

#include "Table.h"
#include "connection.h"

namespace Tables {

class Engine : public Table<std::pair<int, std::string>> {
 public:
  using value_type = std::pair<int, std::string>;

  Engine(mysqlpp::Connection& conn, std::string name);

  auto get() -> std::vector<value_type>;

  auto insert(const value_type&) -> bool override;
  auto create() -> bool;
};

}  // namespace Tables
