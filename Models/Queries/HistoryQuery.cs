using System.IO;

namespace translator.Models.Queries;

public class HistoryQuery : Query {
  public override QueryType Type => QueryType.History;
  public int Id = 0;

  public override byte[] InnerBytes() {
    var stream = new MemoryStream();
    var writer = new BinaryWriter(stream);

    writer.Write(Id);

    return stream.ToArray();
  }
}
