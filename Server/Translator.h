#pragma once
#include <QString>
#include <QThread>
#include <QtCore/QCoreApplication>
#include <boost/asio/query.hpp>
#include <qonlinetranslator.h>

#include "Query.h"

class Translator {
public:
  using Lang = QOnlineTranslator::Language;
  using Engine = QOnlineTranslator::Engine;

  Translator(int argc, char *argv[]);

  void translate(const std::string text, Engine engine = Engine::Google,
                 Lang dest = Lang::Auto, Lang src = Lang::Auto,
                 Lang ui_lang = Lang::Auto);

  void translate(const Query &query) {
    translate(query.data, query.engine, query.dest, query.source, query.ui);
  }

  std::string json_str() const;

private:
  QString translation;
  QCoreApplication app;
  QOnlineTranslator translator;

  void on_translation_finished();
};
