using System.Collections.Generic;
using translator.Models;

namespace translator.ViewModels;

public class HistoryViewModel : ViewModelBase {
  private HistoryModel HistoryModel = new();

  private HistoryItem _HistoryItem;
  public HistoryItem HistoryItem {
    get => _HistoryItem;
    set {
      Translation.Translation = HistoryModel.GetTranslation(value.Id);

      _HistoryItem = value;
    }
  }

  public List<HistoryItem> History { get; set; } = [];

  public OutputPromptViewModel Translation { get; private set; } = new();

  public HistoryViewModel(HistoryModel HistoryModel) {
    this.HistoryModel = HistoryModel;
  }
}
