#include "history.h"

#include <algorithm>
using namespace std;

auto History::connect(
    const char* database, const char* server, const char* user, const char* pass
) -> bool {
  auto res = conn.connect(database, server, user, pass);
  if (conn.errnum() == 1049 and conn.connect("", server, user, pass))
    return (conn.query() << "create database " << database).exec();
  return res;
}

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

  return true;
}

auto History::connected() const -> bool { return conn.connected(); }

auto History::get_history() -> vector<pair<int, string>> {
  if (not conn.connected()) return {};

  auto query = conn.query() << "select h.translation, t.source from %0 h "
                               "inner join %1 t on h.translation = t.id";
  query.parse();
  auto table = query.store(history_table, translation_table);

  vector<pair<int, string>> res;
  for (auto row : table) res.push_back(pair<int, string>{row[0], row[1]});

  return res;
}

auto History::error() -> std::string { return conn.error(); }

auto History::save(
    const std::string& source, int source_lang, const std::string& translation,
    int translation_lang, int ui_lang, int engine
) -> bool {

  Tables::TranslationType values {
    .engine = engine, .ui_lang = ui_lang, .source_lang = source_lang,
    .translation_lang = translation_lang, .source = source,
    .translation = translation
  };

  if (not this->translation.insert(values)) return false;
  if (not this->translation.search(values)) return false;

  history.insert(values.id);

  return true;
}

// auto History::search(
//     const std::string& source, int source_lang, const std::string& translation,
//     int translation_lang, int ui_lang, int engine
// ) -> std::string {

//   Tables::TranslationType values {
//     .engine = engine, .ui_lang = ui_lang, .source_lang = source_lang,
//     .translation_lang = translation_lang, .source = source,
//     .translation = translation
//   };

//   if (not this->translation.insert(values)) return false;
//   if (not this->translation.search(values)) return false;

//   history.insert(values.id);

//   return true;
// }
