#pragma once
#include <mysql++.h>
#include <optional>

#include "../Example.h"
#include "../Option.h"
#include "Tables/Engine.h"
#include "Tables/Example/Example.h"
#include "Tables/Example/ExampleType.h"
#include "Tables/History.h"
#include "Tables/Language.h"
#include "Tables/Option/Option.h"
#include "Tables/Option/OptionTranslations.h"
#include "Tables/Option/OptionType.h"
#include "Tables/Translation.h"

class History {
 public:
  using engine_type      = Tables::Engine::value_type;
  using history_type     = Tables::History::value_type;
  using language_type    = Tables::Language::value_type;
  using translation_type = Tables::Translation::value_type;

  struct Translation {
    std::string source;
    // std::string m_sourceTranscription;
    // std::string m_sourceTranslit;
    int source_lang;

    std::string translation;
    // std::string m_translationTranscription;
    // std::string m_translationTranslit;
    int translation_lang;

    int ui_lang;
    int engine;

    std::map<std::string, std::vector<Option>> options;
    std::map<std::string, std::vector<Example>> examples;
  };

 public:
  // false = no throw errors
  History()
      : conn(false),
        engine(conn, engine_table),
        history(conn, history_table),
        language(conn, language_table),
        example(conn, example_table),
        example_type(conn, example_type_table),
        option(conn, option_table),
        option_type(conn, option_type_table),
        option_translations(conn, option_translations_table),
        translation(conn, translation_table) {}

  auto connected() const -> bool;
  auto connect(
      const char* database, const char* server, const char* user,
      const char* pass
  ) -> bool;
  auto setup(const std::vector<engine_type>&, const std::vector<language_type>&)
      -> bool;

  auto get_history() -> std::vector<std::pair<int, std::string>>;

  auto save(const Translation& translation) -> bool;
  auto search(int id) -> std::optional<Translation>;
  auto search(
      const std::string& source, int source_lang, int translation_lang,
      int ui_lang, int engine
  ) -> std::optional<Translation>;

  auto error() -> std::string;

 private:
  auto last_insert_id() -> int;
  auto search_for_options(int id) -> std::map<std::string, std::vector<Option>>;
  auto search_for_examples(int id)
      -> std::map<std::string, std::vector<Example>>;

 private:
  mysqlpp::Connection conn;

  // linked to setup function
  Tables::Engine engine;
  Tables::History history;
  Tables::Language language;
  Tables::Translation translation;
  Tables::Example example;
  Tables::ExampleType example_type;
  Tables::Option option;
  Tables::OptionType option_type;
  Tables::OptionTranslations option_translations;

 public:
  static constexpr char engine_table[]              = "engine";
  static constexpr char language_table[]            = "language";
  static constexpr char translation_table[]         = "translation";
  static constexpr char history_table[]             = "history";
  static constexpr char example_table[]             = "example";
  static constexpr char example_type_table[]        = "example_type";
  static constexpr char option_table[]              = "option";
  static constexpr char option_type_table[]         = "option_type";
  static constexpr char option_translations_table[] = "option_translations";
};
