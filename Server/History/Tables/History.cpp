#include "History.h"

#include <string>

using namespace Tables;
using namespace std;

History::History(mysqlpp::Connection& conn, std::string name)
    : Table(conn, name) {}

auto History::create(const string& translation) -> bool {
  auto query = conn.query() << "create table %0 ("
                            << "translation int not null, "
                            << "foreign key (translation) references %1 (translation_id))";
  query.parse();
  return query.execute(name, translation);
}

auto History::insert(const value_type& data) -> bool {
  auto query = conn.query() << "insert into %0 (translation) values (%1)";
  query.parse();
  return query.execute(name, data);
}

auto History::get() -> std::vector<value_type> {
  auto query = conn.query() << "select * from " << name;
  auto table = query.store();
  std::vector<value_type> res;

  for (auto row : table) res.push_back(row[0]);

  return res;
}
