#include "ExampleType.h"

using namespace Tables;
using namespace std;

ExampleType::ExampleType(mysqlpp::Connection& conn, std::string name)
    : Table(conn, name) {}

auto ExampleType::create(const std::string& translation) -> bool {
  auto query =
      conn.query()
      << "create table %0 ("
      << "example_type_id int primary key auto_increment,"
      << "translation_id int not null,"
      << "example_type varchar(64) not null,"
      << "foreign key (translation_id) references %1 (translation_id))";
  query.parse();
  return query.execute(name, translation);
}

auto ExampleType::insert(int translation_id, const string& example_type)
    -> bool {
  auto query = conn.query()
               << "insert into %0 (example_type_id, translation_id, "
                  "example_type) values (null, %1, %2q)";
  query.parse();
  return query.execute(name, translation_id, example_type);
}
