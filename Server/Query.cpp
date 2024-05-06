#include "Query.h"

#include <cstdint>

auto TranslateQuery::from_bytes(byte_iter begin, byte_iter end) -> bool {
  auto size = std::distance(begin, end);

  if (size < query_min_size) return false;

  auto it = begin;

  engine = *reinterpret_cast<Engine*>(&*it);
  it += sizeof(engine);

  source = *reinterpret_cast<Lang*>(&*it);
  it += sizeof(source);

  dest = *reinterpret_cast<Lang*>(&*it);
  it += sizeof(dest);

  ui = *reinterpret_cast<Lang*>(&*it);
  it += sizeof(ui);

  this->size = *reinterpret_cast<size_t*>(&*it);
  it += sizeof(size_t);

  data.assign(reinterpret_cast<char*>(&*it), reinterpret_cast<char*>(&*end));

  return true;
}


auto DatabaseConfigQuery::from_bytes(byte_iter begin, byte_iter end) -> bool {
  valid = true;
  auto it = begin;

  user     = get_string(it, end);
  server   = get_string(it, end);
  password = get_string(it, end);

  return valid;
}
auto DatabaseConfigQuery::get_string(byte_iter& begin, byte_iter end) -> std::string {
  if (std::distance(begin, end) < sizeof(int32_t)) {
    valid = false;
    return "";
  }

  int32_t user_size = *reinterpret_cast<int32_t*>(&*begin);

  begin += sizeof(user_size);
  if (begin == end or begin + user_size > end) {
    valid = false;
    return "";
  }

  std::string res(
      reinterpret_cast<char*>(&*begin),
      reinterpret_cast<char*>(&*begin) + user_size
  );

  begin += user_size;

  return res;
}
