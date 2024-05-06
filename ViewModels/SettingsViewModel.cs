using System.Collections.ObjectModel;
using System.Collections.Generic;

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

  public ObservableCollection<string> Settings { get; }

  public SettingsViewModel() {
    Blank = new();
    General = new();
    Database = new();

    map = new(){
      {"empty",    Blank},
      {"General",  General},
      {"Database", Database}
    };

    Settings = [
      "General",
      "Database"
    ];
  }
}
