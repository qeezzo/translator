using System.Collections.ObjectModel;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using translator.Models;

namespace translator.ViewModels;


public class LanguageLocale : Language {
  public LanguageLocale(Lang lang) : base(lang) {}
  public override string ToString() {
    return LanguagesViewModel.LocaleLanguage(Lang);
  }
}

public partial class LanguagesViewModel : ViewModelBase {
  public MainWindowViewModel? Main { get; } = null;

  public Language LeftSelected {get; set;} = new(Lang.Auto);
  public Language RightSelected {get; set;} = new(Lang.Auto);

  public ObservableCollection<Language> LeftList {get;} = new();
  public ObservableCollection<Language> RightList {get;} = new();
  public void UpdateLists() {
    AllLanguages.ForEach(l => l.RaisePropertyChanged(nameof(l.Name)));
  }

  // ~LanguagesViewModel() { WriteLists(); }
  public LanguagesViewModel() {}
  public LanguagesViewModel(MainWindowViewModel m) {
    this.Main = m;
    LeftList = new(defaults);
    RightList = new(defaults);
    LeftSelected = LeftList[0];
    RightSelected = RightList[0];

    /* ReadList(LeftList, leftFile); */
    /* ReadList(RightList, rightFile); */
    /* LeftList.CollectionChanged += (sender, args) => WriteList(LeftList, leftFile); */
    /* RightList.CollectionChanged += (sender, args) => WriteList(RightList, rightFile); */
  }

  public void ToggleLanguageSelection(ObservableCollection<Language> obj) {
    if (Main?.ContentViewModel is PromptsViewModel)
      Main.ContentViewModel = new LanguageSelectionViewModel(AllLanguages, obj);
    else
      Main!.ContentViewModel = Main!.Prompts;
  }

  private void ReadList(ObservableCollection<Language> list, string filename) {
    DataContractSerializer serializer = new DataContractSerializer(typeof(ObservableCollection<Language>));
    if (File.Exists(filename))
      using (FileStream fs = new FileStream(filename, FileMode.Open))
        if (fs.Length > 0) {
          list = (ObservableCollection<Language>?)serializer.ReadObject(fs) ?? list;
          return;
        }
  }

  private void WriteList(ObservableCollection<Language> list, string filename) {
    DataContractSerializer serializer = new DataContractSerializer(typeof(ObservableCollection<Language>));
    using (FileStream fs = new FileStream(filename, FileMode.Create))
      serializer.WriteObject(fs, list);
  }
}


// static part
public partial class LanguagesViewModel : ViewModelBase {
  /* private static readonly string leftFile = "/tmp/translator.left"; */
  /* private static readonly string rightFile = "/tmp/translator.right"; */
  private static readonly List<Language> AllLanguages;
  private static readonly List<Language> defaults;


  static LanguagesViewModel() {
    AllLanguages = new( Enum.GetValues<Lang>().Select(l => new LanguageLocale(l)));
    AllLanguages.Remove(AllLanguages.Find(l => l.Lang == Lang.NoLanguage)!);
    defaults = [
      AllLanguages.Find(l => l.Lang == Lang.Auto)!,
      AllLanguages.Find(l => l.Lang == Lang.English)!,
      AllLanguages.Find(l => l.Lang == Lang.Russian)!,
    ];
  }

  static public string LocaleLanguage(Lang lang) {
    return (string)(MainWindowViewModel.LocaleDict[LanguageManager.LanguageName(lang)] ?? "...");
  }
}
