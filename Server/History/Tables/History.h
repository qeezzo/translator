#pragma once
#include <string>

#include "Table.h"

namespace Tables {

class History : public Table {
 public:
  using value_type = int;

  History(mysqlpp::Connection& conn, std::string name);

  auto get() -> std::vector<value_type>;

  auto insert(const value_type&) -> bool;
  auto create(const std::string& translation) -> bool;
};

}  // namespace Tables
