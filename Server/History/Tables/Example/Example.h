#pragma once
#include <string>

#include "../Table.h"

namespace Tables {

class Example : public Table {
 public:
  Example(mysqlpp::Connection& conn, std::string name);

  auto insert(int example_type_id, const std::string& word, const std::string& desc)
      -> bool;
  auto create(const std::string&) -> bool;
};

}  // namespace Tables
