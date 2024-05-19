#pragma once
#include <qonlinetranslator.h>

#include <QString>
#include <QTimer>
#include <QThread>
#include <QtCore/QCoreApplication>
#include <boost/asio/query.hpp>

#include "Query.h"

class Translator {
 public:
  using Lang   = QOnlineTranslator::Language;
  using Engine = QOnlineTranslator::Engine;

  struct Translation {
    QString m_sourceTranscription;
    QString m_sourceTranslit;
    Lang m_sourceLang;

    QString m_translation;
    QString m_translationTranscription;
    QString m_translationTranslit;
    Lang m_translationLang;

    QMap<QString, QVector<QOption>> m_options;
    QMap<QString, QVector<QExample>> m_examples;

    QString m_error;

    auto to_json() const -> std::string;
  };

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

  auto translation() const -> Translation;

  auto error() const -> std::string;

 private:

  Translation m_translation;

  QCoreApplication app;
  QOnlineTranslator translator;
  QTimer timer;

  std::string m_error;

  mutable std::vector<std::pair<int, std::string>> langs;
  mutable std::vector<std::pair<int, std::string>> engs;

  void on_translation_finished();
  void on_timeout();
};
