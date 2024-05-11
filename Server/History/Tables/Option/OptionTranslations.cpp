#include "OptionTranslations.h"

using namespace Tables;
using namespace std;

OptionTranslations::OptionTranslations(
    mysqlpp::Connection& conn, std::string name
)
    : Table(conn, name) {}

auto OptionTranslations::create(const string& option) -> bool {
  auto query = conn.query()
               << "create table %0 ("
               << "option_translation_id int primary key auto_increment,"
               << "option_id int not null,"
               << "translation varchar(128) not null,"
               << "foreign key (option_id) references %1 (option_id))";
  query.parse();
  return query.execute(name, option);
}

auto OptionTranslations::insert(int option_id, const string& translation) -> bool {
  auto query = conn.query()
               << "insert into %0 (option_translation_id, option_id, "
                  "translation) values (null, %1, %2q)";
  query.parse();
  return query.execute(name, option_id, translation);
}

// auto OptionTranslations::get() -> std::vector<value_type> {
//   auto query = conn.query() << "select * from " << name;
//   auto table = query.store();
//   std::vector<value_type> res;

//   for (auto row : table) res.push_back(value_type{row[0], row[1]});

//   return res;
// }
