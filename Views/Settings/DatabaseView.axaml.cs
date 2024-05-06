using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace translator.Views.Settings;

public partial class DatabaseView : UserControl
{
    public DatabaseView()
    {
        InitializeComponent();
    }

    private void InitializeComponent()
    {
        AvaloniaXamlLoader.Load(this);
    }
}
