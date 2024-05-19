using Avalonia.Controls;
using Avalonia.Markup.Xaml.Styling;
using System.Linq;
using System;
using translator.Models;

namespace translator.ViewModels;

public class MainWindowViewModel : ViewModelBase {
  public static ResourceInclude? translations;
  public static IResourceDictionary LocaleDict { get; set; }

#pragma warning disable CS8618
  static MainWindowViewModel() {
    _Locale("en-US");
  }
#pragma warning restore

  private TranslatorModel Translator { get; set; }
  private HistoryModel HistoryModel { get; set; }

  // public IResourceDictionary LocaleDict { get; set; }
  public PromptsViewModel Prompts { get; private set; }
  public LanguagesViewModel Languages { get; private set; }
  public SettingsViewModel Settings { get; private set; }
  public HistoryViewModel History { get; private set; }
  public ViewModelBase _ContentViewModel;
  public ViewModelBase ContentViewModel {
    get => _ContentViewModel;
    set {
      if (_ContentViewModel == value)
        return;
      if (_ContentViewModel is SettingsViewModel)
        Settings.SaveSettings();

      _ContentViewModel = value;
      RaisePropertyChanged();
    }
  }

  public MainWindowViewModel() {
    Prompts      = new(this);
    Languages    = new(this);
    Settings     = new(this); Settings.Setup();
    Translator   = new();
    HistoryModel = new();
    History      = new(HistoryModel);

    var db = Settings.Database;
    HistoryModel.ResetHistory(db.User, db.Server, db.Password);

    _ContentViewModel = Prompts;
  }

  public void ToggleSettings() {
    if (ContentViewModel is not SettingsViewModel)
      ContentViewModel = Settings;
    else {
      ContentViewModel = Prompts;
      // Settings.SaveSettings();
    }
  }

  public void ToggleHistory() {
    if (ContentViewModel is not HistoryViewModel) {
      // TODO: WASTEFULNESS!!! change some time
      var db = Settings.Database;
      HistoryModel.ResetHistory(db.User, db.Server, db.Password);

      History.AllHistory = HistoryModel.History;
      ContentViewModel   = History;
    } else
      ContentViewModel = Prompts;
  }

  public void ChangeTheme(string theme) {
    var themes = App.Current?.Resources.MergedDictionaries.OfType<ResourceInclude>().FirstOrDefault(
        x => x.Source?.OriginalString?.Contains("/Theme/") ?? false
    );

    if (themes != null)
      App.Current!.Resources.MergedDictionaries.Remove(themes);

    var newDictionary =
        new ResourceInclude(new Uri($"avares://translator/Assets/Theme/{theme}.axaml")
        ) { Source = new Uri($"avares://translator/Assets/Theme/{theme}.axaml") };
    App.Current?.Resources.MergedDictionaries.Add(newDictionary);
  }

  public void Locale(string locale) {
    _Locale(locale);
    Languages.UpdateLists();
    Settings.UpdateSettings();
  }

  static void _Locale(string locale) {
    var translations =
        App.Current?.Resources.MergedDictionaries.OfType<ResourceInclude>().FirstOrDefault(
            x => x.Source?.OriginalString?.Contains("/Lang/") ?? false
        );

    if (translations != null)
      App.Current!.Resources.MergedDictionaries.Remove(translations);

    var newDictionary =
        new ResourceInclude(new Uri($"avares://translator/Assets/Lang/{locale}.axaml")
        ) { Source = new Uri($"avares://translator/Assets/Lang/{locale}.axaml") };
    App.Current?.Resources.MergedDictionaries.Add(newDictionary);

    LocaleDict = newDictionary.Loaded;
  }

  public void Translate(object obj) {
    Prompts.Right.Translation = Translator.Translate(
        Prompts.Left,
        Settings.General.Engine,
        Languages.LeftSelected.Lang,
        Languages.RightSelected.Lang,
        Settings.General.Language.Lang
    );
  }
}
