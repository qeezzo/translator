#include <qjsondocument.h>
#include <qobject.h>
#include <QString>
#include <QThread>

#include "Translator.h"

Translator::Translator(int argc, char *argv[]) : app(argc, argv) {
  QObject::connect(&translator, &QOnlineTranslator::finished,
                   [&] { on_translation_finished(); });
}

void Translator::translate(const std::string text, Engine engine, Lang dest,
                           Lang src, Lang ui_lang) {
  translator.translate(text.c_str(), engine, dest, src, ui_lang);
  app.exec();
}

void Translator::on_translation_finished() {
  if (translator.error() != QOnlineTranslator::NoError) {
    qCritical() << translator.errorString();
    return;
  }

  translation = translator.toJson().toJson(QJsonDocument::Compact);
  // qInfo() << "size" << translation.size() << ": " << translation;

  app.exit();  // make it synchronous operation
}

std::string Translator::json_str() const {
  auto utf8 = translation.toUtf8();
  std::string result((char *)utf8.data(), (char *)utf8.data() + utf8.size());
  return result;
}
