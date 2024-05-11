#pragma once
#include <string>

#include "../Table.h"

namespace Tables {

class ExampleType : public Table {
 public:
  ExampleType(mysqlpp::Connection& conn, std::string name);

  auto insert(int translation_id, const std::string& example_type) -> bool;
  auto create(const std::string&) -> bool;
};

}  // namespace Tables
