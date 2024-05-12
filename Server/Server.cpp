#include "Server.h"

#include <boost/asio/error.hpp>
#include <expected>

using boost::asio::buffer;
using boost::asio::const_buffer;
using namespace std;

Server::Server(int argc, char* argv[], const unsigned short port)
    : connection(port),
      translator(argc, argv),  // argc, argv required by qt
      buf(1024 * 10) {}

auto Server::start() -> void {
  while (true) {
    boost::system::error_code err;
    if (not connection.is_open() and not new_connection()) return;

    auto type = recieve_query_type();
    if (not type) {
      check_error(type.error());
      continue;
    }

    if (type.value() == QueryType::undefined) {
      write_error_back("undefined type");
      continue;
    }

    auto handle = handles_for_query_types[type.value()];
    auto res    = (this->*handle)();

    if (not res)
      write_error_back(res.error());
    else
      write(buffer(res.value()), err);

    check_error(err);
  }
}

auto Server::write(const_buffer msg, error_code& err) -> void {
  const int32_t value = 0;
  std::vector<const_buffer> buffers;
  buffers.push_back(buffer(&value, sizeof(value)));
  buffers.push_back(msg);

  connection.write(buffers, err);
}

auto Server::write_error_back(const std::string& msg) -> void {
  std::clog << "[data log] " << msg << std::endl;
  if (not connection.is_open()) return;
  error_code err;
  int32_t size = msg.size();

  std::vector<const_buffer> buffers;
  buffers.push_back(buffer(&size, sizeof(size)));
  buffers.push_back(buffer(msg));

  connection.write(buffers, err);
  check_error(err);
}

// TODO: verification of type recieved
auto Server::recieve_query_type() -> std::expected<QueryType, error_code> {
  error_code err;
  bytes_read = connection.read(buf, err);

  if (err or buf.size() < sizeof(int32_t)) return std::unexpected(err);

  int value = *reinterpret_cast<int32_t*>(&*buf.begin());
  if ((int)QueryType::begin < value and value < (int)QueryType::end)
    return (QueryType)value;

  return QueryType::undefined;
}

auto Server::handle_translate() -> handle_result {
  std::clog << "Translate query" << std::endl;
  TranslateQuery query;
  auto begin = buf.begin();
  if (not query.from_bytes(begin + sizeof(int32_t), begin + bytes_read))
    return std::unexpected("wrong query data");

  Translation translation;
  if (auto opt = get_from_history(query); opt) {
    std::clog << "got from history" << std::endl;
    translation = opt.value();
  }

  else if (translator.translate(query)) {
    translation = translator.translation();
    if (translation.m_error.isEmpty()) save_to_history(query, translation);
  } else
    return std::unexpected(translator.error());

  auto json = translation.to_json();
  std::vector<std::byte> bytes(
      (std::byte*)json.data(), (std::byte*)json.data() + json.size()
  );

  return bytes;
}
auto Server::handle_database_config() -> handle_result {
  std::clog << "Database config query" << std::endl;
  DatabaseConfigQuery query;
  auto begin = buf.begin();
  if (not query.from_bytes(begin + sizeof(int32_t), begin + bytes_read))
    return std::unexpected("wrong query data");

  std::clog << query.user << " " << query.server << " " << query.password
            << std::endl;

  history.connect(
      database, query.server.c_str(), query.user.c_str(), query.password.c_str()
  );

  if (not history.connected()) return std::unexpected(history.error());
  if (not history.setup(translator.engines(), translator.languages()))
    return std::unexpected(history.error());

  // get history and parse it into byte vector
  auto full_history = history.get_history();
  if (full_history.empty()) return std::unexpected(history.error());

  std::vector<std::byte> bytes;
  int size = static_cast<int>(full_history.size());
  bytes.insert(
      bytes.end(), (std::byte*)&size, (std::byte*)&size + sizeof(size)
  );

  for (auto& [id, source] : full_history) {
    // id
    std::byte* id_bytes = reinterpret_cast<std::byte*>(&id);
    bytes.insert(bytes.end(), id_bytes, id_bytes + sizeof(id));

    // source size
    int size = static_cast<int>(source.size());
    bytes.insert(
        bytes.end(), (std::byte*)&size, (std::byte*)&size + sizeof(size)
    );

    // source data
    std::byte* source_bytes = reinterpret_cast<std::byte*>(source.data());
    bytes.insert(bytes.end(), source_bytes, source_bytes + source.size());
  }

  return bytes;
}
auto Server::handle_history() -> handle_result {
  std::clog << "History query" << std::endl;
  HistoryQuery query;
  auto begin = buf.begin();
  if (not query.from_bytes(begin + sizeof(int32_t), begin + bytes_read))
    return std::unexpected("wrong query data");

  Translation translation;
  if (auto opt = get_from_history(query.id); opt) {
    std::clog << "got from history" << std::endl;
    translation = opt.value();
  }

  auto json = translation.to_json();
  std::vector<std::byte> bytes(
      (std::byte*)json.data(), (std::byte*)json.data() + json.size()
  );

  return bytes;
}

