using System.Runtime.Serialization;
using System.Collections.Generic;

namespace translator.Models;
[DataContract]
public struct Example {
  [DataMember]
  public string example {get; set;} = string.Empty;
  [DataMember]
  public string description {get; set;} = string.Empty;

  public Example() {}
}

[DataContract]
public struct Option {
  [DataMember]
  public string word {get; set;} = string.Empty;
  [DataMember]
  public string gender {get; set;} = string.Empty;
  [DataMember]
  public List<string> translations {get; set;} = new();

  public Option() {}
}

[DataContract]
public struct TranslationResponse {
  // string source;
  [DataMember]
  public string sourceTranscription {get; set;} = string.Empty;
  [DataMember]
  public string sourceTranslit {get; set;} = string.Empty;
  [DataMember]
  Lang sourceLang {get; set;} = new();

  [DataMember]
  public string translation {get; set;} = string.Empty;
  [DataMember]
  public string translationTranscription {get; set;} = string.Empty;
  [DataMember]
  public string translationTranslit {get; set;} = string.Empty;
  [DataMember]
  public Lang translationLang {get; set;} = new();

  [DataMember]
  public Dictionary<string, List<Example>> examples {get; set;} = new();
  [DataMember]
  public Dictionary<string, List<Option>> options {get; set;} = new();

  [DataMember]
  public string error {get; set;} = string.Empty;

  public TranslationResponse() {}
}
