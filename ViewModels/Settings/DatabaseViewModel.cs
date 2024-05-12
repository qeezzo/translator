using System.Runtime.Serialization;

namespace translator.ViewModels.Settings;

[DataContract]
public class DatabaseViewModel : ViewModelBase {
  [DataMember]
  public string User { get; set; } = "root";
  [DataMember]
  public string Server { get; set; } = "renatus";
  [DataMember]
  public string Password { get; set; } = "8915";
}
