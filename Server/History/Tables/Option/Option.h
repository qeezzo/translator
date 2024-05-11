#pragma once
#include <string>

#include "../Table.h"

namespace Tables {

class Option : public Table {
 public:
  Option(mysqlpp::Connection& conn, std::string name);

  auto insert(int, const std::string&, const std::string&) -> bool;
  auto create(const std::string&) -> bool;
};

}  // namespace Tables
