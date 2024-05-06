using Avalonia.Controls;
using Avalonia.Markup.Xaml.Styling;
using System.Linq;
using System;
using translator.Models;

namespace translator.ViewModels;

public class MainWindowViewModel : ViewModelBase {
  public static ResourceInclude? translations;
  public static IResourceDictionary LocaleDict {get; set;}

  #pragma warning disable CS8618
  static MainWindowViewModel() { _Locale("en-US"); }
  #pragma warning restore

  private TranslatorModel Translator { get; set; }
  private HistoryModel History { get; set; }

  // public IResourceDictionary LocaleDict { get; set; }
  public PromptsViewModel Prompts { get; private set; }
  public LanguagesViewModel Languages { get; private set; }
  public SettingsViewModel Settings { get; private set; }
  public ViewModelBase _ContentViewModel;
  public ViewModelBase ContentViewModel {
    get => _ContentViewModel;
    set {
      if (_ContentViewModel == value) return;
      _ContentViewModel = value;
      RaisePropertyChanged();
    }
  }

  public MainWindowViewModel() {
    Prompts = new(this);
    Languages = new(this);
    Settings = new();
    Translator = new();
    History = new();
    _ContentViewModel = Prompts;
  }

  public void ToggleSettings() {
    if (ContentViewModel is not SettingsViewModel)
      ContentViewModel = Settings;
    else ContentViewModel = Prompts;
  }

  public void ToggleHistory() {
    var db = Settings.Database;
    History.ResetHistory(db.User, db.Server, db.Password);

  }

  public void ChangeTheme(string theme) {
    var themes = App.Current?.Resources
      .MergedDictionaries.OfType<ResourceInclude>()
      .FirstOrDefault(x => x.Source?.OriginalString?.Contains("/Theme/") ?? false);

    if (themes != null)
      App.Current!.Resources.MergedDictionaries.Remove(themes);

    var newDictionary = new ResourceInclude(new Uri($"avares://translator/Assets/Theme/{theme}.axaml")) {
        Source = new Uri($"avares://translator/Assets/Theme/{theme}.axaml")
    };
    App.Current?.Resources.MergedDictionaries.Add(newDictionary);
  }

  public void Locale(string locale) {
    _Locale(locale);
    Languages.UpdateLists();
  }

  static void _Locale(string locale) {
    var translations = App.Current?.Resources
      .MergedDictionaries.OfType<ResourceInclude>()
      .FirstOrDefault(x => x.Source?.OriginalString?.Contains("/Lang/") ?? false);

    if (translations != null)
      App.Current!.Resources.MergedDictionaries.Remove(translations);

    // var resource = AssetLoader.Open(new Uri($"avares://LocalizationSample/Assets/Lang/{targetLanguage}.axaml"));

    var newDictionary = new ResourceInclude(new Uri($"avares://translator/Assets/Lang/{locale}.axaml")) {
        Source = new Uri($"avares://translator/Assets/Lang/{locale}.axaml")
    };
    App.Current?.Resources.MergedDictionaries.Add(newDictionary);

    LocaleDict = newDictionary.Loaded;
  }

  public void Translate(object obj) {
    Prompts.Right = Translator.Translate(
      Prompts.Left,
      Engine.Google,
      Languages.LeftSelected.Lang,
      Languages.RightSelected.Lang,
      Lang.Russian
    );
  }
}
