#pragma once
#include <mysql++.h>

#include "Engine.h"
#include "History.h"
#include "Language.h"
#include "Translation.h"

class History {
 public:
  using engine_type      = Tables::Engine::value_type;
  using history_type     = Tables::History::value_type;
  using language_type    = Tables::Language::value_type;
  using translation_type = Tables::Translation::value_type;

 public:
  // false = no throw errors
  History()
      : conn(false),
        engine(conn, engine_table),
        history(conn, history_table),
        language(conn, language_table),
        translation(conn, translation_table) {}

  // should be in another class. but history is a single DB class
  auto connected() const -> bool;
  auto connect(
      const char* database, const char* server, const char* user,
      const char* pass
  ) -> bool;
  auto setup(const std::vector<engine_type>&, const std::vector<language_type>&)
      -> bool;

  auto get_history() -> std::vector<std::pair<int, std::string>>;

  auto save(
      const std::string& source, int source_lang,
      const std::string& translation, int translation_lang, int ui_lang,
      int engine
  ) -> bool;

  // auto search(
  //     const std::string& source, int source_lang,
  //     const std::string& translation, int translation_lang, int ui_lang,
  //     int engine
  // ) -> bool;

  auto error() -> std::string;

 private:
  mysqlpp::Connection conn;
  Tables::Engine engine;
  Tables::History history;
  Tables::Language language;
  Tables::Translation translation;

 public:
  static constexpr char engine_table[]      = "engine";
  static constexpr char language_table[]    = "language";
  static constexpr char translation_table[] = "translation";
  static constexpr char history_table[]     = "history";
};
