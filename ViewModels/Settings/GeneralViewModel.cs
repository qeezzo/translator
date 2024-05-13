using System.Collections.Generic;
using System.Runtime.Serialization;
using translator.Models;

namespace translator.ViewModels.Settings;

[DataContract]
public class GeneralViewModel : ViewModelBase {
  public struct LanguageItem {
    public string String { get; set; }
    public Lang Lang { get; set; }
  }

  public List<LanguageItem> Languages { get; private set; }
  [DataMember]
  private LanguageItem _Language = new() { String = "en-US", Lang = Lang.English };
  public LanguageItem Language {
    get => _Language;
    set {
      _Language = value;
      MainWindowViewModel?.Locale(value.String);
    }
  }

  public List<Engine> Engines { get; private set; }
  [DataMember]
  private Models.Engine _Engine = Engine.Google;
  public Models.Engine Engine {
    get => _Engine;
    set { _Engine = value; }
  }

  private MainWindowViewModel? MainWindowViewModel = null;
  public GeneralViewModel(MainWindowViewModel m) {
    MainWindowViewModel = m;

    Engines   = new() { Engine.Google, Engine.Yandex };
    Languages = new() {
      new() { String = "ru-RU", Lang = Lang.Russian },
      new() { String = "en-US", Lang = Lang.English }
    };
  }

  public void Save(string file) {
    SettingsSerializer.Write(this, file);
  }

  public static GeneralViewModel? Load(string file) {
    return SettingsSerializer.Read<GeneralViewModel>(file);
  }
}
