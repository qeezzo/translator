using System;
using System.Text;
using System.Net.Sockets;
using translator.Models.Utils;
using System.Collections.Generic;
using Newtonsoft.Json;

namespace translator.Models;


public class TranslatorModel {
  static public List<Language> Languages {get;} = new();
  static TranslatorModel() {
    foreach (Lang lang in Enum.GetValues<Lang>())
      Languages.Add(new Language(lang));
  }

  public void Dispose() {
    server.Close();
  }

  public TranslatorModel(int buffer_size = 1024*10) {
    buffer = new byte[buffer_size];
    TryConnect();
  }

  public TranslationResponse Translate(string data, Engine engine, Lang source, Lang dest, Lang ui) {
    if (!server.Connected) {
      var try_connect = TryConnect();
      if (!try_connect.Item1)
        return new TranslationResponse { error = try_connect.Item2};
    }

    Query query = new Query {
      engine = engine,
      source = source,
      dest = dest,
      ui = ui,
      data = data.ToCharArray()
    };

    Write(query);
    int bytes_read = ReadToBuffer();
    string response = GetStringFromBuffer(bytes_read);
    TranslationResponse translation = ParseJsonResponse(response);

    return translation;
  }


  private TcpClient server = new();
  private NetworkStream? stream = null;
  private byte[] buffer;

  private (bool, string) TryConnect() {
    System.Console.Write("Connecting ... ");
    try {
      server.Connect("127.0.0.1", 30000);
    } catch (SocketException e) {
      System.Console.WriteLine("Failed");
      return (false, e.Message);
    }
    System.Console.WriteLine("Connected");
    stream = server.GetStream();
    return (true, "");
  }
  private void Write(Query query) {
    var bytes = query.ToBytes();
    System.Console.Write("Writing: " + bytes.Length);
    stream?.Write(bytes);
    System.Console.WriteLine(" ... Written");
  }
  private int ReadToBuffer() {
    System.Console.Write("Reading: ");
    int bytes_read = stream?.Read(buffer) ?? 0;
    System.Console.WriteLine(bytes_read + "... Read");
    return bytes_read;
  }
  private string GetStringFromBuffer(int count) {
    string result = Encoding.Default.GetString(buffer, 0, count);
    return result;
  }
  private TranslationResponse ParseJsonResponse(string json) {
    return JsonConvert.DeserializeObject<TranslationResponse>(json);
  }
}
