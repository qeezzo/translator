#pragma once
#include <string>

#include "Table.h"
#include "connection.h"

namespace Tables {

struct TranslationType {
  int id;
  int engine;
  int ui_lang;
  int source_lang;
  int translation_lang;
  std::string source;
  std::string translation;
};

class Translation : public Table<TranslationType> {
 public:
  using value_type = TranslationType;

  Translation(mysqlpp::Connection& conn, std::string name);

  auto create(const std::string& engine, const std::string& language) -> bool;

  auto insert(const value_type&) -> bool override;

  auto search(value_type&) -> bool;
};

}  // namespace Tables
