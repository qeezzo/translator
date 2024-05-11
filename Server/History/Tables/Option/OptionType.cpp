#include "OptionType.h"

using namespace Tables;
using namespace std;

OptionType::OptionType(mysqlpp::Connection& conn, std::string name)
    : Table(conn, name) {}

auto OptionType::create(const std::string& option_type) -> bool {
  auto query =
      conn.query()
      << "create table %0 ("
      << "option_type_id int primary key auto_increment,"
      << "translation_id int not null,"
      << "option_type varchar(64) not null,"
      << "foreign key (translation_id) references %1 (translation_id))";
  query.parse();
  return query.execute(name, option_type);
}

auto OptionType::insert(int translation_id, const string& option_type)
    -> bool {
  auto query = conn.query()
               << "insert into %0 (option_type_id, translation_id, "
                  "option_type) values (null, %1, %2q)";
  query.parse();
  return query.execute(name, translation_id, option_type);
}
