using System;
using System.Collections.Generic;
using translator.Models;

namespace translator.ViewModels;

public class HistoryViewModel : ViewModelBase {
  private HistoryModel HistoryModel = new();

  public string _Input = "";
  public string Input {
    get => _Input;
    set {
      if (_Input == value)
        return;
      _Input = value;

      if (string.IsNullOrEmpty(value)) {
        History = AllHistory;
        RaisePropertyChanged(nameof(History));
        return;
      }

      History = [];

      foreach (var history in AllHistory)
        if (history.Source.ToLower().Contains(_Input, StringComparison.OrdinalIgnoreCase))
          History.Add(history);

      RaisePropertyChanged(nameof(History));
    }
  }

  private HistoryItem _HistoryItem;
  public HistoryItem HistoryItem {
    get => _HistoryItem;
    set {
      if (_HistoryItem.Id == value.Id)
        return;
      Translation.Translation = HistoryModel.GetTranslation(value.Id);

      _HistoryItem = value;
    }
  }

  public List<HistoryItem> _AllHistory = [];
  public List<HistoryItem> AllHistory {
    get => _AllHistory;
    set { _AllHistory = History = value; }
  }
  public List<HistoryItem> History { get; set; } = [];

  public OutputPromptViewModel Translation { get; private set; } = new();

  public HistoryViewModel(HistoryModel HistoryModel) {
    this.HistoryModel = HistoryModel;
  }
}
