using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Runtime.Serialization.Json;
using Avalonia.Data.Converters;

using translator.ViewModels.Settings;

namespace translator.ViewModels;

public class SettingsViewModel : ViewModelBase {
  static string GeneralFile  = "/tmp/translator.settings.general.json";
  static string DatabaseFile = "/tmp/translator.settings.database.json";

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
      _Category        = value;
    }
  }

  static public string LocaleSettings(string settings) {
    return (string)(MainWindowViewModel.LocaleDict[settings] ?? "...");
  }

  public List<string> Settings { get; private set; }

  public void UpdateSettings() {
    Settings = ["Settings.General", "Settings.Database"];
    RaisePropertyChanged(nameof(Settings));
  }

  public SettingsViewModel(MainWindowViewModel m) {
    Blank    = new();
    Database = new();
    General  = new(m);

    map = new() { { "empty", Blank }, { "Settings.General", General }, { "Settings.Database", Database } };

    Settings = ["Settings.General", "Settings.Database"];
  }

  public void Setup() {
    Database          = DatabaseViewModel.Load(DatabaseFile) ?? Database;
    var saved_general = GeneralViewModel.Load(GeneralFile);
    if (saved_general is not null) {
      General.Engine   = saved_general.Engine;
      General.Language = saved_general.Language;
    }

    map = new() { { "empty", Blank }, { "Settings.General", General }, { "Settings.Database", Database } };
  }

  public void SaveSettings() {
    Database.Save(DatabaseFile);
    General.Save(GeneralFile);
  }
}

public static class SettingsSerializer {
  public static void Write<T>(T viewModel, string filename)
      where T : ViewModelBase {
    if (File.Exists(filename))
      File.Delete(filename);

    using (FileStream stream = File.Create(filename)) {
      DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(T));
      serializer.WriteObject(stream, viewModel);
    }
  }

  public static T? Read<T>(string filename)
      where T : ViewModelBase {
    if (!File.Exists(filename))
      return null;

    using (FileStream stream = File.Open(filename, FileMode.Open)) {
      DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(T));
      return (T?)serializer.ReadObject(stream);
    }
  }
}

public class LocaleConverter : IValueConverter {
  public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture) {
    return (string)(MainWindowViewModel.LocaleDict[(string)value!] ?? "...");
  }

  public object ConvertBack(
      object? value, Type targetType, object? parameter, CultureInfo culture
  ) {
    throw new NotImplementedException();
  }
}
