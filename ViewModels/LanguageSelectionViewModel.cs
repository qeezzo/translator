using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.Linq;
using System;
using translator.Models;

namespace translator.ViewModels;

public class LanguageSelectionViewModel : ViewModelBase {
  public string _Input = "";
  public string Input {
    get => _Input;
    set {
      if (_Input == value) return;
      _Input = value;

      ObservableCollection<Language> selected = new(SelectedItems);
      Languages.Clear();

      foreach (var lang in AllLanguages)
        if (lang.Name.ToLower().Contains(_Input, StringComparison.OrdinalIgnoreCase))
          Languages.Add(lang);

      foreach (var lang in selected) SelectedItems.Add(lang);
    }
  }

  public List<Language> AllLanguages {get;}   // source
  public ObservableCollection<Language> Languages {get; set;} // filtered list
  public ObservableCollection<Language> SelectedItems {get; set;} = new();

  #pragma warning disable CS8618
  public LanguageSelectionViewModel() { Languages = new(); }
  #pragma warning restore
  public LanguageSelectionViewModel(List<Language> all, ObservableCollection<Language> selected) {
    AllLanguages = all;
    Languages = new(AllLanguages);
    SelectedItems = selected;
  }
}
