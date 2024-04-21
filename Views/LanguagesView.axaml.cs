using Avalonia.Controls;

namespace translator.Views;

public partial class LanguagesView : UserControl
{
    public LanguagesView()
    {
        InitializeComponent();

        // set default selected item
        /* ListBoxLeft.AttachedToVisualTree += (s, e) => ListBoxLeft.SelectedIndex = 0; */
        /* ListBoxRight.AttachedToVisualTree += (s, e) => ListBoxRight.SelectedIndex = ListBoxRight.Items.Count - 1; */
    }
}
