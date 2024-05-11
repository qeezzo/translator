using System;
using System.Collections.Generic;
using System.Text;
using translator.Models.Queries;

namespace translator.Models;

public class HistoryModel {
  private Client Client = Client.GetClient();

  public HistoryModel() {}

  public List<(int, string)> _History = [];
  public List<(int, string)> History {
    get {
      if (!CheckConnection())
        return [];
      return _History;
    }
  private
    set { _History = value; }
  }

  public void ResetHistory(string user, string server, string password) {
    if (!CheckConnection())
      return;

    Query query = new DatabaseConfigQuery { User = user, Server = server, Password = password };

    Client.Write(query);
    var bytes = Client.Read();

    History = ParseHistory(bytes);

    foreach (var hist in History) {
      Console.WriteLine(hist.Item1 + " : " + hist.Item2);
    }
  }

  private List<(int, string)> ParseHistory(byte[] bytes) {
    if (bytes.Length < sizeof(Int32))
      return [];

    int index = 0;
    int size  = BitConverter.ToInt32(bytes, index);
    index += sizeof(Int32);

    List<(int, string)> result = [];
    while (--size >= 0 && index + sizeof(Int32) <= bytes.Length) {
      int item_id = BitConverter.ToInt32(bytes, index);
      index += sizeof(Int32);

      if (index + sizeof(Int32) > bytes.Length)
        return result;

      int item_size = BitConverter.ToInt32(bytes, index);
      index += sizeof(Int32);

      if (index + item_size > bytes.Length)
        return result;

      string source = Encoding.Default.GetString(bytes, index, item_size);
      index += item_size;

      result.Add((item_id, source));
    }

    return result;
  }

  private bool CheckConnection() {
    return Client.Connected || Client.Connect();
  }
}
