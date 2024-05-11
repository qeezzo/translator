#pragma once
#include <string>

#include "../Table.h"
#include "connection.h"

namespace Tables {

class OptionTranslations : public Table {
 public:
  OptionTranslations(mysqlpp::Connection& conn, std::string name);

  // auto get() -> std::vector<value_type>;

  auto insert(int option_id, const std::string& translation) -> bool;
  auto create(const std::string&) -> bool;
};

}  // namespace Tables
