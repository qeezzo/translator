#pragma once
#include <string>

#include "../Table.h"

namespace Tables {

class OptionType : public Table {
 public:
  OptionType(mysqlpp::Connection& conn, std::string name);

  auto insert(int translation_id, const std::string& option_type) -> bool;
  auto create(const std::string&) -> bool;
};

}  // namespace Tables
