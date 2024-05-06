#pragma once
#include <qonlinetranslator.h>

#include <QString>
#include <QThread>
#include <QtCore/QCoreApplication>
#include <boost/asio/query.hpp>

#include "Query.h"

class Translator {
 public:
  using Lang   = QOnlineTranslator::Language;
  using Engine = QOnlineTranslator::Engine;

  Translator(int argc, char *argv[]);

  auto translate(
      const std::string text, Engine engine = Engine::Google,
      Lang dest = Lang::Auto, Lang src = Lang::Auto, Lang ui_lang = Lang::Auto
  ) -> bool;

  auto translate(const TranslateQuery &query) -> bool {
    return translate(query.data, query.engine, query.dest, query.source, query.ui);
  }

  auto languages() const -> std::vector<std::pair<int, std::string>>;
  auto engines() const -> std::vector<std::pair<int, std::string>>;

  auto json() -> std::string&&;
  auto translation() -> std::string&&;

 private:
  std::string m_json;
  std::string m_translation;
  QCoreApplication app;
  QOnlineTranslator translator;

  std::string error;

  mutable std::vector<std::pair<int, std::string>> langs;
  mutable std::vector<std::pair<int, std::string>> engs;

  auto to_string(const QString& str) const -> std::string;
  void on_translation_finished();
};