auto Server::new_connection() -> bool {
  if (not connection.accept()) return false;
  return true;
}

auto Server::save_to_history(
    const TranslateQuery& query, const Translation& translation
) -> bool {
  if (not history.connected()) return false;
  std::clog << "saving to history ... ";
  auto examples     = translation.m_examples.toStdMap();
  auto options      = translation.m_options.toStdMap();
  auto std_examples = to_standard(examples);
  auto std_options  = to_standard(options);

  auto res = history.save(
      {query.data,
       query.source,
       translation.m_translation.toStdString(),
       translation.m_translationLang,
       query.ui,
       query.engine,
       std_options,
       std_examples}
  );
  std::clog << (res ? "" : "not ") << "saved" << std::endl;
  if (not res) std::clog << history.error() << std::endl;
  return res;
}

auto Server::get_from_history(int id)
    -> std::optional<Translation> {
  if (not history.connected()) return {};

  auto translation = history.search(id);

  if (not translation) return {};

  using Lang = Translator::Lang;
  Translation q{
      .m_sourceLang      = (Lang)translation->source_lang,
      .m_translation     = QString::fromStdString(translation->translation),
      .m_translationLang = (Lang)translation->translation_lang,
      .m_options         = from_standard(translation->options),
      .m_examples        = from_standard(translation->examples),
  };

  return q;
}
auto Server::get_from_history(const TranslateQuery& source)
    -> std::optional<Translation> {
  if (not history.connected()) return {};

  auto translation = history.search(
      source.data, source.source, source.dest, source.ui, source.engine
  );

  if (not translation) return {};

  using Lang = Translator::Lang;
  Translation q{
      .m_sourceLang      = (Lang)translation->source_lang,
      .m_translation     = QString::fromStdString(translation->translation),
      .m_translationLang = (Lang)translation->translation_lang,
      .m_options         = from_standard(translation->options),
      .m_examples        = from_standard(translation->examples),
  };

  return q;
}

auto Server::from_standard(std::map<std::string, std::vector<Example>>& map)
    -> QMap<QString, QVector<QExample>> {
  QMap<QString, QVector<QExample>> res;

  for (auto& [type, v] : map) {
    QVector<QExample> res_v;
    for (auto& e : v)
      res_v.push_back(
          {QString::fromStdString(e.example),
           QString::fromStdString(e.description)}
      );
    res[QString::fromStdString(type)] = std::move(res_v);
  }

  return res;
}

auto Server::from_standard(map<string, vector<Option>>& map)
    -> QMap<QString, QVector<QOption>> {
  QMap<QString, QVector<QOption>> res;

  for (auto& [type, v] : map) {
    QVector<QOption> res_v;
    for (auto& e : v) {
      QStringList translations;
      for (auto t : e.translations)
        translations.push_back(QString::fromStdString(t));
      res_v.push_back(
          {QString::fromStdString(e.word),
           QString::fromStdString(e.gender),
           std::move(translations)}
      );
    }
    res[QString::fromStdString(type)] = std::move(res_v);
  }

  return res;
}

auto Server::to_standard(std::map<QString, QVector<QExample>>& map)
    -> std::map<std::string, std::vector<Example>> {
  std::map<std::string, std::vector<Example>> res;

  for (auto& [type, v] : map) {
    std::vector<Example> res_v;
    for (auto& e : v)
      res_v.push_back({e.example.toStdString(), e.description.toStdString()});
    res[type.toStdString()] = std::move(res_v);
  }

  return res;
}

auto Server::to_standard(std::map<QString, QVector<QOption>>& map)
    -> std::map<std::string, std::vector<Option>> {
  std::map<std::string, std::vector<Option>> res;

  for (auto& [type, v] : map) {
    std::vector<Option> res_v;
    for (auto& e : v) {
      std::vector<std::string> translations;
      for (auto t : e.translations) translations.push_back(t.toStdString());
      // clang-format off
      res_v.push_back({
          e.word.toStdString(),
          e.gender.toStdString(),
          std::move(translations)
      });
      // clang-format on
    }
    res[type.toStdString()] = std::move(res_v);
  }

  return res;
}

auto Server::check_error(const error_code& err) -> bool {
  if (not err) return false;
  std::cerr << "[conn log] " << err.what() << "\n";

  if (err == boost::asio::error::eof) connection.close();

  return true;
}
