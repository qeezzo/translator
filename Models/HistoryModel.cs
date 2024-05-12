using System;
using System.Collections.Generic;
using System.Text;
using translator.Models.Queries;

namespace translator.Models;

public struct HistoryItem {
  public int Id { get; set; }
  public string Source { get; set; }

  public HistoryItem(int id, string source) {
    Id     = id;
    Source = source;
  }

}

public class HistoryModel {
  private Client Client = Client.GetClient();

  public HistoryModel() {}

  public List<HistoryItem> _History = [];
  public List<HistoryItem> History {
    get {
      return _History;
    }
  private
    set { _History = value; }
  }

  public TranslationResponse GetTranslation(int id) {
    if (!CheckConnection())
      return new();

    Query query = new HistoryQuery { Id = id };

    Client.Write(query);
    var bytes = Client.Read();

    return ParseTranslation(bytes);
  }

  public void ResetHistory(string user, string server, string password) {
    if (!CheckConnection())
      return;

    Query query = new DatabaseConfigQuery { User = user, Server = server, Password = password };

    Client.Write(query);
    var bytes = Client.Read();

    History = ParseHistory(bytes);
  }

  private TranslationResponse ParseTranslation(byte[] bytes) {
    string json = Encoding.Default.GetString(bytes);
    return TranslatorModel.ParseJsonResponse(json);
  }

  private List<HistoryItem> ParseHistory(byte[] bytes) {
    if (bytes.Length < sizeof(Int32))
      return [];

    int index = 0;
    int size  = BitConverter.ToInt32(bytes, index);
    index += sizeof(Int32);

    List<HistoryItem> result = [];
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

      result.Add(new HistoryItem(item_id, source));
    }

    return result;
  }

  private bool CheckConnection() {
    return Client.Connected || Client.Connect();
  }
}
