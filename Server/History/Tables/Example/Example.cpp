#include "Example.h"

using namespace Tables;
using namespace std;

Example::Example(mysqlpp::Connection& conn, std::string name)
    : Table(conn, name) {}

auto Example::create(const std::string& example_type) -> bool {
  auto query =
      conn.query()
      << "create table %0 ("
      << "example_id int primary key auto_increment,"
      << "example_type_id int not null,"
      << "example varchar(64) not null,"
      << "description varchar(512) not null,"
      << "foreign key (example_type_id) references %1 (example_type_id))";
  query.parse();
  return query.execute(name, example_type);
}

auto Example::insert(
    int example_type_id, const string& word, const string& desc
) -> bool {
  auto query = conn.query()
               << "insert into %0 (example_id, example_type_id, example, "
                  "description) values (null, %1, %2q, %3q)";
  query.parse();
  return query.execute(name, example_type_id, word, desc);
}
