using System.Text;
using System.Linq;
using System.Net.Sockets;
using translator.Models.Queries;
using System;

namespace translator.Models;

public class Client {
  static Client client = new();
  static public Client GetClient() {
    if (client is null)
      return client = new();
    return client;
  }

  ~Client() {
    server.Close();
  }

  private Client(int buffer_size = 1024 * 10) {
    buffer = new byte[buffer_size];
    Connect();
  }

  private TcpClient server      = new();
  private NetworkStream? stream = null;
  private readonly byte[] buffer;
  private int bytes_read;

  public string Error { get; private set; } = "";
  public bool Connected {
    get => server.Connected && !server.Client.Poll(1, SelectMode.SelectRead);
  }

  public bool Connect() {
    System.Console.Error.Write("Connecting ... ");
    try {
      server = new TcpClient();
      server.Connect("127.0.0.1", 30000);
    } catch (SocketException e) {
      System.Console.Error.WriteLine("failed");
      Error = e.Message;
      return false;
    }
    System.Console.Error.WriteLine("connected");
    stream = server.GetStream();
    return true;
  }

  public void Write(Query query) {
    var bytes = query.ToBytes();
    System.Console.Error.Write("Writing " + bytes.Length + " ... ");

    try {
      stream?.Write(bytes);
    } catch (Exception e) {
      Error = e.Message;
      return;
    }

    System.Console.Error.WriteLine("written");
  }
  public byte[] Read() {
    System.Console.Error.Write("Reading ... ");

    try {
      bytes_read = stream?.Read(buffer, 0, buffer.Length) ?? 0;
    } catch (Exception e) {
      Error = e.Message;
      return [];
    }

    System.Console.Error.WriteLine(bytes_read + " read");

    return ReadErrors() ? [] : buffer.Take(bytes_read).Skip(4).ToArray();
  }
  public string ReadString() {
    var bytes = Read();
    if (bytes.Length == 0)
      return "";
    string result = Encoding.Default.GetString(bytes);
    return result;
  }

  private bool ReadErrors() {
    if (bytes_read < 4)
      server.Close();

    int size = BitConverter.ToInt32(buffer, 0);
    if (size == 0)
      return false;

    Error = Encoding.Default.GetString(buffer, 4, size);

    System.Console.Error.WriteLine(Error);

    return true;
  }
}
