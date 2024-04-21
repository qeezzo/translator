using System.Windows.Input;
using System;

namespace translator.Commands;

public class Command : ICommand {

  private Action<object?> execute;
  private bool canExecute = true;

  public event EventHandler? CanExecuteChanged;

  public Command(Action<object?> execute) {
    this.execute = execute;
  }

  public void Execute (object? parameter) {
    execute(parameter);
  }

  public bool CanExecute (object? parameter) {
    return canExecute;
  }

  public void Lock() {
    canExecute = false;
    CanExecuteChanged?.Invoke(this, new());
  }

  public void UnLock() {
    canExecute = true;
    CanExecuteChanged?.Invoke(this, new());
  }
}
