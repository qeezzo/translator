#include "History.h"

#include <algorithm>
#include <optional>
using namespace std;

auto History::connect(
    const char* database, const char* server, const char* user, const char* pass
) -> bool {
  auto res = conn.connect(database, server, user, pass);
  if (conn.errnum() == 1049 and conn.connect("", server, user, pass))
    return (conn.query() << "create database " << database).exec();
  return res;
}

// setup every table. create if not exist
auto History::setup(
    const vector<engine_type>& engines, const vector<language_type>& languages
) -> bool {
  if (not engine.exist() and not engine.create()) return false;
  if (engine.get() != engines) {
    int count{};
    ranges::for_each(engines, [&](auto value) {
      count += engine.insert(value);
    });
    if (count != engines.size()) return false;
  }

  if (not language.exist() and not language.create()) return false;
  if (language.get() != languages) {
    int count{};
    ranges::for_each(languages, [&](auto value) {
      count += language.insert(value);
    });
    if (count != engines.size()) return false;
  }

  if (not translation.exist() and
      not translation.create(engine.table(), language.table()))
    return false;

  if (not history.exist() and not history.create(translation.table()))
    return false;

  if (not example_type.exist() and not example_type.create(translation.table()))
    return false;
  if (not example.exist() and not example.create(example_type.table()))
    return false;

  if (not option_type.exist() and not option_type.create(translation.table()))
    return false;
  if (not option.exist() and not option.create(option_type.table()))
    return false;

  if (not option_translations.exist() and
      not option_translations.create(option.table()))
    return false;

  return true;
}

auto History::connected() const -> bool {
  return conn.connected();
}

auto History::get_history() -> vector<pair<int, string>> {
  if (not conn.connected()) return {};

  auto query = conn.query()
               << "select h.translation, t.source from %0 h "
                  "inner join %1 t on h.translation = t.translation_id";
  query.parse();
  auto table = query.store(history_table, translation_table);

  vector<pair<int, string>> res;
  for (auto row : table) res.push_back(pair<int, string>{row[0], row[1]});

  return res;
}

auto History::error() -> std::string {
  return conn.error();
}

auto History::save(const Translation& translation) -> bool {
  Tables::TranslationType values{
      .engine           = translation.engine,
      .ui_lang          = translation.ui_lang,
      .source_lang      = translation.source_lang,
      .translation_lang = translation.translation_lang,
      .source           = translation.source,
      .translation      = translation.translation};

  if (not this->translation.insert(values)) return false;

  auto translation_id = last_insert_id();
  if (translation_id < 0) return false;

  history.insert(translation_id);

  for (const auto& [type, example_vec] : translation.examples) {
    this->example_type.insert(translation_id, type);
    auto id = last_insert_id();
    if (id < 0) return false;
    for (const auto& example : example_vec)
      this->example.insert(id, example.example, example.description);
  }

  for (const auto& [type, option_vec] : translation.options) {
    this->option_type.insert(translation_id, type);
    auto option_id = last_insert_id();
    if (option_id < 0) return false;
    for (const auto& option : option_vec) {
      this->option.insert(option_id, option.word, option.gender);
      auto id = last_insert_id();
      if (id < 0) return false;
      for (const auto& translation : option.translations)
        this->option_translations.insert(id, translation);
    }
  }

  return true;
}

auto History::search(
    const std::string& source, int source_lang, int translation_lang,
    int ui_lang, int engine
) -> optional<Translation> {
  Translation translation;

  auto query = conn.query()
               << "select translation_id, translation, source_lang, "
                  "translation_lang from %0 where "
                  "engine = %1 and ui_lang = %2 "
               << (source_lang != 0 ? "and source_lang = %3 " : "")
               << (translation_lang != 0 ? "and translation_lang = %4 " : "")
               << " and source = %5q";
  query.parse();
  auto res = query.store(
      this->translation.table(),
      engine,
      ui_lang,
      source_lang,
      translation_lang,
      source
  );

  if (not res or res.empty()) return {};

  int id                       = res.front()[0];
  translation.translation      = (std::string)res.front()[1];
  translation.source_lang      = (int)res.front()[2];
  translation.translation_lang = (int)res.front()[3];
  translation.engine           = engine;
  translation.ui_lang          = ui_lang;

  translation.examples = search_for_examples(id);
  translation.options  = search_for_options(id);

  return translation;
}

auto History::search_for_options(int id)
    -> std::map<std::string, std::vector<Option>> {
  // clang-format off
  auto query = conn.query()
      << "select e.option_type_id, e.option_type from %0 e "
         "where e.translation_id = %1";
  query.parse();
  auto res = query.store(option_type.table(), id);
  if (not res or res.empty()) return {};

  std::map<std::string, std::vector<Option>> result;
  for (auto row : res) {
    auto query = conn.query()
        << "select o.option_id, o.word, o.gender from %0 o "
           "where o.option_type_id = %1";
    query.parse();

    int id = (int) row[0];
    auto res = query.store(option.table(), id);
    if (not res or res.empty()) continue;
    std::string name = (std::string)row[1];

    for (auto row : res) {
      auto query = conn.query()
          << "select ot.translation from %0 ot "
             "where ot.option_id = %1";
      query.parse();

      int id = (int) row[0];
      auto res = query.store(option_translations.table(), id);

      if (not res or res.empty()) continue;

      std::vector<std::string> translations;
      for (auto row : res)
        translations.push_back((std::string)row[0]);

      result[name].push_back({(std::string)row[1], (std::string)row[2], translations});
    }
  }

  return result;
  // clang-format on
}

auto History::search_for_examples(int id)
    -> std::map<std::string, std::vector<Example>> {
  // clang-format off
  auto query = conn.query()
      << "select e.example_type_id, e.example_type from %0 e "
         "where e.translation_id = %1";
  query.parse();

  auto res = query.store(example_type.table(), id);
  if (not res or res.empty()) return {};

  std::map<std::string, std::vector<Example>> result;
  for (auto row : res) {
    auto query = conn.query()
        << "select e.example, e.description from %0 e "
           "where e.example_type_id = %1";
    query.parse();

    int id = (int) row[0];
    auto res = query.store(example.table(), id);
    if (not res or res.empty()) continue;
    std::string example = (std::string)row[1];

    for (auto row : res)
      result[example].push_back({(std::string)row[0], (std::string)row[1]});
  }

  return result;
  // clang-format on
}

auto History::last_insert_id() -> int {
  auto query = conn.query() << "select last_insert_id()";
  auto res   = query.store();
  if (not res) return -1;
  return res.front().front();
}
