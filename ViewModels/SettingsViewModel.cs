using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
using Avalonia.Data.Converters;

using translator.ViewModels.Settings;

namespace translator.ViewModels;

public class SettingsViewModel : ViewModelBase {
  public Dictionary<string, ViewModelBase> map;
  public DatabaseViewModel Database { get; private set; }
  public GeneralViewModel General { get; private set; }
  public BlankViewModel Blank { get; private set; }

  public ViewModelBase _ContentViewModel = new BlankViewModel();
  public ViewModelBase ContentViewModel {
    get => _ContentViewModel;
    set {
      _ContentViewModel = value;
      RaisePropertyChanged();
    }
  }
  public string _Category = "";
  public string Category {
    get => _Category;
    set {
      if (string.IsNullOrEmpty(value))
        value = "empty";
      ContentViewModel = map[value];
      _Category = value;
    }
  }

  static public string LocaleSettings(string settings) {
    return (string)(MainWindowViewModel.LocaleDict[settings] ?? "...");
  }

  public List<string> Settings { get; private set; }

  public void UpdateSettings() {
    Settings = [
      "Settings.General",
      "Settings.Database"
    ];
    RaisePropertyChanged(nameof(Settings));
  }

  public SettingsViewModel(MainWindowViewModel m) {
    Blank = new();
    General = new(m);
    Database = new();

    map = new(){
      {"empty",             Blank},
      {"Settings.General",  General},
      {"Settings.Database", Database}
    };

    Settings = [
      "Settings.General",
      "Settings.Database"
    ];
  }
}

public class LocaleConverter : IValueConverter {

  public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture) {
    return (string)(MainWindowViewModel.LocaleDict[(string)value!] ?? "...");
  }

  public object ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
  {
    throw new NotImplementedException();
  }
}
