using System;
using System.Collections.Generic;
using System.Globalization;
using Avalonia.Data.Converters;
using translator.Models;

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
  public TranslationResponse _Right = new();
  public TranslationResponse Right {
    get => _Right;
    set {
      _Right = value;
      RaisePropertyChanged();
    }
  }

  public PromptsViewModel(MainWindowViewModel m) {
    this.MainWindowViewModel = m;
  }
}
public static class Converters 
{
    public static FuncValueConverter<List<string>, string> ListConverter { get; } = 
      new FuncValueConverter<List<string>, string> (list => string.Join(", ", list!));
}

public class KeyValuePairConverter : IValueConverter {

  public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture) {
    if (value is KeyValuePair<string, List<Option>> options) {
      if (parameter as string == "Key")
        return options.Key;
      else if (parameter as string == "Value")
        return options.Value;
    }
    if (value is KeyValuePair<string, List<Example>> examples) {
      if (parameter as string == "Key")
        return examples.Key;
      else if (parameter as string == "Value")
        return examples.Value;
    }
    throw new NotImplementedException();
  }

  public object ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
  {
    throw new NotImplementedException();
  }
}
