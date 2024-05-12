using System.Collections.Generic;
using System.Collections.ObjectModel;
using translator.Models;

namespace translator.ViewModels.Settings;

public class GeneralViewModel : ViewModelBase {
  public struct LanguageItem {
    public string String { get; set; }
    public Lang Lang { get; set; }
  }

  public List<LanguageItem> Languages { get; private set; }
  private LanguageItem _Language = new() { String = "en-US", Lang = Lang.English };
  public LanguageItem Language {
    get => _Language;
    set {
      _Language = value;
      MainWindowViewModel?.Locale(value.String);
    }
  }

  public List<Engine> Engines { get; private set; }
  private Models.Engine _Engine = Engine.Google;
  public Models.Engine Engine {
    get => _Engine;
    set { _Engine = value; }
  }

  private MainWindowViewModel? MainWindowViewModel;
  public GeneralViewModel(MainWindowViewModel m) {
    MainWindowViewModel = m;

    Engines   = new() { Engine.Google, Engine.Yandex };
    Languages = new() {
      new() { String = "ru-RU", Lang = Lang.Russian },
      new() { String = "en-US", Lang = Lang.English }
    };
  }
}
