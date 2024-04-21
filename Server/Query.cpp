#include "Query.h"

Query::Query(byte_iter begin, byte_iter end) { from_bytes(begin, end); }

auto Query::from_bytes(byte_iter begin, byte_iter end) -> void {
  auto size = std::distance(begin, end);

  if (size < query_min_size) valid = false;

  auto it = begin;

  engine = *reinterpret_cast<Engine *>(&*it);
  it += sizeof(engine);

  source = *reinterpret_cast<Lang *>(&*it);
  it += sizeof(source);

  dest = *reinterpret_cast<Lang *>(&*it);
  it += sizeof(dest);

  ui = *reinterpret_cast<Lang *>(&*it);
  it += sizeof(ui);

  size = *reinterpret_cast<size_t *>(&*it);
  it += sizeof(size_t);

  data.assign(reinterpret_cast<char *>(&*it), reinterpret_cast<char *>(&*end));
}
