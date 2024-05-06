#include "Translator.h"

#include <qjsondocument.h>
#include <qobject.h>

#include <QString>
#include <QThread>

Translator::Translator(int argc, char *argv[]) : app(argc, argv) {
  QObject::connect(&translator, &QOnlineTranslator::finished, [&] {
    on_translation_finished();
  });
}

auto Translator::translate(
    const std::string text, Engine engine, Lang dest, Lang src, Lang ui_lang
) -> bool {
  error = "";
  translator.translate(text.c_str(), engine, dest, src, ui_lang);
  app.exec();
  return error.empty();
}

auto Translator::on_translation_finished() -> void {
  if (translator.error() != QOnlineTranslator::NoError) {
    error = to_string(translator.errorString());
    return;
  }

  m_translation = to_string(translator.translation());
  m_json = to_string(translator.toJson().toJson(QJsonDocument::Compact));

  app.exit();  // make it synchronous operation
}

auto Translator::json() -> std::string&& { return std::move(m_json); }
auto Translator::translation() -> std::string&& { return std::move(m_translation); }

auto Translator::languages() const -> std::vector<std::pair<int, std::string>> {
  if (not langs.empty()) return langs;
  for (int i = Lang::Auto; i < Lang::End; ++i)
    langs.push_back({i, to_string(QOnlineTranslator::languageName((Lang)i))});
  return langs;
}
auto Translator::engines() const -> std::vector<std::pair<int, std::string>> {
  if (not engs.empty()) return engs;
  engs = {
      {1, "Google"},
      {2, "Yandex"},
  };
  return engs;
}

auto Translator::to_string(const QString &str) const -> std::string {
  auto utf8 = str.toUtf8();
  std::string result((char *)utf8.data(), (char *)utf8.data() + utf8.size());
  return result;
}
