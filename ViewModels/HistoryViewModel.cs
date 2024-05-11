using System.Collections.ObjectModel;
using System.Collections.Generic;

namespace translator.ViewModels;

public class HistoryViewModel : ViewModelBase {

  public (int, string) HistoryItem { get; set; }
  public List<(int, string)> History { get; set; } = [];

  public HistoryViewModel() {}
}
