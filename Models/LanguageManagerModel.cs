using System.Collections.Generic;
using System.Runtime.Serialization;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System;

namespace translator.Models;

[DataContract]
public class Language : INotifyPropertyChanged, IComparable<Language> {

  [DataMember]
  public Lang Lang {get; set;}
  public string Name {get => ToString();} 
  public Language(Lang lang) { Lang = lang; }
  public override string ToString() {
    return LanguageManager.LanguageName(Lang);
  }

  public event PropertyChangedEventHandler? PropertyChanged;
  public void RaisePropertyChanged([CallerMemberName] string? propertyName = null) {
    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
  }

  public int CompareTo(Language? lang) {
    System.Console.WriteLine("HRHEHREHRHE");
    return lang?.Lang.CompareTo(lang) ?? -1;
  }
}

public enum Lang {
    NoLanguage = -1,
    Auto,
    Afrikaans,
    Albanian,
    Amharic,
    Arabic,
    Armenian,
    Azerbaijani,
    Bashkir,
    Basque,
    Belarusian,
    Bengali,
    Bosnian,
    Bulgarian,
    Cantonese,
    Catalan,
    Cebuano,
    Chichewa,
    Corsican,
    Croatian,
    Czech,
    Danish,
    Dutch,
    English,
    Esperanto,
    Estonian,
    Fijian,
    Filipino,
    Finnish,
    French,
    Frisian,
    Galician,
    Georgian,
    German,
    Greek,
    Gujarati,
    HaitianCreole,
    Hausa,
    Hawaiian,
    Hebrew,
    HillMari,
    Hindi,
    Hmong,
    Hungarian,
    Icelandic,
    Igbo,
    Indonesian,
    Irish,
    Italian,
    Japanese,
    Javanese,
    Kannada,
    Kazakh,
    Khmer,
    Kinyarwanda,
    Klingon,
    KlingonPlqaD,
    Korean,
    Kurdish,
    Kyrgyz,
    Lao,
    Latin,
    Latvian,
    LevantineArabic,
    Lithuanian,
    Luxembourgish,
    Macedonian,
    Malagasy,
    Malay,
    Malayalam,
    Maltese,
    Maori,
    Marathi,
    Mari,
    Mongolian,
    Myanmar,
    Nepali,
    Norwegian,
    Oriya,
    Papiamento,
    Pashto,
    Persian,
    Polish,
    Portuguese,
    Punjabi,
    QueretaroOtomi,
    Romanian,
    Russian,
    Samoan,
    ScotsGaelic,
    SerbianCyrillic,
    SerbianLatin,
    Sesotho,
    Shona,
    SimplifiedChinese,
    Sindhi,
    Sinhala,
    Slovak,
    Slovenian,
    Somali,
    Spanish,
    Sundanese,
    Swahili,
    Swedish,
    Tagalog,
    Tahitian,
    Tajik,
    Tamil,
    Tatar,
    Telugu,
    Thai,
    Tongan,
    TraditionalChinese,
    Turkish,
    Turkmen,
    Udmurt,
    Uighur,
    Ukrainian,
    Urdu,
    Uzbek,
    Vietnamese,
    Welsh,
    Xhosa,
    Yiddish,
    Yoruba,
    YucatecMaya,
    Zulu
};
public enum Engine {
    Google = 1,
    Yandex = 2,
    Bing,
    LibreTranslate,
    Lingva
};

public static class LanguageManager {
  static public List<Language> Languages {get;} = new();
  static LanguageManager() {
    //Languages = new (Enum.GetValues<Lang>().ToList<Language>());
    foreach (Lang lang in Enum.GetValues<Lang>())
      Languages.Add(new Language(lang));
  }

