using System.Runtime.Serialization;
using translator.Models;

namespace translator.ViewModels.Settings;

[DataContract]
public class DatabaseViewModel : ViewModelBase {
  [DataMember]
  public string User { get; set; } = "root";

  [DataMember]
  public string Server {
    get; set;
  } = "renatus";

  [DataMember]
  public string Password {
    get; set;
  } = "8915";

  public void Reset() {
    (new HistoryModel()).ResetHistory(User, Server, Password);
  }

  public void Save(string file) {
    SettingsSerializer.Write(this, file);
  }

  public static DatabaseViewModel? Load(string file) {
    return SettingsSerializer.Read<DatabaseViewModel>(file);
  }
}
