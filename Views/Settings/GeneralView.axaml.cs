using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace translator.Views.Settings;

public partial class GeneralView : UserControl
{
    public GeneralView()
    {
        InitializeComponent();
    }

    private void InitializeComponent()
    {
        AvaloniaXamlLoader.Load(this);
    }
}
