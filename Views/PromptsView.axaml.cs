using Avalonia.Controls;


namespace translator.Views;

public partial class PromptsView : UserControl
{
    public PromptsView()
    {
        InitializeComponent();

        // auto focus on start
        PromptLeft.AttachedToVisualTree += (s, e) => PromptLeft.Focus();
    }
}
