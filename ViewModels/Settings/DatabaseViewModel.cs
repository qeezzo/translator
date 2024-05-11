namespace translator.ViewModels.Settings;

public class DatabaseViewModel : ViewModelBase {
  public string User { get; set; } = "root";
  public string Server { get; set; } = "renatus";
  public string Password { get; set; } = "8915";
}
