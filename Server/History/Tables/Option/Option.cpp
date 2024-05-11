#include "Option.h"

using namespace std;
using namespace Tables;

Option::Option(mysqlpp::Connection& conn, std::string name)
    : Table(conn, name) {}

auto Option::create(const string& translation) -> bool {
  auto query =
      conn.query()
      << "create table %0 ("
      << "option_id int primary key auto_increment,"
      << "option_type_id int not null,"
      << "word varchar(64) not null,"
      << "gender varchar(64) not null,"
      << "foreign key (option_type_id) references %1 (option_type_id))";
  query.parse();
  return query.execute(name, translation);
}

auto Option::insert(
    int option_type_id, const string& word, const string& gender
) -> bool {
  auto query = conn.query() << "insert into %0 (option_id, option_type_id, "
                               "word, gender) values (null, %1, %2q, %3q)";
  query.parse();
  return query.execute(name, option_type_id, word, gender);
}
