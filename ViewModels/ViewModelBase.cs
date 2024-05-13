using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Runtime.Serialization;

namespace translator.ViewModels;

[DataContract]
public class ViewModelBase : INotifyPropertyChanged
{
  public event PropertyChangedEventHandler? PropertyChanged;

  public void RaisePropertyChanged([CallerMemberName] string? propertyName = null) {
    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
  }
}
