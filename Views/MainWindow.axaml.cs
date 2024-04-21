using Avalonia.Controls;
using Avalonia;
using Avalonia.Interactivity;
using Avalonia.Media.Imaging;
using Avalonia.Markup.Xaml;
using Avalonia.Input;
using System.IO;

namespace translator.Views;

public partial class MainWindow : Window
{
  /* private UserControl _Languages; */

  public MainWindow()
  {
    InitializeComponent();
    #if DEBUG
      this.AttachDevTools();
    #endif

    /* _Languages = this.FindControl<UserControl>("Languages")!; */
    /* _Languages.PointerMoved += MyControl_PointerEnter; */
    /* // _Languages.PointerExited += MyControl_PointerEnter; */
  }

  /* private void MyControl_PointerEnter(object sender, RoutedEventArgs e) */
  /* { */
  /*   var cursorImage = new Bitmap("Assets/cat.png"); */
  /*   var stream = new MemoryStream(File.ReadAllBytes("Assets/cat.png")); */

  /*   // Create a custom cursor with the loaded image */
  /*   var customCursor = new Cursor(new Bitmap(stream), new Avalonia.PixelPoint(0, 0)); */
  /*   // var customCursor = new Cursor(StandardCursorType.Wait); */

  /*   // Set the custom cursor */
  /*   Cursor = customCursor; */
  /* } */
}
