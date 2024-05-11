#include "Translator.h"

#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qobject.h>

#include <QString>
#include <QThread>

Translator::Translator(int argc, char *argv[]) : app(argc, argv) {
  QObject::connect(&translator, &QOnlineTranslator::finished,
                   [&] { on_translation_finished(); });
}

auto Translator::translate(const std::string text, Engine engine, Lang dest,
                           Lang src, Lang ui_lang) -> bool {
  m_error = "";
  translator.translate(text.c_str(), engine, dest, src, ui_lang);
  app.exec();
  return m_error.empty();
}

auto Translator::on_translation_finished() -> void {
  if (translator.error() != QOnlineTranslator::NoError) {
    m_error = translator.errorString().toStdString();
    return;
  }

  m_translation = Translation{
      .m_sourceTranscription = translator.sourceTranscription(),
      .m_sourceTranslit      = translator.sourceTranslit(),
      .m_sourceLang          = translator.sourceLanguage(),

      .m_translation              = translator.translation(),
      .m_translationTranscription = translator.translationTranslit(),
      .m_translationTranslit      = translator.translationTranslit(),
      .m_translationLang          = translator.translationLanguage(),

      .m_options  = translator.translationOptions(),
      .m_examples = translator.examples(),

      .m_error = translator.errorString(),
  };

  app.exit();  // make it synchronous operation
}

auto Translator::translation() const -> Translation { return m_translation; }
auto Translator::error() const -> std::string { return m_error; }

auto Translator::languages() const -> std::vector<std::pair<int, std::string>> {
  if (not langs.empty()) return langs;
  for (int i = Lang::Auto; i < Lang::End; ++i)
    langs.push_back(
        {i, QOnlineTranslator::languageName((Lang)i).toStdString()});
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

auto Translator::Translation::to_json() const -> std::string {
  QJsonObject translationOptions;
  for (auto it = m_options.cbegin(); it != m_options.cend(); ++it) {
    QJsonArray arr;
    for (const QOption &option : it.value()) arr.append(option.toJson());
    translationOptions.insert(it.key(), arr);
  }

  QJsonObject examples;
  for (auto it = m_examples.cbegin(); it != m_examples.cend(); ++it) {
    QJsonArray arr;
    for (const QExample &example : it.value()) arr.append(example.toJson());
    examples.insert(it.key(), arr);
  }

  QJsonObject object{
      {"sourceTranscription", m_sourceTranscription},
      {"ourceTranslit", m_sourceTranslit},
      {"sourceLanguage", m_sourceLang},
      {"translation", m_translation},
      {"translationTranscription", m_translationTranscription},
      {"translationTranslit", m_translationTranslit},
      {"translationLanguage", m_translationLang},
      {"options", qMove(translationOptions)},
      {"examples", qMove(examples)},
      {"error", m_error},
  };

  return QJsonDocument(object).toJson().toStdString();
}
