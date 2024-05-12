#pragma once

#include <boost/asio/registered_buffer.hpp>
#include <expected>
#include <unordered_map>

#include "Example.h"
#include "History.h"
#include "Option.h"
#include "ServerConnection.h"
#include "Translator.h"

class Server {
 public:
  Server(int argc, char* argv[], const unsigned short port = 30000);

  auto start() -> void;

 private:
  using error_code    = boost::system::error_code;
  using handle_result = std::expected<std::vector<std::byte>, std::string>;
  using Translation   = Translator::Translation;

  ServerConnection connection;
  Translator translator;
  std::vector<std::byte> buf;
  int bytes_read = 0;

  History history;

  std::unordered_map<QueryType, handle_result (Server::*)()>
      handles_for_query_types{
          {QueryType::TranslateQuery, &Server::handle_translate},
          {QueryType::DataConfigQuery, &Server::handle_database_config},
          {QueryType::HistoryQuery, &Server::handle_history},
      };

  auto recieve_query() -> bool;
  auto recieve_query_type() -> std::expected<QueryType, error_code>;
  auto bytes_to_query(std::vector<std::byte> bytes) -> Query;
  auto write_error_back(const std::string&) -> void;
  auto write(const_buffer, error_code&) -> void;

  auto new_connection() -> bool;
  auto new_history() -> bool;
  auto save_to_history(
      const TranslateQuery& source, const Translation& translation
  ) -> bool;
  auto get_from_history(const TranslateQuery& source)
      -> std::optional<Translation>;
  auto get_from_history(int id) -> std::optional<Translation>;

  auto from_standard(std::map<std::string, std::vector<Example>>& map)
      -> QMap<QString, QVector<QExample>>;
  auto from_standard(std::map<std::string, std::vector<Option>>& map)
      -> QMap<QString, QVector<QOption>>;
  auto to_standard(std::map<QString, QVector<QExample>>&)
      -> std::map<std::string, std::vector<Example>>;
  auto to_standard(std::map<QString, QVector<QOption>>&)
      -> std::map<std::string, std::vector<Option>>;

  auto handle_translate() -> handle_result;
  auto handle_database_config() -> handle_result;
  auto handle_history() -> handle_result;

  auto check_error(const error_code& err) -> bool;

  static constexpr char database[] = "translator";
};
