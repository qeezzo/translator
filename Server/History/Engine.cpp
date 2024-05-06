#include "Engine.h"

using namespace Tables;

Engine::Engine(mysqlpp::Connection& conn, std::string name)
    : Table(conn, name) {}

auto Engine::create() -> bool {
  auto query = conn.query() << "create table " << name << "("
                            << "id int primary key,"
                            << "name varchar(32) not null)";
  return query.exec();
}

auto Engine::insert(const value_type& data) -> bool {
  auto query = conn.query() << "insert into %0 (id, name) values (%1, %2q)";
  query.parse();
  const auto& [id, engine] = data;
  return query.execute(name, id, engine);
}

auto Engine::get() -> std::vector<value_type> {
  auto query = conn.query() << "select * from " << name;
  auto table = query.store();
  std::vector<value_type> res;

  for (auto row : table)
    res.push_back(value_type{row[0], row[1]});


  return res;
}
