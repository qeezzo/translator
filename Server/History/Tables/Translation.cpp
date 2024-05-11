#include "Translation.h"

using namespace Tables;
using namespace std;

Translation::Translation(mysqlpp::Connection& conn, std::string name)
    : Table(conn, name) {}

auto Translation::create(const string& engine, const string& language) -> bool {
  auto query = conn.query()
               << "create table %0("
               << "translation_id int primary key auto_increment,"
               << "source varchar(1024) not null,"
               << "source_lang int not null,"
               << "translation varchar(2048) not null,"
               << "translation_lang int not null,"
               << "ui_lang int not null,"
               << "engine int not null,"
               << "foreign key (source_lang) references %1 (language_id),"
               << "foreign key (translation_lang) references %1 (language_id),"
               << "foreign key (ui_lang) references %1 (language_id),"
               << "foreign key (engine) references %2 (engine_id))";

  query.parse();
  return query.execute(name, language, engine);
}

auto Translation::insert(const value_type& data) -> bool {
  auto query = conn.query() << "insert into %0 (translation_id, source, source_lang, "
                               "translation, translation_lang, ui_lang, "
                               "engine) values (null, %1q, %2, %3q, %4, %5,%6)";
  query.parse();
  return query.execute(
      name, data.source, data.source_lang, data.translation,
      data.translation_lang, data.ui_lang, data.engine
  );
}

auto Translation::search(value_type& data) -> bool {
  auto query = conn.query() << "select id, translation from %0 where "
                            << "engine = %1 and "
                            << "ui_lang = %2 and "
                            << "source_lang = %3 and "
                            << "translation_lang = %4 and "
                            << "source = %5q ";

  query.parse();
  auto res = query.store(
      name, data.engine, data.ui_lang, data.source_lang, data.translation_lang,
      data.source
  );

  if (not res) return false;

  data.id          = res.front()[0];
  data.translation = static_cast<std::string>(res.front()[1]);

  return true;
}
