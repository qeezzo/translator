using System;
using Newtonsoft.Json;
using translator.Models.Queries;
using System.Collections.Generic;

namespace translator.Models;


public class TranslatorModel {
  static public List<Language> Languages {get;} = new();
  static TranslatorModel() {
    foreach (Lang lang in Enum.GetValues<Lang>())
      Languages.Add(new Language(lang));
  }

  private Client Client = Client.GetClient();

  public TranslatorModel() {}

  public TranslationResponse Translate(string data, Engine engine, Lang source, Lang dest, Lang ui) {
    if (!Client.Connected && !Client.Connect())
      return new() { error = Client.Error};

    Query query = new TranslatorQuery {
      engine = engine,
      source = source,
      dest = dest,
      ui = ui,
      data = data.ToCharArray()
    };

    Client.Write(query);
    string response = Client.ReadString();
    if (string.IsNullOrEmpty(response))
      return new() { error = Client.Error };

    return ParseJsonResponse(response);
  }

  static public TranslationResponse ParseJsonResponse(string json) {
    return JsonConvert.DeserializeObject<TranslationResponse>(json);
  }
}
