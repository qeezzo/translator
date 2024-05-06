using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace translator.Views.Settings;

public partial class BlankView : UserControl
{
    public BlankView()
    {
        InitializeComponent();
    }

    private void InitializeComponent()
    {
        AvaloniaXamlLoader.Load(this);
    }
}
