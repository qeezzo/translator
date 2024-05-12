namespace translator.ViewModels;

public class PromptsViewModel : ViewModelBase {
  public MainWindowViewModel MainWindowViewModel { get; }

  public string _Left = string.Empty;
  public string Left {
    get => _Left;
    set {
      _Left = value;
      RaisePropertyChanged();
    }
  }

  public OutputPromptViewModel Right { get; private set; } = new();

  public PromptsViewModel(MainWindowViewModel m) {
    this.MainWindowViewModel = m;
  }
}