  static public string LanguageName(Lang lang) {
    switch (lang) {
      case Lang.Auto:              return "Auto";
      case Lang.Afrikaans:         return "Afrikaans";
      case Lang.Albanian:          return "Albanian";
      case Lang.Amharic:           return "Amharic";
      case Lang.Arabic:            return "Arabic";
      case Lang.Armenian:          return "Armenian";
      case Lang.Azerbaijani:       return "Azeerbaijani";
      case Lang.Basque:            return "Basque";
      case Lang.Bashkir:           return "Bashkir";
      case Lang.Belarusian:        return "Belarusian";
      case Lang.Bengali:           return "Bengali";
      case Lang.Bosnian:           return "Bosnian";
      case Lang.Bulgarian:         return "Bulgarian";
      case Lang.Catalan:           return "Catalan";
      case Lang.Cantonese:         return "Cantonese";
      case Lang.Cebuano:           return "Cebuano";
      case Lang.SimplifiedChinese: return "Chinese (Simplified)";
      case Lang.TraditionalChinese:return "Chinese (Traditional)";
      case Lang.Corsican:          return "Corsican";
      case Lang.Croatian:          return "Croatian";
      case Lang.Czech:             return "Czech";
      case Lang.Danish:            return "Danish";
      case Lang.Dutch:             return "Dutch";
      case Lang.English:           return "English";
      case Lang.Esperanto:         return "Esperanto";
      case Lang.Estonian:          return "Estonian";
      case Lang.Fijian:            return "Fijian";
      case Lang.Filipino:          return "Filipino";
      case Lang.Finnish:           return "Finnish";
      case Lang.French:            return "French";
      case Lang.Frisian:           return "Frisian";
      case Lang.Galician:          return "Galician";
      case Lang.Georgian:          return "Georgian";
      case Lang.German:            return "German";
      case Lang.Greek:             return "Greek";
      case Lang.Gujarati:          return "Gujarati";
      case Lang.HaitianCreole:     return "Haitian Creole";
      case Lang.Hausa:             return "Hausa";
      case Lang.Hawaiian:          return "Hawaiian";
      case Lang.Hebrew:            return "Hebrew";
      case Lang.HillMari:          return "Hill Mari";
      case Lang.Hindi:             return "Hindi";
      case Lang.Hmong:             return "Hmong";
      case Lang.Hungarian:         return "Hungarian";
      case Lang.Icelandic:         return "Icelandic";
      case Lang.Igbo:              return "Igbo";
      case Lang.Indonesian:        return "Indonesian";
      case Lang.Irish:						 return "Irish";
      case Lang.Italian:					 return "Italian";
      case Lang.Japanese:					 return "Japanese";
      case Lang.Javanese:					 return "Javanese";
      case Lang.Kannada:					 return "Kannada";
      case Lang.Kazakh:						 return "Kazakh";
      case Lang.Khmer:						 return "Khmer";
      case Lang.Kinyarwanda:			 return "Kinyarwanda";
      case Lang.Klingon:					 return "Klingon";
      case Lang.KlingonPlqaD:			 return "Klingon (PlqaD)";
      case Lang.Korean:						 return "Korean";
      case Lang.Kurdish:					 return "Kurdish";
      case Lang.Kyrgyz:						 return "Kyrgyz";
      case Lang.Lao:								return "Lao";
      case Lang.Latin:							return "Latin";
      case Lang.Latvian:						return "Latvian";
      case Lang.LevantineArabic:		return "Levantine Arabic";
      case Lang.Lithuanian:				return "Lithuanian";
      case Lang.Luxembourgish:			return "Luxembourgish";
      case Lang.Macedonian:				return "Macedonian";
      case Lang.Malagasy:					return "Malagasy";
      case Lang.Malay:							return "Malay";
      case Lang.Malayalam:					return "Malayalam";
      case Lang.Maltese:						return "Maltese";
      case Lang.Maori:							return "Maori";
      case Lang.Marathi:						return "Marathi";
      case Lang.Mari:							return "Mari";
      case Lang.Mongolian:					return "Mongolian";
      case Lang.Myanmar:						return "Myanmar";
      case Lang.Nepali:						return "Nepali";
      case Lang.Norwegian:					return "Norwegian";
      case Lang.Oriya:							return "Oriya";
      case Lang.Chichewa:					return "Chichewa";
      case Lang.Papiamento:				return "Papiamento";
      case Lang.Pashto:						return "Pashto";
      case Lang.Persian:						return "Persian";
      case Lang.Polish:						return "Polish";
      case Lang.Portuguese:				return "Portuguese";
      case Lang.Punjabi:						return "Punjabi";
      case Lang.QueretaroOtomi:		return "Queretaro Otomi";
      case Lang.Romanian:					return "Romanian";
      case Lang.Russian:						return "Russian";
      case Lang.Samoan:						return "Samoan";
      case Lang.ScotsGaelic:				return "Scots Gaelic";
      case Lang.SerbianCyrillic:		return "Serbian (Cyrillic)";
      case Lang.SerbianLatin:			return "Serbian (Latin)";
      case Lang.Sesotho:						return "Sesotho";
      case Lang.Shona:							return "Shona";
      case Lang.Sindhi:						return "Sindhi";
      case Lang.Sinhala:						return "Sinhala";
      case Lang.Slovak:						return "Slovak";
      case Lang.Slovenian:					return "Slovenian";
      case Lang.Somali:						return "Somali";
      case Lang.Spanish:						return "Spanish";
      case Lang.Sundanese:					return "Sundanese";
      case Lang.Swahili:						return "Swahili";
      case Lang.Swedish:						return "Swedish";
      case Lang.Tagalog:						return "Tagalog";
      case Lang.Tahitian:					return "Tahitian";
      case Lang.Tajik:							return "Tajik";
      case Lang.Tamil:							return "Tamil";
      case Lang.Tatar:							return "Tatar";
      case Lang.Telugu:						return "Telugu";
      case Lang.Thai:							return "Thai";
      case Lang.Tongan:						return "Tongan";
      case Lang.Turkish:						return "Turkish";
      case Lang.Turkmen:						return "Turkmen";
      case Lang.Udmurt:						return "Udmurt";
      case Lang.Uighur:						return "Uighur";
      case Lang.Ukrainian:					return "Ukrainian";
      case Lang.Urdu:							return "Urdu";
      case Lang.Uzbek:							return "Uzbek";
      case Lang.Vietnamese:				return "Vietnamese";
      case Lang.Welsh:							return "Welsh";
      case Lang.Xhosa:							return "Xhosa";
      case Lang.Yiddish:						return "Yiddish";
      case Lang.Yoruba:						return "Yoruba";
      case Lang.YucatecMaya:				return "Yucatec Maya";
      case Lang.Zulu:							return "Zulu";
      default: return "unknown";
    }
  }
}

